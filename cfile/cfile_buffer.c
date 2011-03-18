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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int cfile_buffer(CFILE** pfile, off_t change)
{
  CFILE* file=NULL;
  int bytes_read=0;
  int changeto=0;

  if(NULL==pfile && NULL!=*pfile) return -1;

  file = *pfile;

  changeto=file->file_pos+change;

  cfile_lseek(&file, changeto, SEEK_SET);
  
  // see if we need to allocate the buffer for this file.
  if(NULL==file->buffer) {
    file->buffer = (char*) malloc(CFILE_BUFFER_SIZE);
    memset(file->buffer, 0, CFILE_BUFFER_SIZE);
  }

  bytes_read = cfile_read(&file, file->buffer, CFILE_BUFFER_SIZE);
  if(-1==bytes_read) {
    perror("cfile_buffer()::cfile_read()");
  }
  else {
    file->buffer_ptr=file->buffer;
    file->buffer_end=file->buffer+bytes_read;
  }

  return bytes_read;
}


#ifndef NOTEST
void Test__cfile_buffer(CuTest* tc)
{
}
#endif // NOTEST
