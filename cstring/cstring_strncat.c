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
CSTRING* cstring_strncat(CSTRING* s1, CSTRING* s2, int n)
{
  int newsize=0;
  int length=0;
  // if the destination is NULL, return NULL
  if(NULL==s1) return NULL;

  // if the string to concat is NULL, return the destination
  if(NULL==s2) return s1;

  // see if we can append stuff to s1
  if(1!=s1->canfree) return NULL;

  // check to see if we need to increase the memory size
  length=(s2->length>n?n:s2->length);
  newsize = s1->length + length + 1;
  if(0>=(s1->memsize - newsize)) {
    void *tmp;

    // get twice as much, for next time.
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

  memset((s1->string+s1->length), 0, length);
  // concatinate the strings
  memmove((s1->string+s1->length), s2->string, length);

  s1->length = s1->length+length;

  // return the destination string
  return s1;
}

#ifndef NOTEST
void Test__cstring_strncat(CuTest* tc)
{
  CSTRING* a=NULL;
  CSTRING* b=NULL;
  CSTRING* c=NULL;
  CSTRING  z;

  // passing 2 nulls
  a = cstring_strncat(NULL, NULL, 0);
  CuAssertPtrEquals(tc, NULL, a);

  // passing first as null
  b = cstring_import("second", 6);
  a = cstring_strncat(NULL, b, 3);
  CuAssertPtrEquals(tc, NULL, a);
  cstring_free(&b);

  // passing second as null
  b = cstring_import("second", 6);
  a = cstring_strncat(b, NULL, 3);
  CuAssertPtrNotNull(tc, a);
  CuAssertPtrEquals(tc, b, a);
  cstring_free(&b);

  // passing static as first argument
  a = cstring_import("second", 6);
  b = cstring_import("c", 1);
  z = cstring_strstr(a, b);
  cstring_free(&b);
  b = cstring_strncat(&z, a, 3);
  CuAssertPtrEquals(tc, NULL, b);
  cstring_free(&a);

  // passing static as second argument
  a = cstring_import("second", 6);
  b = cstring_import("third", 5);
  c = cstring_import("i", 1);
  z = cstring_strstr(b, c);
  a = cstring_strncat(a, &z, 3);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 9, a->length);
  CuAssertPtrNotNull(tc, a->string);

  // valgrind freaks with this for some reason:
  // conditional jump or move depends on uninitialized value(s)
  //CuAssertStrEquals(tc, "secondird", a->string);

  cstring_free(&a);
  cstring_free(&b);
  cstring_free(&c);
}
#endif // NOTEST


