#include "cerberus.h"

  // ##########################################################################
  // ##############=- PARSE FILES -=###########################################
  // ##########################################################################
int cer_parse_files(CLOG_INFO* log, CERBFIG** pcer_config, CXMLROOT* xml_root, struct linkedlist** pmessages)
{
  // key storage var
  CSTRING*  dk=NULL;
  CSTRING*  line=NULL;
  CXMLNODE* xml=NULL;
  CXMLNODE* xml_node=NULL;
  CXMLNODE* node=NULL;
  CSTRING*  curl_return=NULL;
  int       return_code = 0;
  CFILE*    file=NULL;
  CERBFIG*  cer_config = *pcer_config;
  struct linkedlist* messages=*pmessages;

  char *filename = NULL;

  linkedlist_iterate(messages);
  while(NULL!=(filename=linkedlist_next(messages))) {
    unsigned int email_delivered=0;
    unsigned int email_matched=0;
    unsigned int key_errors=0;
    return_code=0; // reset for each pop3 loop
    file = cfile_open(&(cer_config->cfsys), filename, O_RDONLY);

    // log what file we're going to process
    clog(log, CDEBUG, "cer_parse_files(): Processing %s", filename);

    xml = cmime_file_parse(log, &file, cer_config->tmp_cerbmime);
    
    if(cer_config->debug_parse) {
    	if(cer_config->print_xml) {
    		line = cxml_node_tostring( NULL, xml, 0);
    		printf("XML:\n%s\n", line->string);
    	}

	// simulate the email delivered - so it cleans up
	email_delivered=1;
    	
    	goto CER_PARSE_FILES_CLEANUP;
    }
    
    if(NULL==xml) {
      return_code=EX_SOFTWARE;
    }
    else if(NULL!=xml_root->data) {
      CXMLNODE *snode=NULL;
      CXMLROOT *xml_key=NULL;
      int _found = 0;
      char* _tmp;
      xml_node = xml_root->data;

      // add the version number to the XML for the GUI.
      snode = cxml_node_get(log, xml, "email", NULL);
      snode = cxml_node_create(log, snode, "parser_version", 14);
      cxml_node_adddata(log, snode, "2.x build ", 10);
      cxml_node_adddata(log, snode, BUILDNUMBER, strlen(BUILDNUMBER));

      // add the cerbmail filename to the xml packet
      snode = cxml_node_get(log, xml, "email", NULL);
      snode = cxml_node_create(log, snode, "cerbmail", 8);
      cxml_node_adddata(log, snode, filename, strlen(filename));

      // parse any mime encoded subject lines.
      snode = cxml_node_get(log, xml, "email","headers","subject",NULL);
      if(NULL!=snode) {
        snode->data = cmime_parse_subject(log, snode->data);
      }

      if(NULL!=cer_config->xsp && 0<cer_config->xsp->length) {
        struct curl_httppost* post=NULL;
        struct curl_httppost* last=NULL;
        CXMLNODE *xsp_key = cer_key_info(log, cer_config->xsp);

        // get URL from xsp key
        if(NULL!=xsp_key) {
          CXMLNODE *xsp_url = NULL;

          cxml_node_iterate(log, xsp_key);
          if(NULL!=(xsp_url=cxml_node_next(log, xsp_key, "domain"))) {
            CSTRING *cs_user = cstring_import("user", 4);
            CSTRING *cs_pass = cstring_import("password", 8);
            CSTRING *cs_xml = cstring_import("xml", 3);
            CSTRING *cs_action = cstring_import("action", 6);
            CSTRING *cs_boot = cstring_import("boot", 4);

            line = cxml_node_tostring( NULL, xml, 0);

            // turn on verbose logging for curl
            if(CDEBUG==log->s_logfile_level) {
              dl_curl_easy_setopt(cer_config->curl, CURLOPT_VERBOSE, 1);
              dl_curl_easy_setopt(cer_config->curl, CURLOPT_DEBUGFUNCTION, cer_curl_error);
              dl_curl_easy_setopt(cer_config->curl, CURLOPT_DEBUGDATA, log);
            }

            // use the cer_curl_write function instead
            dl_curl_easy_setopt(cer_config->curl, CURLOPT_WRITEFUNCTION, cer_curl_write);

            // pass a cstring obj to the 4th param of cer_curl_write()
            curl_return = cstring_new(0);
            dl_curl_easy_setopt(cer_config->curl, CURLOPT_FILE, curl_return);

            if(NULL!=cer_config->curl_cainfo) {
              clog(log, CDEBUG, "SSL: Setting CURLOPT_CAINFO to %s", cer_config->curl_cainfo->string);
              dl_curl_easy_setopt(cer_config->curl, CURLOPT_CAINFO, cer_config->curl_cainfo->string);
            }
            if(NULL!=cer_config->curl_capath) {
              clog(log, CDEBUG, "SSL: Setting CURLOPT_CAPATH to %s", cer_config->curl_capath->string);
              dl_curl_easy_setopt(cer_config->curl, CURLOPT_CAPATH, cer_config->curl_capath->string);
            }
            switch(cer_config->curl_verifyhostpeer) {
              case 0:
                clog(log, CDEBUG, "SSL: Setting SSL Certificate verify to off");
                dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYHOST, (long) 0);
                dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYPEER, (long) 0);
                break;
              case 1:
                clog(log, CDEBUG, "SSL: Setting SSL Certificate verify to check for a domain");
                dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYHOST, (long) 1);
                dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYPEER, (long) 1);
                break;
              case 2:
                clog(log, CDEBUG, "SSL: Setting SSL Certificate verify to match domains");
                dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYHOST, (long) 2);
              default:
                break;
            }


            // check for boot loader xml
            clog(log, CDEBUG, "XML-BOOT: Time to get the key node");

            // add the user
            if(NULL!=cer_config->parser_user) {
              dl_curl_formadd(&post, &last,
                              CURLFORM_PTRNAME, cs_user->string,
                              CURLFORM_NAMELENGTH, cs_user->length,
                              CURLFORM_PTRCONTENTS, cer_config->parser_user->string,
                              CURLFORM_CONTENTSLENGTH, cer_config->parser_user->length ,CURLFORM_END);
            }

            // add the password
            if(NULL!=cer_config->parser_pass) {
              dl_curl_formadd(&post, &last,
                               CURLFORM_PTRNAME, cs_pass->string,
                               CURLFORM_NAMELENGTH, cs_pass->length,
                               CURLFORM_PTRCONTENTS, cer_config->parser_pass->string,
                               CURLFORM_CONTENTSLENGTH, cer_config->parser_pass->length,
                               CURLFORM_END);
            }

            dl_curl_formadd(&post, &last, 
            					CURLFORM_PTRNAME, cs_xml->string, 
            					CURLFORM_NAMELENGTH, cs_xml->length, 
								CURLFORM_PTRCONTENTS, line->string, 
								CURLFORM_CONTENTSLENGTH, line->length,
								CURLFORM_END);

            dl_curl_formadd(&post, &last, 
            					CURLFORM_PTRNAME, cs_action->string, 
            					CURLFORM_NAMELENGTH, cs_action->length, 
								CURLFORM_PTRCONTENTS, cs_boot->string, 
								CURLFORM_CONTENTSLENGTH, cs_boot->length,
								CURLFORM_END);

            dl_curl_easy_setopt(cer_config->curl, CURLOPT_HTTPPOST, post);

            if(1==cer_config->parser_https) {
              cer_config->parser_url = cstring_import("https://", 8);
            }
            else {
              cer_config->parser_url = cstring_import("http://", 7);
            }
            cstring_strcat(cer_config->parser_url, xsp_url->data);

            clog(log, CDEBUG, "Giving cURL URL: %s", cer_config->parser_url->string);

            dl_curl_easy_setopt(cer_config->curl, CURLOPT_URL, cer_config->parser_url->string);

            // post to the url
            clog(log, CDEBUG, "Posting via cURL");
            dl_curl_easy_perform(cer_config->curl);

            dl_curl_formfree(post);
            
            // print debugging infomation
            if(cer_config->print_xml) printf("XSP-XML:\n%s\n", line->string);

            cstring_free(&line);
            cstring_free(&cs_user);   cs_user=NULL;
            cstring_free(&cs_pass);   cs_pass=NULL;
            cstring_free(&cs_xml);    cs_xml=NULL;
            cstring_free(&cs_action); cs_action=NULL;

            // load xml
            if(NULL!=curl_return && 0<curl_return->length) {

              // print debugging infomation
              if(cer_config->print_curl) printf("XSP-CURL:\n%s\n", curl_return->string);

              // parse the XML now
              clog(log, CDEBUG, "XML-BOOT: Starting XML key parsing");

              clog(log, CDEBUG, "XML-BOOT: Creating XML DOM Variable");
              xml_key = cxml_root_new(log);
              clog(log, CDEBUG, "XML-BOOT: XML KEY Variable Created");

              clog(log, CDEBUG, "XML-BOOT: Loading the XML KEY Config");
              if(0==cxml_read_buf(log, xml_key, curl_return->string)) {
                clog(log, CFATAL, "XSP: %s", curl_return->string);
                return EX_SOFTWARE;
              }
              clog(log, CDEBUG, "XML-BOOT: XML KEY Config file loaded");
              clog(log, CDEBUG, "XML-BOOT: Checking for KEY XML Config element");
              if(NULL!=xml_key && NULL!=xml_key->data) {
                // set the contents to the key/parser elements retrieved from xSP
                xml_node = xml_key->data;
                clog(log, CDEBUG, "XML-BOOT: Found KEY XML element");
                cxml_node_iterate(log, xml_node);
                while(NULL!=(node=cxml_node_next(log, xml_node, "key"))) {
                  // add the key we read from the original config.xml file
                  cxml_node_adddata(log, node, cer_config->xsp->string, cer_config->xsp->length);
                }
              }
              else {
                clog(log, CFATAL, "XML-BOOT: KEY XML element not found");
              }

              cstring_free(&curl_return);
            }
            else {
              clog(log, CFATAL, " ---- XSP-BOOT ERROR ----");
              clog(log, CFATAL, "XSP-BOOT: cURL did not return any data!");
              clog(log, CFATAL, "XSP-BOOT: Check the login information in the config.xml file");
              clog(log, CFATAL, " ---- XSP-BOOT ERROR ----");
            }
          }
        }
      }

      // LOAD KEY FROM XML FILE
      cxml_node_iterate(log, xml_node);
      // only  if _found is 0 or 1, found will be -1 and  2 for free keys
      while((0==_found || 1==_found) && NULL!=(node=cxml_node_next(log, xml_node, "key"))) {
        _found = 0; // reset for each loop
        if(NULL!=node->data) {
          int key_type=0;
          CXMLNODE* key_type_node=NULL;
          CXMLNODE* _keyxml=cer_key_info(log, node->data);

          key_type_node = cxml_node_get(log, _keyxml, "key", "type", NULL);
          if(NULL!=key_type_node) {
            key_type = atoi(key_type_node->data->string);
          }

          // check the URL against the local IPs
          if(7 == key_type) {
            CSOCKET *sev_sock=NULL;
            int sev_x = 0;
            CXMLNODE* sev_node = NULL;
            char *sev_ip = NULL;
            char *sev_url = NULL;
            char *sev_a = NULL;
            char *sev_b = NULL;
            int sev_found = 0;

            sev_node = cxml_node_get(log, node, "key", "parser", NULL);
            sev_a = cxml_node_getattribute(log, sev_node, "url");
            if(NULL!=sev_a) {
              sev_url = strdup(sev_a);
              sev_a = strstr(sev_url, "//");
              if(NULL!=sev_a) {
                sev_a+=2; // go past the slash slash
                sev_b = strstr(sev_a, "/"); // find the slash past the hostname
                if(NULL!=sev_b) {
                  *sev_b = 0; // set the slash as the end of string
                  sev_b = strstr(sev_a, "@");
                  if(NULL!=sev_b) {
                    ++sev_b; // go past the @
                    if(NULL!=sev_b) { // make sure we're not sitting on the NULL
                      sev_a = sev_b; // set sev_a to the beginning of the hostname;
                    }
                  }
                }

                // sev_a is sitting at the beginning of the hostname

                // open a socket object
                sev_sock = csocket_new(log);

                // get all IPs for the box
                csocket_get_iplist(log, sev_sock);

                // get the IP for sev_a
                if(0==csocket_gethostbyname(log, sev_sock, sev_a)) {
                  if(0==csocket_set_hptosa(log, sev_sock)) {
                    sev_ip = strdup(inet_ntoa(sev_sock->sa.sin_addr));
                    if(NULL!=sev_ip) {
                      clog(log, CDEBUG, "KEY: Comparing Destination IP %s against:", sev_ip);
                      for(sev_x=0; NULL!=sev_sock->iplist[sev_x]; sev_x++) {
                        clog(log, CDEBUG, "KEY: Local IP: %s", sev_sock->iplist[sev_x]);
                        if(0==strcmp(sev_sock->iplist[sev_x], sev_ip)) {
                          // if we find that the destination is this box,
                          // switch the key type to normal unlimtied xSP
                          sev_found = 1;
                          key_type = 5;
                          clog(log, CDEBUG, "    - Match Found!");
                          break; // don't compare the next ones since we found one
                        }
                        else {
                          clog(log, CDEBUG, "    - Did not match.");
                        }
                      }
                    }
                  }
                }

                // free the socket
                csocket_free(log, sev_sock);

              } // if(did we find the hostname)
            }
            else {
              sev_found=0;
            }

            if(0==sev_found) {
              clog(log, CFATAL, " ---- KEY ERROR ---- ");
              clog(log, CFATAL, "The key tested is a single server key.");
              clog(log, CFATAL, "The satellite URL is for an IP that is");
              clog(log, CFATAL, "not on this machine. Please correct this");
              clog(log, CFATAL, "in the xSP GUI by editing the satellite");
              clog(log, CFATAL, "and updating the URL with one that is on");
              clog(log, CFATAL, "this machine.");
              clog(log, CFATAL, " ---- KEY ERROR ---- ");
              // free the key xml
              cxml_node_free(log, &_keyxml);
              _keyxml=NULL;
            }
          } // if(7 == key_type)

          // check the URL against the key domains
          if(8 == key_type) {
            char *sev_url = NULL;
            char *sev_a = NULL;
            char *sev_b = NULL;
            int sev_found = 0;
            CXMLNODE* sev_node = NULL;
            CXMLNODE *_domain = NULL;

            sev_node = cxml_node_get(log, node, "key", "parser", NULL);
            sev_a = cxml_node_getattribute(log, sev_node, "url");

            if(NULL!=sev_a) {
              sev_url = strdup(sev_a);
              sev_a = strstr(sev_url, "//");
              if(NULL!=sev_a) {
                sev_a+=2; // go past the slash slash
                sev_b = strstr(sev_a, "/"); // find the slash past the hostname
                if(NULL!=sev_b) {
                  *sev_b = 0; // set the slash as the end of string
                  sev_b = strstr(sev_a, "@");
                  if(NULL!=sev_b) {
                    ++sev_b; // go past the @
                    if(NULL!=sev_b) { // make sure we're not sitting on the NULL
                      sev_a = sev_b; // set sev_a to the beginning of the hostname;
                    }
                  }
                }

                // sev_a is sitting at the beginning of the hostname
                while(NULL!=(_domain=cxml_node_next(log, _keyxml, "domain")) ) {
                  CSTRING *domStr = _domain->data;
                  int sevLength = strlen(sev_a);

                  // [bgh] if the domain from the key is longer than or equal to the length
                  // of the hostname in the URL we can keep going
                  if(NULL!=domStr && domStr->length <= sevLength) {
                    char *domMatch = sev_a + sevLength; // [bgh] put domMatch on the null terminator
                    domMatch -= domStr->length; // [bgh] from the end of the hostname, go backwards

                    if(0==strcasecmp(domStr->string, domMatch)) {
                      // [bgh] if the domainPtr is greater than the beginning, make sure we have a '.' before the match.
                      // this will keep abc.com from matching babc.com, but def.abc.com will match abc.com
                      if(sev_a < domMatch) {
                        --domMatch;
                        if('.'==*domMatch) {
                          sev_found = 1;
                          key_type = 5;
                          break;
                        }
                      } else {
                        sev_found = 1;
                        key_type = 5;
                        break;
                      }
                    }
                  }
                }

              } // if(did we find the hostname)
            }
            else {
              sev_found=0;
            }

            if(0==sev_found) {
              clog(log, CFATAL, " ---- KEY ERROR ---- ");
              clog(log, CFATAL, "We could not match the domains in the");
              clog(log, CFATAL, "key to the URL we're going to post to.");
              clog(log, CFATAL, " ---- KEY ERROR ---- ");
              // free the key xml
              cxml_node_free(log, &_keyxml);
              _keyxml=NULL;
            }
          } // if(8 == key_type)

          if(NULL!=_keyxml) {
            CXMLNODE *_domain = NULL;

            cxml_node_iterate(log, _keyxml);

            // check for _found so that we don't send the same message to the same URL 2x
            // type of 5 means xSP key and has no domain to test
            while( 0==_found && ( 5==key_type || (NULL!=(_domain=cxml_node_next(log, _keyxml, "domain"))) ) ) {
              CXMLNODE* pnode = NULL;

              // set found = 1 if the domain matches, this keeps us from sending to the same URL again for this key
              if(5!=key_type) {
                _found = cer_check_domain(log, xml, _domain->data, key_type);
              }
              else {
                // xsp keys
                _found=1;
              }

              // get the URL for the parser to post to.
              // if 1 || 2 = found (1 for normal keys, 2 for free keys)
              if((1==_found || 2==_found) && NULL!= (pnode = cxml_node_get(log, node, "key", "parser", NULL))) {
                _tmp = cxml_node_getattribute(log, pnode, "url");
                if(NULL!=_tmp) {
                  // add the key xml to the post info
                  // This is commented out on purpose as we may
                  // want it later but do not want it now.
                  // cxml_node_addnode(log, xml, _keyxml);

                  // save the posting to url
                  cer_config->parser_url = cstring_import(_tmp, strlen(_tmp));

                  // get the user name to post
                  _tmp = cxml_node_getattribute(log, pnode, "user");
                  if(NULL!=_tmp) {
                    cer_config->parser_user = cstring_import(_tmp, strlen(_tmp));
                    _tmp = cxml_node_getattribute(log, pnode, "password");
                    if(NULL!=_tmp) {
                      cer_config->parser_pass = cstring_import(_tmp, strlen(_tmp));
                      if(0==return_code && NULL!=xml) {
                        // ##########################################################################
                        // ################=- POST INFO -=###########################################
                        // ##########################################################################
                        line = cxml_node_tostring( NULL, xml, 0);
                        if(NULL!=line) {
                          struct curl_httppost* post=NULL;
                          struct curl_httppost* last=NULL;

                          CSTRING *cs_user = cstring_import("user", 4);
                          CSTRING *cs_pass = cstring_import("password", 8);
                          CSTRING *cs_xml = cstring_import("xml", 3);                          
                          
                          ++email_matched;

													// [bgh] add the tagline from the key to the email body
													CXMLNODE* emailBody = cxml_node_get(log, xml, "email","file","tempname", NULL);
													if(NULL!=emailBody) {
														CFILE* emailBodyFile = cfile_open(&(cer_config->cfsys), emailBody->data->string, O_WRONLY| O_APPEND);
														CXMLNODE* tagLineNode = cxml_node_get(log, _keyxml, "key", "tagline", NULL);
														if(NULL!=emailBodyFile && NULL!=tagLineNode) {
															CSTRING* tagLine = tagLineNode->data;
															if(NULL!=tagLine && 0<tagLine->length) {
																int x = 0;
																// add a line break from the bottom of the email
																cfile_write(&emailBodyFile, "\n\n", 2);
																// add the '-' line
																for(x = 0; x<tagLine->length; x++) {
																	cfile_write(&emailBodyFile, "_", 1);
																}
																// add the tagline
																cfile_write(&emailBodyFile, "\n", 1);
																cfile_write(&emailBodyFile, tagLine->string, tagLine->length);
																cfile_write(&emailBodyFile, "\n", 1);
																cfile_sync(&emailBodyFile);
																cfile_close(&emailBodyFile);
															}
														}
													}

                          // turn on verbose logging for curl
                          if(CDEBUG==log->s_logfile_level) {
                            dl_curl_easy_setopt(cer_config->curl, CURLOPT_VERBOSE, 1);
                            dl_curl_easy_setopt(cer_config->curl, CURLOPT_DEBUGFUNCTION, cer_curl_error);
                            dl_curl_easy_setopt(cer_config->curl, CURLOPT_DEBUGDATA, log);
                          }

                          // use the cer_curl_write function instead
                          dl_curl_easy_setopt(cer_config->curl, CURLOPT_WRITEFUNCTION, cer_curl_write);

                          // pass a cstring obj to the 4th param of cer_curl_write()
                          curl_return = cstring_new(0);
                          dl_curl_easy_setopt(cer_config->curl, CURLOPT_FILE, curl_return);

                          if(NULL!=cer_config->curl_cainfo) {
                            clog(log, CDEBUG, "SSL: Setting CURLOPT_CAINFO to %s", cer_config->curl_cainfo->string);
                            dl_curl_easy_setopt(cer_config->curl, CURLOPT_CAINFO, cer_config->curl_cainfo->string);
                          }
                          if(NULL!=cer_config->curl_capath) {
                            clog(log, CDEBUG, "SSL: Setting CURLOPT_CAPATH to %s", cer_config->curl_capath->string);
                            dl_curl_easy_setopt(cer_config->curl, CURLOPT_CAPATH, cer_config->curl_capath->string);
                          }
                          switch(cer_config->curl_verifyhostpeer) {
                            case 0:
                              clog(log, CDEBUG, "SSL: Setting SSL Certificate verify to off");
                              dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYHOST, (long) 0);
                              dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYPEER, (long) 0);
                              break;
                            case 1:
                              clog(log, CDEBUG, "SSL: Setting SSL Certificate verify to check for a domain");
                              dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYHOST, (long) 1);
                              dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYPEER, (long) 1);
                              break;
                            case 2:
                              clog(log, CDEBUG, "SSL: Setting SSL Certificate verify to match domains");
                              dl_curl_easy_setopt(cer_config->curl, CURLOPT_SSL_VERIFYHOST, (long) 2);
                            default:
                              break;
                          }

                          // print debugging infomation
                          if(cer_config->print_xml) printf("XML:\n%s\n", line->string);

                          // add the user
                          dl_curl_formadd(&post, &last, CURLFORM_PTRNAME, cs_user->string, CURLFORM_PTRCONTENTS, cer_config->parser_user->string, CURLFORM_END);

                          // add the password
                          dl_curl_formadd(&post, &last, CURLFORM_PTRNAME, cs_pass->string, CURLFORM_PTRCONTENTS, cer_config->parser_pass->string, CURLFORM_END);

                          // post string to URL
                          dl_curl_formadd(&post, &last, CURLFORM_PTRNAME, cs_xml->string, CURLFORM_PTRCONTENTS, line->string, CURLFORM_END);

                          // dig through the xml file to get the filenames of each file and add the same file 1MB / loop
                          cer_add_sub_files(log, xml, &post, &last);

                          dl_curl_easy_setopt(cer_config->curl, CURLOPT_HTTPPOST, post);

                          dl_curl_easy_setopt(cer_config->curl, CURLOPT_URL, cer_config->parser_url->string);

                          // post to the url
                          clog(log, CDEBUG, "Posting via cURL");
                          dl_curl_easy_perform(cer_config->curl);

                          dl_curl_formfree(post);
                          
                          // free the XML we posted
                          cstring_free(&line);
                          cstring_free(&cs_user);   cs_user=NULL;
                          cstring_free(&cs_pass);   cs_pass=NULL;
                          cstring_free(&cs_xml);    cs_xml=NULL;                          

                          // print debugging infomation
                          if(cer_config->print_curl) printf("CURL:\n%s\n", curl_return->string);

                          // log any error
                          if(NULL!=curl_return) {
                            if(0!=curl_return->length) {
                              // log the error
                              clog(log, CFATAL, "PHP: %s", curl_return->string);
                            }
                            else {
                              // success! record it!
                              ++email_delivered;
                            }
                          }

                          cstring_free(&curl_return);
                          cstring_free(&cer_config->parser_user);
                          cstring_free(&cer_config->parser_pass);
                          cstring_free(&cer_config->parser_url);
                        }
                        // ##########################################################################
                        // ################=- END POST INFO -=#######################################
                        // ##########################################################################
                      } // if(0==return_code && NULL!=xml)
                    }
                    else {
                      ++key_errors;
                      clog(log, CERROR, "Password could not be found for the URL parser!");
                    }
                  }
                  else {
                    ++key_errors;
                    clog(log, CERROR, "User could not be found for the URL parser!");
                  }
                }
                else {
                  ++key_errors;
                  clog(log, CERROR, "Key matched but we could not find the parser URL!");
                }
              } // if(0<_found && NULL!= (pnode = cxml_node_get(log, node, "key", "parser", NULL)))
              else {
                if(0<_found) {
                  ++key_errors;
                  clog(log, CERROR, "Key matched but we could not find the <key><parser ...> XML!");
                }
              }
            } // while((0==_found || 1==_found) && NULL!=(_domain=cxml_node_next(log, _keyxml, "domain")))

            // free the key xml
            cxml_node_free(log, &_keyxml);
          }
          else {
            ++key_errors;
            clog(log, CERROR, "Had problem with key");
          }
        }
        else {
          ++key_errors;
          clog(log, CERROR, "<key> XML node had no key data");
        }
      } // while()
      if(0==email_delivered || 0!=key_errors) {
        clog(log, CFATAL, "Error loading/matching key!");
        return_code=EX_SOFTWARE;
      }
      if(NULL!=xml_key) cxml_root_free(log, &xml_key);
      xml_key=NULL;
    } // if(NULL==xml)

    // ##########################################################################
    // ################=- CLEAN UP -=############################################
    // ##########################################################################

CER_PARSE_FILES_CLEANUP:

    cfile_close(&file);

    if(0<email_delivered && 0==key_errors) {
      // if curl returned nothing..
      cfile_unlink(&(cer_config->cfsys), file->filename);
    }
    else if(cer_config->super_clean) {
      cfile_unlink(&(cer_config->cfsys), file->filename);
    }

    cfile_free(&file);

    // remove the temp files
//    cer_del_sub_files(log, cer_config->cfsys, xml);

    if(NULL!=xml){
      cxml_node_free(log, &xml);
    }
    xml=NULL;
    cstring_free(&dk);
  } // while(NULL!=(filename=linkedlist_next(messages)))

  return return_code;
}


