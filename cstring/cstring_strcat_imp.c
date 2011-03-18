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

/* concatinates s2 onto s1 */
CSTRING* cstring_strcat_imp(CSTRING* s1, char *s2, int n)
{
  int newsize=0;
  char *dst=NULL;

  // if the destination is NULL, return NULL
  if(NULL==s1) return NULL;

  // if the string to concat is NULL, return the destination
  if(NULL==s2) return s1;

  // see if we can append stuff to s1
  if(1!=s1->canfree) return NULL;

  if(0==n) return s1;

  // check to see if we need to increase the memory size
  newsize = s1->length + n + 1;
  if(0>=(s1->memsize - newsize)) {
    void *tmp;

    // save time and get 2 times as much as we need
    newsize*=2;
    
    // allocate the memory
    tmp = realloc(s1->string, newsize);

    if(NULL!=tmp) {
      s1->string=tmp;
      s1->memsize=newsize;
    }
    else {
      return s1;
    }
  }

  dst = s1->string+s1->length;
  memset(dst, 0, newsize-s1->length);
  // concatinate the strings
  memmove(dst, s2, n);

  s1->length = s1->length+n;

  // return the destination string
  return s1;
}

#ifndef NOTEST
void Test__cstring_strcat_imp(CuTest* tc)
{
  CSTRING* a = NULL;

  // pass all nulls
  a = cstring_strcat_imp(NULL, NULL, 0);
  CuAssertPtrEquals(tc, NULL, a);

  // pass a null first param
  a = cstring_strcat_imp(NULL, "abcde", 5);
  CuAssertPtrEquals(tc, NULL, a);

  // pass a NULL second parameter
  a = cstring_new(0);
  a = cstring_strcat_imp(a, NULL, 0);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 0, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "", a->string);
  cstring_free(&a);

  // pass good both, second zero length
  a = cstring_new(0);
  a = cstring_strcat_imp(a, "", 0);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 0, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "", a->string);
  cstring_free(&a);

  // pass good both, "abcde" for second and length zero
  a = cstring_new(0);
  a = cstring_strcat_imp(a, "abcde", 0);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 0, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "", a->string);
  cstring_free(&a);

  // pass good both, "abcde" for second and length 3
  a = cstring_new(0);
  a = cstring_strcat_imp(a, "abcde", 3);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 3, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "abc", a->string);
  cstring_free(&a);

  // pass good both, "abcde" for second and length 5
  a = cstring_new(0);
  a = cstring_strcat_imp(a, "abcde", 5);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 5, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "abcde", a->string);
  cstring_free(&a);
}
#endif // NOTEST
