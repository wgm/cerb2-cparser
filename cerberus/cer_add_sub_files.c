#include "cerberus.h"

// ##########################################################################
// ################=- add_sub_files() -=#####################################
// ##########################################################################

int cer_add_sub_files(CLOG_INFO* log, CXMLNODE* sub, struct curl_httppost ** post, struct curl_httppost ** last) {
  int files_added=0;
  CXMLNODE* node=NULL;
  CXMLNODE* tnode=NULL;

  // add files on this level,
  cxml_node_iterate(log, sub);
  while(NULL!=(node=cxml_node_next(log, sub, "file"))) {
    cxml_node_iterate(log, node);
    while(NULL!=(tnode = cxml_node_next(log, node, "tempname"))) {
      dl_curl_formadd(post, last, CURLFORM_PTRNAME, tnode->data->string, CURLFORM_FILE, tnode->data->string, CURLFORM_END);
    }
  }

  // loop through the subs
  cxml_node_iterate(log, sub);
  while(NULL!=(node=cxml_node_next(log, sub, "sub"))) {
    // call self with sub
    // add number of files added to return value
    files_added+=cer_add_sub_files(log, node, post, last);
  }

  // return number of files added
  return files_added;
}

