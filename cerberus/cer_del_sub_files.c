#include "cerberus.h"


// ##########################################################################
// ################=- del_sub_files() -=#####################################
// ##########################################################################

int cer_del_sub_files(CLOG_INFO* log, CFSYS* cfsys, CXMLNODE* sub) {
  int files_deld=0;
  CXMLNODE* node=NULL;
  CXMLNODE* tnode=NULL;

  // add files on this level,
  cxml_node_iterate(log, sub);
  while(NULL!=(node=cxml_node_next(log, sub, "file"))) {
    if(NULL!=(tnode = cxml_node_get(log, node, "file", "tempname", NULL))) {
      cfile_unlink(&cfsys, tnode->data->string);
    }
  }

  // loop through the subs
  cxml_node_iterate(log, sub);
  while(NULL!=(node=cxml_node_next(log, sub, "sub"))) {
    // call self with sub
    // add number of files added to return value
    files_deld+=cer_del_sub_files(log, cfsys, node);
  }

  // return number of files added
  return files_deld;
}

