/***************************************************************************
                          cmime_parse_qptext.c  -  description
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

static char hc[256]={
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
  0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   10,   11,   12,   13,   14,   15,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   10,   11,   12,   13,   14,   15,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,\
 -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0 \
};

void cmime_parse_qptext_line(CSTRING* line)
{
  register char *posa=NULL;
  register char *posb=NULL;
  if(NULL==line || NULL==line->string) return;

  posa=posb=line->string;

  while(line->length>(posa-line->string)) {
    if(*posa=='=') { // found a 'quoted' text bit
      // there is always 2 chars following the = because the line is terminated with \r\n
      if(*(posa+1)=='\r' && *(posa+2)=='\n') {
        // escaped line break, remove it.
        posa+=3;
      }
      else if(-1 == hc[(int)*(posa+1)]) { // we have an illegal quoting
        *posb=*posa;
        ++posb;
        ++posa;
      }
      else {
        *posb=( ( hc[ (int)*(posa+1) ] *16 ) + hc[ (int)*(posa+2) ] );
        ++posb;
        posa+=3;
      }
    }
    else {
      *posb=*posa;
      ++posb;
      ++posa;
    }
  }

  *posb=0; // cap the line

  // change the length of the cstring
  line->length=(posb-line->string);
}

// decode qp text and watch for boundary and UUdata
void cmime_parse_qptext(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CFILE** save, CXMLNODE* sub, CXMLNODE* files, CXMLNODE* boundary)
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

    cmime_parse_qptext_line(line);

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
}

#ifndef NOTEST
void Test__cmime_parse_qptext_line(CuTest* tc)
{
}
#endif // NOTEST

#ifndef NOTEST
void Test__cmime_parse_qptext(CuTest* tc)
{
}
#endif // NOTEST

