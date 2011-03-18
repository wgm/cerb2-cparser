/***************************************************************************
                          cmime_parse_text.c  -  description
                             -------------------
    begin                : Mon Mar 31 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include <string.h>

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// decode qp text and watch for boundary and UUdata
void cmime_parse_text(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CFILE** save, CXMLNODE* sub, CXMLNODE* files, CXMLNODE* boundary)
{
  CSTRING* line=NULL;
  CFILE* file=NULL;
  
  if(NULL==pfile) return;
  file = *pfile;

  line = cstring_new(0);

  // with multipart we need to find boundaries, let's find one.
  // look for significance (border / uudecode)
  // --[boundary]\r\n (length must be at least 5)
  // length must be at least 5 to match 'begin' also
  while( NULL!=cfile_getline_buffer(&file, "\r\n", line)) {
    if(4<line->length) {
      if(NULL!=boundary) {
        if(*(line->string) == '-') { // - == possible boundary
          if(*(line->string+1) == '-') { // beginning of boundary
            if(0==strncmp((line->string+2), boundary->data->string, boundary->data->length)) {
              break; // get out of the loop and return!
            }
          }
        }
      }
      else if(*(line->string) == 'b') { // b == possible "begin" for uuencoded
        CSTRING* ctmp = cmime_parse_uu_begin( log, line);
        if(NULL!=ctmp) {
          cfile_getline_next(&file);
          cmime_parse_uu( log, &file, file_pattern, ctmp, sub, boundary);
          cstring_free(&ctmp);
          continue;
        }
      }
    }

    // do we need to start a new file?
    if(CMIMEMAXFILESIZE < (*save)->file_pos+line->length) {
      CXMLNODE* tmpnode;
      // start a new file
      // close the old one
      cfile_close(save);
      cfile_free(save);

      *save=cfile_mkstemp(&(file->cfsys), file_pattern->string);

      // add the header info so the subfunctions can check against it
      tmpnode = cxml_node_create(NULL, files, "tempname", 8);
      cxml_node_adddata(NULL, tmpnode, (*save)->filename, strlen((*save)->filename));
    }
    
    cfile_write(save, line->string, line->length);

    // advance to next line
    cfile_getline_next(&file);
  }

  cstring_free(&line);
  return;
}

#ifndef NOTEST
void Test__cmime_parse_text(CuTest* tc)
{
}
#endif // NOTEST


