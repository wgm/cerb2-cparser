/* cString - library for managing strings in C
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "./cstring.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/* deletes a cstring */
void cstring_free(CSTRING **p)
{
  if(NULL==p || NULL==*p) {
    errno=EFAULT;
    return;
  }

  if((*p)->canfree && 0<(*p)->memsize) {
    if(NULL!=(*p)->string) {
      // cleanse the memory we've used
      memset((*p)->string, 0, (*p)->memsize);
      free((*p)->string);
    }

    // clean the memory up you've used.
    memset( (*p), 0, sizeof(CSTRING));

    free( (*p) );

    (*p)=NULL;
  }
  else {
    errno=EFAULT;
  }
}


#ifndef NOTEST
void Test__cstring_free(CuTest* tc)
{
  CSTRING* a=NULL;
  CSTRING* b=NULL;
  CSTRING  c;
  int error = 0;

  // pass a null to the 'free' function
  cstring_free(NULL);
  error = errno;
  errno = 0;
  CuAssertIntEquals(tc, EFAULT, error);

  // send non-alloc'd - passing static non-alloc address
  a = cstring_import("ABCDE", 5);
  c = cstring_strchr(a, 'C');
  b=&c;
  cstring_free(&b);
  error = errno;
  errno = 0;
  CuAssertIntEquals(tc, EFAULT, error);
  cstring_free(&a);

  // passing argument returned from cstring_import
  a = cstring_import("ABCDE", 5);
  cstring_free(&a);

  // check that the structure is null'd
  CuAssertPtrEquals(tc, NULL, a);

  // trying to double free an imported cstring
  a = cstring_import("abcde", 5);
  cstring_free(&a);
  cstring_free(&a);
  error = errno;
  errno = 0;
  CuAssertIntEquals(tc, EFAULT, error);
}
#endif // NOTEST
