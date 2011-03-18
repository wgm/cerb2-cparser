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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CFILE* cfile_open_create(CFSYS** cfp, const char *pathname, int flags, mode_t mode)
{
  CFILE* file = NULL;
  int    fno  = -1;
  CFSYS* cf=NULL;
  
  if(NULL==cfp || NULL==*cfp || NULL==pathname) return NULL;
  
  cf = *cfp;
  
  if(!(CFSYS_INMEMORY & cf->settings)) {
    // use the standard file system
    fno = open(pathname, flags | O_BINARY, mode);
  
    if(-1!=fno) {
      file = cfile_new();
      file->filename = strdup(pathname);
      file->fileno = fno;
    }
    else {
      // we couldn't open the file.. 
      // if the O_CREAT flag was given, switch to memory mode
      // and make the file there in the code below
      if(O_CREAT & flags && CFSYS_FALLBACK & cf->settings) {
        cf->settings |= CFSYS_INMEMORY;
      }
    }
  }
  
  // if above didn't get called (or if it fallback'd) call this code
  if(CFSYS_INMEMORY & cf->settings) {
      // check to see if this file is already in the mfs
    dnode_t *dn = NULL;
    
    dn = dict_lookup(cf->files, pathname);
    if(NULL==dn) {
      if(O_CREAT & flags) {
        // file doesn't exist - and the create flag was given, make it
        file = cfile_new();
        file->filename = strdup(pathname);
        file->fileno = cf->filenumber++;
      }
    }
    else {
      // file exists - retrieve it
      file = dnode_get( dn );
    }
  }

  // save the file in the file system
  if(NULL!=file && NULL!=file->filename && NULL==dict_lookup(cf->files, pathname)) {
    dnode_t *node = (cf->files)->dict_allocnode((cf->files)->dict_context);
    char *key = strdup(file->filename);

    if (node) {
      dnode_init(node, file);
      dict_insert(cf->files, node, key);

      // let the child know who they are
      file->me = node;
    }
  }
    
  if(NULL!=file) {
    file->cfsys = cf;
  }
  
  return file;
}

#ifndef NOTEST
void Test__cfile_open_create(CuTest* tc)
{
  CFILE *fileA = NULL;
  CFILE *fileB = NULL;
  CFILE *fileC = NULL;
  CFSYS *cf = NULL;
// CFILE* cfile_open(CFSYS** cfp, const char *pathname, int flags)

  cf = cfile_init(CFSYS_INMEMORY);
  
  fileA = cfile_open(&cf, "/A.txt", O_RDWR);
  CuAssertPtrEquals(tc, NULL, fileA);
  
  fileA = cfile_open(&cf, "/A.txt", O_RDWR|O_CREAT);
  CuAssertPtrNotNull(tc, fileA);
  CuAssertStrEquals(tc, "/A.txt", fileA->filename);
  CuAssertIntEquals(tc, 4, fileA->fileno);
  CuAssertPtrEquals(tc, cf, fileA->cfsys);
  
  fileB = cfile_open(&cf, "/B.txt", O_RDWR|O_CREAT);
  CuAssertPtrNotNull(tc, fileB);
  CuAssertStrEquals(tc, "/B.txt", fileB->filename);
  CuAssertIntEquals(tc, 5, fileB->fileno);
  CuAssertPtrEquals(tc, cf, fileB->cfsys);
  
  fileC = cfile_open(&cf, "/A.txt", O_RDWR);
  CuAssertPtrNotNull(tc, fileC);
  CuAssertStrEquals(tc, "/A.txt", fileC->filename);
  CuAssertIntEquals(tc, 4, fileC->fileno);
  CuAssertPtrEquals(tc, fileA, fileC);
  CuAssertPtrEquals(tc, cf, fileC->cfsys);

  cfile_close(&fileC);
  
  fileC = cfile_open(&cf, "/B.txt", O_RDWR);
  CuAssertPtrNotNull(tc, fileC);
  CuAssertStrEquals(tc, "/B.txt", fileC->filename);
  CuAssertIntEquals(tc, 5, fileC->fileno);
  CuAssertPtrEquals(tc, fileB, fileC);
  CuAssertPtrEquals(tc, cf, fileC->cfsys);
  
  cfile_close(&fileC);
  
  cfile_cleanup(&cf);
  
  // TODO need to test fallback, etc
}
#endif // NOTEST
