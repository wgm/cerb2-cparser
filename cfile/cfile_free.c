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

#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

void cfile_free(CFILE** pfile) {
  CFILE* file = NULL;
  if(NULL==pfile || NULL==*pfile) return;
  
  file = *pfile;

  if(NULL!=file->me) {
    // if there is a parent, remove from parent before deleting from memory
    if(NULL!=file->cfsys && NULL!=file->cfsys->files) {
  
      // make sure the filesys contains the child!
      if(1==dict_contains(file->cfsys->files, file->me)) {
        // delete the file from the filesys
        dict_delete(file->cfsys->files, file->me);
      }
    }
  
    // free the filename (key)
    if(NULL!=dnode_getkey(file->me)) {
      char * key = NULL;
      key = (char *)dnode_getkey(file->me);
      free(key);
    }
  
    // free the dict node
    dnode_destroy(file->me);  
  }
  
  if(NULL!=file->filename) free(file->filename);
  file->filename = NULL;

  if(NULL!=file->buffer) free(file->buffer);
  file->buffer = NULL;
  file->buffer_end = NULL;
  file->buffer_ptr = NULL;

  free(file);
  file=NULL;
}

#ifndef NOTEST
void Test__cfile_free(CuTest* tc)
{
}
#endif // NOTEST
