/***************************************************************************
                          cmime_parse_b64.c  -  description
                             -------------------
    begin                : Mon Mar 31 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include <string.h> 
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
 
#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif


static char b64d[] =
  {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
     52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
     -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
  };



int cmime_parse_b64_string(char *str, int len)
{
  char *posa=str;
  char *posb=str;
  char *end=NULL;
  int quad=0;
  char ch;

  if(NULL==str || 0==len) return 0;

  end=str+len;

  while(posa<end && 0!=(ch = *posa)) {
    ch = b64d[(int)ch];
    if (0 > ch) {
      ++posa;
      continue;
    }

    switch(quad % 4) {
    case 0:
      *posb = ch << 2;
      break;
    case 1:
      *posb |= (ch & 0x30) >> 4;
      ++posb;
      *posb = (ch & 0x0F) << 4;
      break;
    case 2:
      *posb |= (ch & 0x3C) >> 2;
      ++posb;
      *posb = (ch & 0x03) << 6;
      break;
    case 3:
      *posb |= (ch & 0x3F);
      ++posb;
      break;
    }
    quad++;
    ++posa;
  }

  // since zeros can result at the end of the string, backtrace until we
  // find the end of the real string and sit on that last letter
//  while(posb>str && 0==*posb) --posb;

  return (posb-str);
}



// decode base 64 and watch for boundary and UUdata
void cmime_parse_b64(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CFILE** save, CXMLNODE* sub, CXMLNODE* files, CXMLNODE* boundary, int split)
{
  register unsigned char* posa;
  CFILE* file=NULL;
  unsigned int dst = 0;
  unsigned char *decode = NULL;
  register char ch = 0;
  CSTRING* line = cstring_new(0);
  unsigned int quad=0;

  if(NULL==pfile) return;
  file = *pfile;

  decode = (char*)malloc(CFILE_BUFFER_SIZE);

  if(NULL==decode) {
    perror("cmime_parse_b64()::malloc");
  }

  // with multipart we need to find boundaries, let's find one.
  // look for significance (border / uudecode)
  // --[boundary]\r\n (length must be at least 5)
  // length must be at least 5 to match 'begin' also

  // when we find the boundary we need to not "JUMP SHIP" but
  // finish decoding the info in the b64 buffer.
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

    // if the string has \r\n, replace \r with 0
    posa = line->string;
    posa += line->length-1;
    while( (char*)posa >= (char*)line->string && ( ('\n'==*posa) || ('\r'==*posa) ) ) {
      *posa=0;
      --posa;
    }

    // make it the beginning of the string
    posa = line->string;

    do {

      // run through the whole string, converting as we go
      // :dst-decode+1: the +1 is so that we do not overrun the buffer
      // because there are multiple ++dst below
      while (CFILE_BUFFER_SIZE-1>=dst && 0 != (ch = *posa)) {
        if (ch == '=') {
          *posa=0; // so that we get out of the do-while
          break;
        }

        ch = b64d[(int)ch];
        if (0 > ch) {
          ++posa;
          continue;
        }

        switch(quad % 4) {
        case 0:
          decode[dst] = ch << 2;
          break;
        case 1:
          decode[dst] |= (ch & 0x30) >> 4;
          ++dst;
          decode[dst] = (ch & 0x0F) << 4;
          break;
        case 2:
          decode[dst] |= (ch & 0x3C) >> 2;
          ++dst;
          decode[dst] = (ch & 0x03) << 6;
          break;
        case 3:
          decode[dst] |= (ch & 0x3F);
          ++dst;
          break;
        }
        quad++;
        ++posa;
      } // while()

      // save and move decoding buffer
      if(0<dst) { // make sure there is data to write
        int safechars;
        // figure out how many characters are finished decoding.
        safechars = dst-1; // keep out an extra char

        // save the chars
        // do we need to start a new file?
        if(split && CMIMEMAXFILESIZE < (*save)->file_pos+safechars) {
          CXMLNODE* tmpnode;
          // start a new file
          // close the old one
          cfile_close(save);
          cfile_free(save);

          *save=cfile_mkstemp(&(file->cfsys), file_pattern->string);

          tmpnode = cxml_node_create(NULL, files, "tempname", 8);
          cxml_node_adddata(NULL, tmpnode, (*save)->filename, strlen((*save)->filename));
        }

        cfile_write(save, decode, safechars);

        // slide the saved memory off the buffer. :-)
        memmove(decode, decode+safechars, 1);

        // reposition the destination
        dst = 1;
      }

    // if we were not done with the CSTRING line, go back and finish
    } while (*posa!=0);

    // advance to next line
    cfile_getline_next(&file);
  }

  decode[dst]=0;
  cfile_write(save, decode, dst);
  free(decode);
  decode=NULL;

  if(NULL!=line) cstring_free(&line);
  return;
}


#ifndef NOTEST
void Test__cmime_parse_b64_string(CuTest* tc)
{
}
#endif // NOTEST

#ifndef NOTEST
void Test__cmime_parse_b64(CuTest* tc)
{
}
#endif // NOTEST

