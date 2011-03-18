/***************************************************************************
                          cmime_file_parse.c  -  description
                             -------------------
    begin                : Fri Mar 28 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include "cmime.h"
 
#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif
 
CXMLNODE* cmime_file_parse(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern)
{
  CFILE* file = NULL;
  CXMLNODE *sub = NULL;
  CXMLNODE *header = NULL;
  CXMLNODE *part = NULL;

  if(NULL==pfile) return NULL;
  file = *pfile;

  header = cmime_header_parse( log, &file, file_pattern );
  if(NULL!=header) {
    sub = cxml_node_create(NULL, NULL, "email", 5);
    if(NULL!=header) cxml_node_addnode(NULL, sub, header);
    part   = cmime_body_parse( log, &file, file_pattern, sub, "email");
  }

  return sub;
}

#ifndef NOTEST
void Test__cmime_file_parse(CuTest* tc)
{
}
#endif // NOTEST
