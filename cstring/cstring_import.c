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

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "./cstring.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/* imports a char* into a new cstring */
CSTRING* cstring_import(char *s, int n)
{
  CSTRING* r=NULL;

  if(NULL==s) {
    return NULL;
  }

  r = cstring_new(n);

  // copy the data into the string
  if(NULL!=r && NULL!=s && 0<n) {
    memmove(r->string, s, n);
    r->length = n;
  }

  return r;
}

#ifndef NOTEST
void Test__cstring_import(CuTest* tc)
{
  CSTRING* a=NULL;
  char *str=NULL;

  // passing NULL, 0
  a = cstring_import(NULL, 0);
  CuAssertPtrEquals(tc, NULL, a);

  // passing static string 'abcde', length 5
  a = cstring_import("abcde", 5);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 5, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "abcde", a->string);
  cstring_free(&a);

  // passing strdup'd string 'abc...xyz', strlen(strdup'd string)
  str = strdup("abcdefghijklmnopqrstuvwxyz");
  a = cstring_import(str, strlen(str));
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 26, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, str, a->string);
  free(str);
  cstring_free(&a);

  // passing strdup'd string 'abc...xyz', length 5
  str = strdup("abcdefghijklmnopqrstuvwxyz");
  a = cstring_import(str, 5);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 5, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "abcde", a->string);
  free(str);
  cstring_free(&a);
}
#endif // NOTEST
