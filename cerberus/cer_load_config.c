#include "cerberus.h"

int cer_load_config(CLOG_INFO* log, CXMLROOT** pxml_root, char *configfile, CERBFIG** pcer_config)
{
  int return_code=0;
  CXMLNODE* xml_node=NULL;
  CXMLNODE* node=NULL;

  CXMLROOT* xml_root = NULL;
  CERBFIG* cer_config = NULL;

  if(NULL==pxml_root || NULL==pcer_config) {
    clog(log, CFATAL, "XML: Memory allocation error!");
    return EX_SOFTWARE;
  }

  xml_root = *pxml_root;
  cer_config = *pcer_config;

  // set defaults:
  cer_config->pop3_max = 1024;
  cer_config->pop3_timeout = 30;
  cer_config->curl_verifyhostpeer=-1;
  cer_config->debug_parse=0;
  
  // old style delete functionality
  cer_config->pop3_max_delete = 1;

  clog(log, CDEBUG, "XML: Loading the XML Config file %.80s", configfile);
  if(0==cxml_read_file(log, xml_root, configfile)) {
    clog(log, CFATAL, "XML: XML Config file error, shutting down.");
    return EX_SOFTWARE;
  }
  clog(log, CDEBUG, "XML: XML Config file loaded");
  clog(log, CDEBUG, "XML: Checking for first XML Config element");
  if(NULL!=xml_root && NULL!=xml_root->data) {
    xml_node = xml_root->data;
    clog(log, CDEBUG, "XML: Found first XML Config element");
  }
  else {
    clog(log, CFATAL, "XML: First XML Config element not found");
  }

  clog(log, CDEBUG, "XML: Loading information from XML Config file");
  if(xml_node) {
    clog(log, CDEBUG, "XML: -- Checking for the 'configuration' node");
    if(0!=strcmp(xml_node->name->string, "configuration")) {
      clog(log, CFATAL, "XML: -- 'configuration' node not found");
    }
    else { // is correct has "configuration" node.
      char* _tmp;
      // create the cstring so we can concat things to it.
      if(0==return_code) {
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "xsp", NULL))) {
          cer_config->xsp = cstring_strdup(node->data);

          _tmp = cxml_node_getattribute(log, node, "https");
          if(NULL!=_tmp) {
            if(NULL!=_tmp && 0==strcmp("true", _tmp)) {
              cer_config->parser_https = 1;
            }
          }

          _tmp = cxml_node_getattribute(log, node, "user");
          if(NULL!=_tmp) {
            cer_config->parser_user = cstring_import(_tmp, strlen(_tmp));
            _tmp = cxml_node_getattribute(log, node, "password");
            if(NULL!=_tmp) {
              cer_config->parser_pass = cstring_import(_tmp, strlen(_tmp));
            }
          }
        }
      }
      if(0==return_code) {
        CSTRING *_tmp_path=cstring_new(0);
        cer_config->tmp_cerbmail=cstring_new(0);
        cer_config->tmp_cerbmime=cstring_new(0);
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "global", "tmp_dir", NULL))) {
          _tmp=cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            // copy the path into a string object
            cstring_strcat_imp(_tmp_path, _tmp, strlen(_tmp));

            // check for an ending slash and add one if need be
#ifdef WIN32
            if('\\' != _tmp_path->string[_tmp_path->length-1]) {
              cstring_strcat_imp(_tmp_path, "\\", 1);
              clog(log, CWARN, "XML: Trailing slash was not on the tmp_dir element. I added one! (%s)", _tmp_path->string);
            }
#else
            if('/' != _tmp_path->string[_tmp_path->length-1]) {
              cstring_strcat_imp(_tmp_path, "/", 1);
              clog(log, CWARN, "XML: Trailing slash was not on the tmp_dir element. I added one! (%s)", _tmp_path->string);
            }
#endif

            cstring_strcat(cer_config->tmp_cerbmail, _tmp_path);
            cstring_strcat(cer_config->tmp_cerbmime, _tmp_path);
            cstring_free(&_tmp_path);
          }
        }
        else {
          clog(log, CWARN, "tmp_dir not found in xml config file, using current directory");
        }
        cstring_strcat_imp(cer_config->tmp_cerbmail, "cerbmail_XXXXXX", 15);
        cstring_strcat_imp(cer_config->tmp_cerbmime, "cerbmime_XXXXXX", 15);
      }
      if(0==return_code) {
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "debug", "xml", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            cer_config->print_xml = atoi(_tmp);
          }
        }
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "debug", "curl", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            cer_config->print_curl = atoi(_tmp);
          }
        }
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "debug", "parse", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            cer_config->debug_parse = atoi(_tmp);
          }
        }
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "debug", "superclean", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp)   {
            cer_config->super_clean = atoi(_tmp);
          }
        }
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "ssl", "cainfo", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            if(NULL != (cer_config->curl_cainfo=cstring_new(strlen(_tmp)))) {
              cstring_strcat_imp(cer_config->curl_cainfo, _tmp, strlen(_tmp));
            }
          }
        }
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "ssl", "capath", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            if(NULL != (cer_config->curl_capath=cstring_new(strlen(_tmp)))) {
              cstring_strcat_imp(cer_config->curl_capath, _tmp, strlen(_tmp));
            }
          }
        }
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "ssl", "verify", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            cer_config->curl_verifyhostpeer = atol(_tmp);
          }
        }
        // get the max number of POP3 messages per run here
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "global", "max_pop3_messages", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            cer_config->pop3_max = atoi(_tmp);
            clog(log, CDEBUG, "XML: setting the maximum number of POP3 emails to download to %d", cer_config->pop3_max);
          }
        }
        // get the max number of POP3 messages per run here
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "global", "max_pop3_delete", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            if(NULL!=_tmp && 0==strcmp("false", _tmp)) {
              cer_config->pop3_max_delete = 0;
            }
          }
        }

        // get the pop3 timeout
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "global", "pop3_timeout", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp) {
            cer_config->pop3_timeout = atoi(_tmp);
            if(0>=cer_config->pop3_timeout) {
              cer_config->pop3_timeout=30;
            }
            clog(log, CDEBUG, "XML: setting the pop3 timeout to %d seconds", cer_config->pop3_timeout);
          }
        }
        // location of the override curl lib
        if(NULL!= (node = cxml_node_get(log, xml_node, "configuration", "global", "libcurl", NULL))) {
          _tmp = cxml_node_getattribute(log, node, "value");
          if(NULL!=_tmp && 7<strlen(_tmp)) {
            cer_config->curl_location = cstring_import(_tmp, strlen(_tmp));
            clog(log, CDEBUG, "XML: setting the libcurl location to %s", cer_config->curl_location->string);
          }
        }
        cxml_node_iterate(log, xml_node);
        while(NULL!=(node=cxml_node_next(log, xml_node, "pop3"))) {
          CXMLNODE* popnode=NULL;
          CPOP3 *_pop = NULL;
          int    popport = 110;

          // if the list isn't malloc'd yet:
          if(NULL==cer_config->poplist) {
            cer_config->poplist = linkedlist_new(0);
          }

          // get enough data to init the pop3 connect struct
          if(NULL!= (popnode = cxml_node_get(log, node, "pop3", "port", NULL))) {
            _tmp = cxml_node_getattribute(log, popnode, "value");
            if(NULL!=_tmp) {
              popport = atoi(_tmp);
            }
          }
          if(NULL!= (popnode = cxml_node_get(log, node, "pop3", "host", NULL))) {
            _tmp = cxml_node_getattribute(log, popnode, "value");
            if(NULL!=_tmp) {
              _pop = cpop3_init(log, _tmp, popport, cer_config->pop3_timeout, cer_config->cfsys);
              if(NULL!=_pop) {
                // add the additional data to the struct
                if(NULL!= (popnode = cxml_node_get(log, node, "pop3", "user", NULL))) {
                  _tmp = cxml_node_getattribute(log, popnode, "value");
                  if(NULL!=_tmp) {
                    _pop->user = strdup(_tmp);
                  }
                }
                if(NULL!= (popnode = cxml_node_get(log, node, "pop3", "password", NULL))) {
                  _tmp = cxml_node_getattribute(log, popnode, "value");
                  if(NULL!=_tmp) {
                    _pop->pass = strdup(_tmp);
                  }
                }
                if(NULL!= (popnode = cxml_node_get(log, node, "pop3", "delete", NULL))) {
                  _tmp = cxml_node_getattribute(log, popnode, "value");
                  if(NULL!=_tmp && 0==strcmp("false", _tmp)) {
                    _pop->dele = 0;
                  }
                }
                linkedlist_add(cer_config->poplist, _pop);
              } // if(NULL!=_pop)
            }
          }
        }// while(pop3)
      }// if(0==return_code)
    }
  }

  return return_code;
}
