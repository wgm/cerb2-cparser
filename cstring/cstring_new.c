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

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "./cstring.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/* creates a new cstring */
CSTRING* cstring_new(int size)
{
  CSTRING* r=NULL;

  // allocate memory for the structure
  r = (CSTRING*) malloc(sizeof(CSTRING));

  // check if malloc failed
  if(NULL==r) {
    errno=ENOMEM;
    return NULL;
  }

  // memory was allocated, set it to a default value
  memset(r, 0, sizeof(CSTRING));

  // set the memory size
  if(0<size) {
    // add one to the size _just in case_ we need this extra for the terminating NULL;
    ++size;
    r->memsize = size;
  }
  else {
    r->memsize = CSTRING_BUFSIZE;
  }

  r->string = malloc(r->memsize);

  // check if malloc failed
  if(NULL==r->string) {
    free(r);
    r=NULL;
    errno=ENOMEM;
    return NULL;
  }

  r->canfree=1;

  // memory is allocated...
  // set the memory to a default value
  memset(r->string, 0, r->memsize);

  r->length = 0;

  return r;
}

#ifndef NOTEST
void Test__cstring_new(CuTest* tc)
{
  CSTRING* cptr = NULL;

  // check creation with a 0
  cptr = cstring_new(0);
  CuAssertPtrNotNull(tc, cptr);
  CuAssertStrEquals(tc, "", cptr->string);
  CuAssertIntEquals(tc, 0, cptr->length);
  CuAssertIntEquals(tc, CSTRING_BUFSIZE, cptr->memsize);
  CuAssertIntEquals(tc, 1, cptr->canfree);
  cstring_free(&cptr);

  // check creation with a higher value
  cptr = cstring_new(128);
  CuAssertPtrNotNull(tc, cptr);
  CuAssertStrEquals(tc, "", cptr->string);
  CuAssertIntEquals(tc, 0, cptr->length);
  CuAssertIntEquals(tc, 129, cptr->memsize);
  CuAssertIntEquals(tc, 1, cptr->canfree);
  cstring_free(&cptr);
}
#endif // NOTEST
