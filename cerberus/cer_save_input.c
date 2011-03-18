// ##########################################################################
// ################=- save_input() -=########################################
// ##########################################################################

#include "cerberus.h"

CFILE* cer_save_input(CLOG_INFO* log, CFSYS* cfsys, char *pathfile) {
  CFILE* file = NULL;
  int num = 0;

  if(NULL==pathfile) {
    return NULL;
  }

  file = cfile_mkstemp(&cfsys, pathfile);
  if(NULL==file) {
    clog(log, CFATAL, "**** BEGIN ERROR MESSAGE ****");
    clog(log, CFATAL, "We tried to make a temp file and failed.");
    clog(log, CFATAL, "The file template we used was: %s", pathfile);
    clog(log, CFATAL, "On the next line we will print the error message");
    clog(log, CFATAL, "that was given to us by the Operating System:");
    clog_perror(log, CFATAL, "Reason:");
    clog(log, CFATAL, "****  END ERROR MESSAGE  ****");
    return NULL;
  }

  num = cfile_pipe(&file, &PCFILE_STDIN, 1); // save and convert to dos format
  if(0>=num) {
    clog(log, CFATAL, "**** BEGIN ERROR MESSAGE ****");
    clog(log, CFATAL, "You are getting this error because the parser is in PIPing mode");
    clog(log, CFATAL, "and is being run from cron. If you want the parser in POP3 mode");
    clog(log, CFATAL, "remove the <!-- and --> lines from around the <pop3> section");
    clog(log, CFATAL, "in the config.xml file. Or it did not receive any input from");
    clog(log, CFATAL, "the console/pipe.");
    clog(log, CFATAL, "****  END ERROR MESSAGE  ****");
    // cleanup before returning
    cfile_close(&file);
    cfile_free(&file);
    return NULL;
  }

  // rewind to the beginning of the file
  num = cfile_lseek(&file, 0, SEEK_SET);

  // return the file integer
  return file;
}
