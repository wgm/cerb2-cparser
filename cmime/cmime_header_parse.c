/***************************************************************************
                          cmime_header_parse.c  -  description
                             -------------------
    begin                : Fri Mar 28 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/


#include <string.h>

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#ifndef isspace
#define isspace(c) ((' '==c || '\f'==c || '\n'==c || '\r'==c || '\t'==c || '\v'==c) ? 1 : 0 )
#endif

void cmime_header_parse_ct(CLOG_INFO* log, CSTRING* dupe, CXMLNODE* node, char* tagname) {
  CXMLNODE* snode;
  char *posa;
  char *posb;

  posa = (char*)strchr(dupe->string, ';');
  if(NULL!=posa) { // we should look for boundary and charset
    posa=(char*)strstr(dupe->string, tagname);
    if(NULL!=posa) {
      // get the boundary
      // find the equal sign
      posa = (char*)strchr(posa, '=');
      if(NULL!=posa) {
        ++posa;

        // skip whitespace after the equal sign
        while(isspace(*posa)) ++posa;

        if(0!=*posa) {
          int isquoted=0;
          if(*posa=='"') {
            isquoted=1;
            ++posa;
          }

          // find the end of the string, put posb there
          posb=posa;
          if(isquoted) {
            while('"'!=*posb && 0!=*posb) ++posb;
          }
          else {
            while(!isspace(*posb) && '"'!=*posb && 0!=*posb && ';'!=*posb) ++posb;
          }

          snode = cxml_node_create(NULL, node, tagname, strlen(tagname));
          if(NULL!=snode) {
            cxml_node_adddata(NULL, snode, node->data->string+(posa-dupe->string), posb-posa);
          }
        }
      }
    }
  }
}

CXMLNODE* cmime_header_parse(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern)
{
  CFILE* file = NULL;
  CXMLNODE *root = NULL;
  CXMLNODE *node = NULL;
  CSTRING* line = NULL;

  if(NULL==pfile) return NULL;
  file = *pfile;

  line = cstring_new(0);
  root = cxml_node_create( NULL , NULL, "headers", 7);

  // read headers
  while( NULL!=cfile_getline_buffer(&file, "\r\n", line)) {
    cfile_getline_next(&file);
    node=cmime_parse_822( log, root, line, node );
    if(NULL==node) {
      break;
    }
  }

  cstring_free(&line);

  // content-type code
  node = cxml_node_get(NULL, root, "headers", "content-type", NULL);
  if(NULL!=node && NULL!=node->data && NULL!=node->data->string) {
    CSTRING* dupe=NULL;

    // make a copy to do the searching with
    dupe = cstring_strdup(node->data);
    cstring_strlower(dupe);

    // remember the content type for 'switch/case' later.
    if(strstr(dupe->string, "multipart/"))          cxml_node_addattribute(NULL, node, "case", "a");
    else if(strstr(dupe->string, "text/plain"))     cxml_node_addattribute(NULL, node, "case", "b");
    else if(strstr(dupe->string, "text/html"))      cxml_node_addattribute(NULL, node, "case", "c");
    else if(strstr(dupe->string, "text/"))          cxml_node_addattribute(NULL, node, "case", "d");
    else if(strstr(dupe->string, "message/rfc822")) cxml_node_addattribute(NULL, node, "case", "e");
    else if(strstr(dupe->string, "/octet-stream"))  cxml_node_addattribute(NULL, node, "case", "f");
    else if(strstr(dupe->string, "/ms-tnef"))       cxml_node_addattribute(NULL, node, "case", "g");
    else                                            cxml_node_addattribute(NULL, node, "case", "z");

    cmime_header_parse_ct( log, dupe, node, "boundary");
    cmime_header_parse_ct( log, dupe, node, "charset");
    cmime_header_parse_ct( log, dupe, node, "name");

    cstring_free(&dupe);
  }
  else {
    // we found the boundary but it did not have a content type, so we add the 'unknown' one.
    node = cxml_node_create( NULL, root, "content-type", 12);
    cxml_node_adddata(NULL, node, "unknown", 7);
    cxml_node_addattribute(NULL, node, "case", "z");
  }

  // content-dispostion code..
  node = cxml_node_get(NULL, root, "headers", "content-disposition", NULL);
  if(NULL!=node && NULL!=node->data && NULL!=node->data->string) {
    CSTRING* dupe=NULL;

    // make a copy to do the searching with
    dupe = cstring_strdup(node->data);
    cstring_strlower(dupe);

    cmime_header_parse_ct( log, dupe, node, "filename");
    cmime_header_parse_ct( log, dupe, node, "size");

    cstring_free(&dupe);
  }

  // content-transfer-encoding
  node = cxml_node_get(NULL, root, "headers", "content-transfer-encoding", NULL);
  if(NULL!=node && NULL!=node->data && NULL!=node->data->string) {
    CSTRING* dupe=NULL;

    // make a copy to do the searching with
    dupe = cstring_strdup(node->data);
    cstring_strlower(dupe);

    if(strstr(dupe->string, "base64"))                cxml_node_addattribute(NULL, node, "case", "a");
    else if(strstr(dupe->string, "7bit"))             cxml_node_addattribute(NULL, node, "case", "b");
    else if(strstr(dupe->string, "8bit"))             cxml_node_addattribute(NULL, node, "case", "c");
    else if(strstr(dupe->string, "quoted-printable")) cxml_node_addattribute(NULL, node, "case", "d");
    else if(strstr(dupe->string, "uuencode"))         cxml_node_addattribute(NULL, node, "case", "e");
    else                                              cxml_node_addattribute(NULL, node, "case", "z");

    cstring_free(&dupe);
  }

  // content-disposition
  node = cxml_node_get(NULL, root, "headers", "content-disposition", NULL);
  if(NULL!=node && NULL!=node->data && NULL!=node->data->string) {
    CSTRING* dupe=NULL;

    // make a copy to do the searching with
    dupe = cstring_strdup(node->data);
    cstring_strlower(dupe);

    if(strstr(dupe->string, "inline"))          cxml_node_addattribute(NULL, node, "inline", "true");
    else if(strstr(dupe->string, "attachment")) cxml_node_addattribute(NULL, node, "attachment", "true");
    else if(strstr(dupe->string, "form-data"))  cxml_node_addattribute(NULL, node, "form-data", "true");

    cstring_free(&dupe);
  }

  return root;
}

#ifndef NOTEST
void Test__cmime_header_parse_ct(CuTest* tc)
{
}
#endif // NOTEST

#ifndef NOTEST
void Test__cmime_header_parse(CuTest* tc)
{
}
#endif // NOTEST


