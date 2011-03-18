/* cFile - file I/O library with line buffering
 * Copyright (c) 2003 WebGroup Media, LLC <support@webgroupmedia.com>
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
 
#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CSTRING* cfile_getline_buffer(CFILE** pfile, char *term, CSTRING* line) {
  CFILE* file = NULL;
  int   eof=0;
  char *pos=NULL;
  int   tlen=0;
  int   tpos=0;

  if(NULL==term || NULL==pfile) return NULL;

  file = *pfile;

  // prepare the CSTRING buffer
  line->length=0; // fill from beginning

  // check to see if we need to rewind
  // this will also get called when the
  // buffer has not been init'd because
  // the line_used var has been init'd
  // to zero
  if(!file->line_used) {
    cfile_buffer(&file, file->line_pos-file->file_pos);
    if(NULL==file->buffer) {
      return NULL;
    }
  }

  // set it to 0 so that people have to use the cfile_getline_next() function
  // after calling this one.
  file->line_used=0;
  
  // beginning of line: current position = read position - position from end of buffer
  // this should be the offset in the file of the beginning of the line
  file->line_pos=file->file_pos-(file->buffer_end-file->buffer_ptr);

  // length of the terminator
  tlen = strlen(term);

  pos=file->buffer_ptr;
  // find the terminator we are looking for
  for(;;) {
    if(pos+tlen < file->buffer_end) { // if we have room to match the term, keep looking.
      // find a first character match
      if(*pos==*term) {
        // loop and see if the rest of the term matches
        for(tpos=1; tpos < tlen; ++tpos) {
          // check the chars
          if(*(pos+tpos)!=*(term+tpos)) {
            break;
          }
        }
        if(tpos==tlen) { // if it matched all the characters
          pos+=(tlen); // add the length of the matched string to the pointer
          break;
        }
      }
      ++pos;
    }
    // there is no more left to buffer, and there is not enough room to compare properly
    else if(1==eof) {
      pos = file->buffer_end;
      break;
    }
    else { // not enough room to match the pattern. We need to save and rebuffer
      // save
      cstring_strcat_imp(line, file->buffer_ptr, (pos-file->buffer_ptr));
      // rebuffer
      eof = cfile_buffer(&file, (pos - file->buffer_end));
      // if it is the end of file, we need to remember so
      if(CFILE_BUFFER_SIZE>eof) {
        eof=1;
      }
      // reset the local pointers
      pos = file->buffer_ptr;
    }
  }

  // save the end of the line
  cstring_strcat_imp(line, file->buffer_ptr, pos-file->buffer_ptr);

  // set the ptr for the next time we start the loop
  file->buffer_ptr=pos;
  
  // do not return an empty string, return NULL instead.
  if(0==line->length) {
    return NULL;
  }

  return line;
}

#ifndef NOTEST
void Test__cfile_getline_buffer(CuTest* tc)
{
}
#endif // NOTEST
