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
#include <stdlib.h>
#include <stdio.h>

#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CFILE * cfile_mkstemp(CFSYS** cfp, char *path) {
  CFSYS* cf=NULL;
  CFILE* file=NULL;
  
  if(NULL==cfp || NULL==*cfp) return NULL;
  
  cf = *cfp;

  // get the malloc'd var
  file = cfile_new();

  // check that the parameter is not NULL
  if(NULL==path) return NULL;

#if !defined(WIN32)
  // strdup the path because mkstemp will clobber it.
  file->filename = strdup(path);

  if(NULL!=file->filename) {
    // make the temp filename and open the file
    file->fileno = mkstemp(file->filename);

    // did something bad happen?
    if(-1==file->fileno) {
      free(file->filename);
      file->filename=NULL;
      cfile_free(&file);
      return NULL;
    }
  }
#else
  // windows must use _tmpnam(dir, prefix) instead
  // as the mktemp on windows can only make 27(26?) unique tmp files
  if(NULL==strstr(path, "\\")) {
    // there was no directory, etc, just try the tmpnam() function
    char * tn = NULL;
    tn = tmpnam(NULL);
    if(NULL==tn) {
      cfile_free(&file);
      return NULL;
    }
    else {
      file->filename=strdup(tn);
    }
  }
  else {
    // there was a directory, split the prefix from the filename
    char * tn = NULL;
    char * dir = NULL;
    char * prefix = NULL;
    dir = strdup(path); // copy the string so we can mangle it

    // remove the XXXXXX for windows since they don't replace it
    prefix = strstr(dir, "XXXXXX");
    *prefix = 0;

    // set the prefix to the beginning of the dir so we can find the file name
    prefix = dir;
    while(NULL!=(tn=strstr(prefix+1, "\\"))) {
        prefix = tn;
    }
    *prefix=0;
    ++prefix;
    tn = NULL;
    tn = _tempnam(dir, prefix);
    free(dir);
    dir=NULL;
    prefix=NULL;
    if(NULL!=tn) {
      file->filename = tn;
    }
  }

  // open the temp file for reading and writting with mode 0600
  file->fileno = open(file->filename, O_CREAT | O_EXCL | O_RDWR | O_BINARY, S_IRUSR | S_IWUSR);

  // did something bad happen
  if(-1==file->fileno) {
    free(file->filename);
    file->filename=NULL;
    return NULL;
  }
#endif

  // if the previous stuff worked, add the name/value pair
  if(NULL!=file && NULL!=file->filename) {
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
void Test__cfile_mkstemp(CuTest* tc)
{
}
#endif // NOTEST
