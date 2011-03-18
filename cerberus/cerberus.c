/***************************************************************************
                          cerberus.c  -  description
                             -------------------
    begin                : Wed Mar 19 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/


#include "cerberus.h"

#ifdef WIN32
int winfork() { return -1; }
int winwait4(int a, void *b, int c, void *d) { return 0; }
#define cer_fork winfork
#define cer_wait4 winwait4
#else
#define cer_fork fork
#define cer_wait4 wait4
#endif

// ##########################################################################
// ################=- main() -=##############################################
// ##########################################################################

int main(int argc, char **argv) {
  int       exit_code=0;

  CERBFIG*  cer_config=NULL;

  // linked list to store the messages in
  struct linkedlist* messages=NULL;

  CXMLROOT* xml_root=NULL;
  CFILE*    file=NULL;
  CFSYS*    cfsys=NULL;

  // log struct
  CLOG_INFO* log=NULL;

#ifdef MEMWATCH
  EF_ALIGNMENT=1;
  EF_PROTECT_BELOW=1;
  EF_PROTECT_FREE=1;
#endif

// memory checking
// mtrace();

// ##########################################################################
// ############=- CHECK CMD LINE PARAMETERS -=###############################
// ##########################################################################

  // start the logging so we can log!
  if( !(argc==4) ) {
    fprintf(stderr, "\nUsage:\n%.80s xml_config_file log_level log.txt \n\n", argv[0]);
  }

// ##########################################################################
// ############=- Run Test Suite -=##########################################
// ##########################################################################
#ifndef NOTEST
  if(1==argc) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    printf("Running Test Suite\n");

    CuSuiteAddSuite(suite, CuGetSuite());
    CuSuiteAddSuite(suite, CuStringGetSuite());
    CuSuiteAddSuite(suite, TestSuite__cstring());
    CuSuiteAddSuite(suite, TestSuite__cfile());
    CuSuiteAddSuite(suite, TestSuite__cxml());
//    CuSuiteAddSuite(suite, TestSuite__cmime());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);

    CuStringFree(output);
    CuSuiteFree(suite);

    return EX_USAGE;
  }
#endif

  log = clog_open(argv[3], clog_getlevel(argv[2]), NULL, 0);

  // if we couldn't log to the file, let's log to stderr!
  if(NULL!=log && NULL==log->s_logfile) {
    clog_setcallback(log, clog_stderr);
    clog_setcallbacklevel(log, clog_getlevel(argv[2]));
    clog(log, CERROR, "Could not log to file, logging to stderr!");
  }

  clog(log, CMARK, "Cerberus v. 2.x build %s Starting", BUILDNUMBER);

  clog(log, CDEBUG, "CMDLINE: Command line: %.80s %.80s %.80s", argv[0], argv[1], argv[2]);

  clog(log, CDEBUG, "CMDLINE: Command line arguments check passed");

  clog(log, CMARK, "Cerberus Starting...");

// ##########################################################################
// ############=- LOAD XML CONFIG FILE -=####################################
// ##########################################################################

  // parse the XML now
  clog(log, CDEBUG, "XML: Starting XML config file parsing");

  clog(log, CDEBUG, "XML: Creating XML DOM Variable");
  xml_root = cxml_root_new(log);
  clog(log, CDEBUG, "XML: XML DOM Variable Created");

  // make a new cer filesystem obj
  cfsys = cfile_init(0);
  
  cer_config = malloc(sizeof(CERBFIG));
  memset(cer_config, 0, sizeof(CERBFIG));

  cer_config->cfsys = cfsys;
  
  exit_code = cer_load_config(log, &xml_root, argv[1], &cer_config);

  // ##########################################################################
  // ################=- READ IN FILES -=#######################################
  // ##########################################################################

  if(0==exit_code) {
    CPOP3* pop3 = NULL;
    char *filename = NULL;

    messages = linkedlist_new(0);
    // if there is something in the list, process via pop3
    if(NULL!=cer_config->poplist) {

      exit_code = cer_curl_init(log, &cer_config, cer_config->curl_location);

      clog(log, CMARK, "Parser is in POP3 mode.");


      while(NULL!=(pop3=linkedlist_remove_last(cer_config->poplist))) {
        if(0==exit_code) {
          if(NULL!=pop3->user && NULL!=pop3->pass) {
            if(0==cpop3_connect(log, pop3)){
              if(0==cpop3_user(log, pop3, pop3->user)) {
                if(0==cpop3_pass(log, pop3, pop3->pass)) {
                  int x=0;
                  int y=0;
                  x = cpop3_stat(log, pop3);
                  while(y<x && y<cer_config->pop3_max) {
                    if(NULL!=(filename = cpop3_retr(log, pop3, cer_config->tmp_cerbmail->string))) {
                      int pid = 0;
                      linkedlist_add(messages, filename);

                      pid = cer_fork();
                      if(-1==pid) {
                        clog(log, CDEBUG, "FORK: Could not fork, running straight through");
                        // if we couldn't fork run the parser and risk killing entire process
                        exit_code = cer_parse_files(log, &cer_config, xml_root, &messages);

                        if(pop3->dele) {
                          if(1==cer_config->pop3_max_delete) {
                            cpop3_dele(log, pop3);
                          }
                          else if(0==exit_code) {
                            cpop3_dele(log, pop3);
                          }
                        }
                      }
                      else if(0==pid) {
                        clog(log, CDEBUG, "FORK: Forked, running file parser");
                        exit_code = cer_parse_files(log, &cer_config, xml_root, &messages);

                        if(pop3->dele) {
                          if(1==cer_config->pop3_max_delete) {
                            cpop3_dele(log, pop3);
                          }
                          else if(0==exit_code) {
                            cpop3_dele(log, pop3);
                          }
                        }
                        // didn't send quit, just close the fork'd connection
                        cpop3_disconnect(log, pop3);
                        cpop3_free(log, &pop3);

                        // remove the emails from the list that we've processed
                        linkedlist_iterate(messages);
                        while(NULL!=(filename=linkedlist_remove_last(messages))) {
                          free(filename);
                          filename = NULL;
                        }
                        linkedlist_del(cer_config->poplist, free);
                        linkedlist_del(messages, free);
                        goto CLEANUP;
                      }
                      else {
                        // must be the parent process
                        // clean up any forked children that are sitting around
                        clog(log, CDEBUG, "FORK: Forked, am parent waiting for child process");
                        while(0<(pid=cer_wait4(0, NULL, 0, NULL))) {
                          clog(log, CDEBUG, "WAIT: cleaned up after child %d!", pid);
                        };
                      }
                      // remove the emails from the list that we've processed
                      linkedlist_iterate(messages);
                      while(NULL!=(filename=linkedlist_remove_last(messages))) {
                        free(filename);
                        filename = NULL;
                      }
                    }
                    ++y;
                  }
                }
              }
              cpop3_quit(log, pop3);
              cpop3_disconnect(log, pop3);
            }
          }
          else {
            clog(log, CERROR, "POP3: User or Password was NULL, skipping");
          }

        }
        cpop3_free(log, &pop3);
      }

      linkedlist_del(cer_config->poplist, free);
    }
    // otherwise it's stdin
    else {
      clog(log, CMARK, "Parser is in PIPE mode, waiting for input");
      file = cer_save_input(log, cfsys, cer_config->tmp_cerbmail->string);
      if(NULL!=file) {
        filename = strdup(file->filename);
        linkedlist_add(messages, filename);
        cfile_close(&file);
        cfile_free(&file);
  
        exit_code = cer_curl_init(log, &cer_config, cer_config->curl_location);
  
        if(0==exit_code) {
          exit_code = cer_parse_files(log, &cer_config, xml_root, &messages);
        }
      }
    }

    // free the linked list
    linkedlist_del(messages, free);
  }

  // this is above cleanup to keep the forks from clobbering each other
  cfile_cleanup(&cfsys);
  
CLEANUP:

  if(NULL!=dl_curl_easy_cleanup) {
    // clean up the info in cURL
    dl_curl_easy_cleanup(cer_config->curl);
  }
  if(NULL!=dl_curl_global_cleanup) {
    // close down curl
    dl_curl_global_cleanup();
  }

#ifndef WIN32
  if(NULL!=dl_curl) {
     dlclose(dl_curl);
     dl_curl=NULL;
  }
#endif

  dl_curl = NULL;
  dl_curl_formadd=NULL;
  dl_curl_formfree=NULL;
  dl_curl_global_init=NULL;
  dl_curl_easy_init=NULL;
  dl_curl_easy_setopt=NULL;
  dl_curl_easy_perform=NULL;
  dl_curl_easy_cleanup=NULL;
  dl_curl_global_cleanup=NULL;

  // free the xml data
  cxml_root_free(log, &xml_root);
  if(NULL!=cer_config->xsp) cstring_free(&cer_config->xsp);

  cstring_free(&cer_config->curl_location);

  cstring_free(&cer_config->tmp_cerbmail);
  cstring_free(&cer_config->tmp_cerbmime);

  // free the optional SSL data
  cstring_free(&cer_config->curl_cainfo);
  cstring_free(&cer_config->curl_capath);
  cer_config->curl_verifyhostpeer = 0;

  clog(log, CMARK, "Shutting Down");

  // close the log file
  clog_close(log);
  
  free(cer_config);
  cer_config=NULL;

  return exit_code;
}
