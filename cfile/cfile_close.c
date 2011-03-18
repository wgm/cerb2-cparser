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

#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int cfile_close(CFILE** pfile) {
  CFILE* file = NULL;
  int retval=-1;
  if(NULL==pfile) return -1;
  
  file = *pfile;
  
  if(NULL==file->cfsys || !(CFSYS_INMEMORY & file->cfsys->settings)) {  
    retval = close(file->fileno);
  }
  else {
    // reset the info for the opening
    file->file_pos=0;
    file->line_pos=0;
    file->line_used=0;
    file->buffer_ptr=file->buffer;
  }
    
  return retval;
}

#ifndef NOTEST
void Test__cfile_close(CuTest* tc)
{
  CFILE *file = NULL;
  CFSYS *cf = NULL;
  
  cf = cfile_init(CFSYS_INMEMORY);
  
  file = cfile_open(&cf, "/A.txt", O_RDWR|O_CREAT);
  CuAssertPtrNotNull(tc, file);
  CuAssertStrEquals(tc, "/A.txt", file->filename);
  CuAssertIntEquals(tc, 4, file->fileno);
  CuAssertIntEquals(tc, 0, file->file_pos);
  CuAssertIntEquals(tc, 0, file->line_pos);
  CuAssertIntEquals(tc, 0, file->line_used);
  CuAssertPtrEquals(tc, file->buffer, file->buffer_ptr);
  CuAssertPtrEquals(tc, NULL, file->buffer_ptr);
  
  file->file_pos=1;
  file->line_pos=1;
  file->line_used=1;
  file->buffer_ptr="abcde";
  
  cfile_close(&file);

  CuAssertIntEquals(tc, 0, file->file_pos);
  CuAssertIntEquals(tc, 0, file->line_pos);
  CuAssertIntEquals(tc, 0, file->line_used);
  CuAssertPtrEquals(tc, file->buffer, file->buffer_ptr);
  CuAssertPtrEquals(tc, NULL, file->buffer_ptr);
    
  cfile_free(&file);
  cfile_cleanup(&cf);
}
#endif // NOTEST
