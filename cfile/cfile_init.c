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
#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CFSYS* cfile_init(int flags) {
  CFSYS *cf;
  cf = (CFSYS*) malloc(sizeof(CFSYS));
  
  if(NULL!=cf) {
    // set it to all zero
    memset(cf, 0, sizeof(CFSYS));
  
    // save hte flags
    cf->settings = flags;
  
    // set the initial available file number
    cf->filenumber = 4;
    
    // if the filesystem is blank - make a new one
    // this is used for tracking files
    if(NULL==cf->files) {
      cf->files = dict_create(DICTCOUNT_T_MAX, (dict_comp_t)strcmp);
    }
  }
  
  return cf;
}

#ifndef NOTEST
void Test__cfile_init(CuTest* tc)
{
  CFSYS* cf = NULL;
  
  cf = cfile_init(0);
  CuAssertPtrNotNull(tc,cf);
  CuAssertIntEquals(tc,0,cf->settings);
  CuAssertPtrNotNull(tc,cf->files);
  CuAssertIntEquals(tc,0,dict_count(cf->files));
  cfile_cleanup(&cf);
  
  cf = cfile_init(CFSYS_INMEMORY);
  CuAssertPtrNotNull(tc,cf);
  CuAssertIntEquals(tc,CFSYS_INMEMORY,cf->settings);
  cfile_cleanup(&cf);
}
#endif // NOTEST
