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

void cfile_cleanup(CFSYS** cfp) {
  CFSYS* cf = NULL;
  dnode_t *dn = NULL;
  
  if(NULL==cfp || NULL==*cfp) return;

  cf = *cfp;

  // free the children
  if(NULL!=cf->files) {
    if(!dict_isempty(cf->files)) {
      for (dn = dict_first(cf->files); dn; dn = dict_first(cf->files)) {
        CFILE *data=NULL;
        data = (CFILE*)dnode_get(dn);
        cfile_close(&data);
        cfile_free(&data);
      }
    }
    dict_free_nodes(cf->files);
    dict_destroy(cf->files);
    cf->files=NULL;
  }
  
  free(cf);
  cf=NULL;
}

#ifndef NOTEST
void Test__cfile_cleanup(CuTest* tc)
{
  CFSYS *cf = NULL;
  
  cf = cfile_init(0);
  CuAssertPtrNotNull(tc,cf);
  CuAssertIntEquals(tc,0,cf->settings);
  CuAssertPtrNotNull(tc,cf->files);
  CuAssertIntEquals(tc,0,dict_count(cf->files));
  cfile_cleanup(&cf);
}
#endif // NOTEST
