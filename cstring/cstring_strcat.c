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
CSTRING* cstring_strcat(CSTRING* s1, CSTRING* s2)
{
  int newsize=0;
  char *dst=NULL;

  // if the destination is NULL, return NULL
  if(NULL==s1) return NULL;

  // if the string to concat is NULL, return the destination
  if(NULL==s2) return s1;

  // see if we can append stuff to s1
  if(1!=s1->canfree) return NULL;

  if(0==s2->length) return s1;

  // check to see if we need to increase the memory size
  newsize = s1->length + s2->length + 1;
  if(0>=(s1->memsize - newsize)) {
    void *tmp;

    // save time and get 2 times as much, for next time. ;-)
    newsize*=2;
    
    // allocate the memory
    tmp = realloc(s1->string, newsize);

    if(NULL!=tmp) {
      s1->string=tmp;
      s1->memsize = newsize;
    }
    else {
      return s1;
    }
  }

  dst = s1->string+s1->length;
  memset(dst, 0, newsize-s1->length);
  // concatinate the strings
  memmove(dst, s2->string, s2->length);

  s1->length = s1->length+s2->length;

  // return the destination string
  return s1;
}

#ifndef NOTEST
void Test__cstring_strcat(CuTest* tc)
{
  CSTRING* a=NULL;
  CSTRING* b=NULL;
  CSTRING* c=NULL;
  CSTRING  z;

  // passing 2 NULLs
  a = cstring_strcat(NULL, NULL);
  CuAssertPtrEquals(tc, NULL, a);

  // passing first as NULL
  b = cstring_import("second", 6);
  a = cstring_strcat(NULL, b);
  CuAssertPtrEquals(tc, NULL, a);
  cstring_free(&b);

  // passing second as NULL
  b = cstring_import("second", 6);
  a = cstring_strcat(b, NULL);
  CuAssertPtrEquals(tc, b, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertTrue(tc, 0!=b->memsize);
  CuAssertIntEquals(tc, b->canfree, a->canfree);
  CuAssertIntEquals(tc, b->length, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, b->string, a->string);
  cstring_free(&b);

  // passing static as first argument
  a = cstring_import("second", 6);
  b = cstring_import("c", 1);
  z = cstring_strstr(a, b);
  cstring_free(&b);
  b = cstring_strcat(&z, a);
  CuAssertPtrEquals(tc, NULL, b);
  cstring_free(&a);

  // passing static cstring as second argument
  // passing static as second argument
  a = cstring_import("second", 6);
  b = cstring_import("third", 5);
  c = cstring_import("i", 1);
  z = cstring_strstr(b, c);
  a = cstring_strcat(a, &z);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 9, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "secondird", a->string);

  cstring_free(&a);
  cstring_free(&b);
  cstring_free(&c);
}
#endif // NOTEST
