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

/* duplicates a cstring string of n length */
CSTRING* cstring_strndup(CSTRING* s, int n)
{
  CSTRING* r=NULL;
  int length = 0;

  // check if passed parameter is NULL
  if(NULL==s) return NULL;

  // set the length
  length = (s->length>n?n:s->length);

  // allocate memory for the new structure
  r = (CSTRING*) malloc(sizeof(CSTRING));

  // check if malloc has failed for the new structure
  if(NULL==r) {
    errno=ENOMEM;
    return NULL;
  }

  // memory was allocated, set it to a default value
  memset(r, 0, sizeof(CSTRING));

  // set the memory size
  r->memsize = length+CSTRING_BUFSIZE;
  r->string = malloc(r->memsize);

  // check if malloc failed
  if(NULL==r->string) {
    free(r);
    r=NULL;
    errno=ENOMEM;
    return NULL;
  }

  // memory is allocated...
  // set the memory to a default value
  memset(r->string, 0, r->memsize);
  r->canfree=1;

  if(NULL!=s->string && 0<length) {
    memmove(r->string, s->string, length);
    r->length = length;
  }

  return r;
}

#ifndef NOTEST
void Test__cstring_strndup(CuTest* tc)
{
  CSTRING* a=NULL;
  CSTRING* b=NULL;
  CSTRING  z;

  // pass NULL
  a = cstring_strndup(NULL, 4);
  CuAssertPtrEquals(tc, NULL, a);

  // pass static string
  a = cstring_import("abcde", 5);
  z = cstring_strchr(a, 'b');
  b = cstring_strndup(&z, 3);
  CuAssertPtrNotNull(tc, b);
  CuAssertTrue(tc, 0!=b->memsize);
  CuAssertIntEquals(tc, 1, b->canfree);
  CuAssertIntEquals(tc, 3, b->length);
  CuAssertPtrNotNull(tc, b->string);
  CuAssertStrEquals(tc, "bcd", b->string);

  cstring_free(&a);
  cstring_free(&b);

  // pass alloc'd string request 3 chars
  a = cstring_import("abcde", 5);
  b = cstring_strndup(a, 3);
  CuAssertPtrNotNull(tc, b);
  CuAssertTrue(tc, 0!=b->memsize);
  CuAssertIntEquals(tc, 1, b->canfree);
  CuAssertIntEquals(tc, 3, b->length);
  CuAssertPtrNotNull(tc, b->string);
  CuAssertStrEquals(tc, "abc", b->string);

  cstring_free(&a);
  cstring_free(&b);

  // pass alloc'd string request 0 chars
  a = cstring_import("abcde", 5);
  b = cstring_strndup(a, 0);
  CuAssertPtrNotNull(tc, b);
  CuAssertTrue(tc, 0!=b->memsize);
  CuAssertIntEquals(tc, 1, b->canfree);
  CuAssertIntEquals(tc, 0, b->length);
  CuAssertPtrNotNull(tc, b->string);
  CuAssertStrEquals(tc, "", b->string);

  cstring_free(&a);
  cstring_free(&b);

  // pass alloc'd string length 5 ask for 10 chars
  a = cstring_import("abcde", 5);
  b = cstring_strndup(a, 10);
  CuAssertPtrNotNull(tc, b);
  CuAssertTrue(tc, 0!=b->memsize);
  CuAssertIntEquals(tc, 1, b->canfree);
  CuAssertIntEquals(tc, 5, b->length);
  CuAssertPtrNotNull(tc, b->string);
  CuAssertStrEquals(tc, "abcde", b->string);

  cstring_free(&a);
  cstring_free(&b);

}
#endif // NOTEST

