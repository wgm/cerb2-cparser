/***************************************************************************
                          cmime_parse_uu.c  -  description
                             -------------------
    begin                : Tue Apr 8 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include <string.h>

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#define  DEC(c)  (((c) - ' ') & 077)    /* single character decode */
#define IS_DEC(c) ( (((c) - ' ') >= 0) &&  (((c) - ' ') <= 077 + 1) )

// returns filename true, NULL for false
CSTRING* cmime_parse_uu_begin(CLOG_INFO* log, CSTRING* line)
{
  char *s;
  register char *t;

  // search for header line
  if(0==strncmp(line->string, "begin", 5)) {
    t=s=line->string;
    // go past 'begin'
    while(*t!=' ' && *t!=0) ++t;
    if(*t==' ') {
      ++t;
      // set S to the beginning of the MODE
      s=t;
      // go past the mode (0664, etc.)
      while(*t!=' ' && *t!=0) ++t;
      if(*t==' ') {
        int mode;
        *t=0;
        mode = atoi(s);
        *t=' ';
        if(0<mode && 777>=mode) {
          ++t;
          // set S to the beginning of the name.
          s=t;
          // set T to the end of the name
          while(*t!='\r' && *t!=0) ++t;
          // make sure t is greater than s
          if(t>s) {
            CSTRING* name;
            name = cstring_import(s, t-s);
            return name;
          }
        }
      }
    }
  }

  return NULL;
}


CXMLNODE* cmime_parse_uu(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CSTRING* filename, CXMLNODE* psub, CXMLNODE* boundary)
{
  CFILE* file=NULL;
  CXMLNODE* filenode = NULL;
  CXMLNODE* sub = NULL;
  CFILE* save=NULL;
  char *dst = NULL;
  char *decode = NULL;
  register char ch;
  CSTRING* line = NULL;
  int n=0;

  if(NULL==pfile) return NULL;
  file = *pfile;
  line = cstring_new(128);

  decode = (char*)malloc(CFILE_BUFFER_SIZE);

  if(NULL==decode) {
    perror("cmime_parse_uu()::malloc");
    return NULL;
  }

  // set up the sub and everything.
  // content-disposition -> filename

  // build the subnode header area with info from above
  sub = cxml_node_create(NULL, psub, "sub", 3);
  if(NULL!=sub) {
    CXMLNODE* tnode = NULL;

    save=cfile_mkstemp(&(file->cfsys), file_pattern->string);

    // add the header info so the subfunctions can check against it
    filenode = cxml_node_create(NULL, sub, "file", 4);

    cxml_node_addattribute(log, filenode, "name", filename->string);

    tnode = cxml_node_create(NULL, filenode, "filename", 8);
    cxml_node_adddata(NULL, tnode, filename->string, filename->length);

    tnode = cxml_node_create(NULL, filenode, "tempname", 8);
    cxml_node_adddata(NULL, tnode, save->filename, strlen(save->filename));

    tnode = cxml_node_create(NULL, sub, "content-disposition", 19);
    filenode = cxml_node_create(NULL, tnode, "filename", 8);
    cxml_node_adddata(NULL, filenode, filename->string, filename->length);
  }

  // align the ptrs
  dst = decode;

  // with multipart we need to find boundaries, let's find one.
  // look for significance (border / uudecode)
  // --[boundary]\r\n (length must be at least 5)
  // length must be at least 5 to match 'begin' also

  // when we find the boundary we need to not "JUMP SHIP" but
  // finish decoding the info in the b64 buffer.
  while( NULL!=cfile_getline_buffer(&file, "\r\n", line)) {
    register char* posa;

    if(2<line->length) {
      if(NULL!=boundary) {
        if(*(line->string) == '-') { // - == possible boundary
          if(*(line->string+1) == '-') { // beginning of boundary
            if(0==strncmp((line->string+2), boundary->data->string, boundary->data->length)) {
              break; // get out of the loop and return!
            }
          }
        }
      }
      if(*(line->string) == 'e') {
        if(*(line->string+1) == 'n') {
          if(*(line->string+2) == 'd') {
            // advance to next line
            cfile_getline_next(&file);
            break; // get out of the loop
          }
        }
      }
    }

    // make it the beginning of the string
    posa = line->string;

    // get the number of chars in the line
    if ((n = DEC(*posa)) <= 0) {
      cfile_getline_next(&file);
      continue;
    }

    // go to the second char.
    ++posa;

    // some place to come back to
    do {
      while (n > 0 && CFILE_BUFFER_SIZE>=(dst-decode)+2 ) {
        if (n >= 3) {
          if (!(IS_DEC(*posa))) {
            if(!IS_DEC(*(posa + 1))) {
              if(!IS_DEC(*(posa + 2))) {
                if(!IS_DEC(*(posa + 3))) {
                  goto CMIME_PARSE_UU_END;
                }
              }
            }
          }

          ch = DEC(*posa) << 2 | DEC(*(posa+1)) >> 4;
          *dst++=ch;
          ch = DEC(*(posa+1)) << 4 | DEC(*(posa+2)) >> 2;
          *dst++=ch;
          ch = DEC(*(posa+2)) << 6 | DEC(*(posa+3));
          *dst++=ch;

        }
        else {
          if (n >= 1) {
            if (!(IS_DEC(*posa) && IS_DEC(*(posa + 1)))) {
              goto CMIME_PARSE_UU_END;
            }
            ch = DEC(*posa) << 2 | DEC(*(posa+1)) >> 4;
            *dst++=ch;
          }
          if (n >= 2) {
            if (!(IS_DEC(*(posa + 1)) && IS_DEC(*(posa + 2)))) {
              goto CMIME_PARSE_UU_END;
            }

            ch = DEC(*(posa+1)) << 4 | DEC(*(posa+2)) >> 2;
            *dst++=ch;
          }
          if (n >= 3) {
            if (!(IS_DEC(*(posa + 2)) && IS_DEC(*(posa + 3)))) {
              goto CMIME_PARSE_UU_END;
            }
            ch = DEC(*(posa+1)) << 6 | DEC(*(posa+3));
            *dst++=ch;
          }
        }
        posa += 4;
        n -= 3;
      }

      // save and move decoding buffer
      if(CFILE_BUFFER_SIZE>=(dst-decode+1)) { // make sure it's 'higher' in memory

        // save the chars
        // do we need to start a new file?
        if(CMIMEMAXFILESIZE < save->file_pos) {
          CXMLNODE* tmpnode;
          // start a new file
          // close the old one
          cfile_close(&save);
          cfile_free(&save);

          save=cfile_mkstemp(&(file->cfsys), file_pattern->string);

          // add the header info so the subfunctions can check against it
          tmpnode = cxml_node_create(NULL, filenode, "tempname", 8);
          cxml_node_adddata(NULL, tmpnode, save->filename, strlen(save->filename));
        }

        cfile_write(&save, decode, dst-decode);

        // reset the decoding ptr
        dst = decode;
      }
    } while(0<n);

    // advance to next line
    cfile_getline_next(&file);
  }

  if(0) {
CMIME_PARSE_UU_END:
    cfile_getline_next(&file);
  }

  // write anything that hasn't been written.
  *dst=0;
  cfile_write(&save, decode, dst-decode);
  cfile_close(&save);
  cfile_free(&save);
  free(decode);
  decode=NULL;

  cstring_free(&line);

  return psub;

}


#ifndef NOTEST
void Test__cmime_parse_uu_begin(CuTest* tc)
{
}
#endif // NOTEST

#ifndef NOTEST
void Test__cmime_parse_uu(CuTest* tc)
{
}
#endif // NOTEST
