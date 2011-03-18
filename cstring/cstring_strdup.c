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

/* duplicates a cstring string */
CSTRING* cstring_strdup(CSTRING* s)
{
  CSTRING* r=NULL;

  // check if passed parameter is NULL
  if(NULL==s) return NULL;

  r = cstring_import(s->string, s->length);

  return r;
}

#ifndef NOTEST
void Test__cstring_strdup(CuTest* tc)
{
  CSTRING* a=NULL;
  CSTRING* b=NULL;
  CSTRING  z;

  // pass NULL
  a = cstring_strdup(NULL);
  CuAssertPtrEquals(tc, NULL, a);

  // pass static string
  a = cstring_import("abcde", 5);
  z = cstring_strchr(a, 'b');
  b = cstring_strdup(&z);
  CuAssertPtrNotNull(tc, b);
  CuAssertTrue(tc, 0!=b->memsize);
  CuAssertIntEquals(tc, 1, b->canfree);
  CuAssertIntEquals(tc, 4, b->length);
  CuAssertPtrNotNull(tc, b->string);
  CuAssertStrEquals(tc, "bcde", b->string);

  cstring_free(&a);
  cstring_free(&b);

  // pass alloc'd string
  a = cstring_import("abcde", 5);
  b = cstring_strdup(a);
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
