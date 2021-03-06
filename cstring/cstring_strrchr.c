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

CSTRING cstring_strrchr(CSTRING* haystack, int chr)
{
  CSTRING r;

  memset(&r, 0, sizeof(r));

  if(NULL==haystack)
    r.string = NULL;
  else
    r.string = strrchr(haystack->string, chr);

  if(NULL != r.string) {
    r.length = haystack->length - (r.string - haystack->string);
  }
  else {
    r.length = 0;
  }

  r.memsize=0;
  r.canfree=0;

  return r;
}

#ifndef NOTEST
void Test__cstring_strrchr(CuTest* tc)
{
  CSTRING z;
  CSTRING *a=NULL;

  // passing NULL in first parameter
  z = cstring_strrchr(NULL, 0);
  CuAssertIntEquals(tc, 0, z.canfree);
  CuAssertIntEquals(tc, 0, z.length);
  CuAssertIntEquals(tc, 0, z.memsize);
  CuAssertPtrEquals(tc, NULL, z.string);

  // pass abcde as first, 0 as second parameter
  a = cstring_import("abcde", 5);
  z = cstring_strrchr(a, 0);
  CuAssertIntEquals(tc, 0, z.canfree);
  CuAssertIntEquals(tc, 0, z.length);
  CuAssertIntEquals(tc, 0, z.memsize);
  CuAssertPtrEquals(tc, a->string+a->length, z.string);
  cstring_free(&a);

  // pass abcde as first, c as second
  a = cstring_import("abcde", 5);
  z = cstring_strrchr(a, 'c');
  CuAssertIntEquals(tc, 0, z.canfree);
  CuAssertIntEquals(tc, 3, z.length);
  CuAssertIntEquals(tc, 0, z.memsize);
  CuAssertPtrEquals(tc, a->string+2, z.string);
  cstring_free(&a);

  // pass abcde as first, z as second
  a = cstring_import("abcde", 5);
  z = cstring_strrchr(a, 'z');
  CuAssertIntEquals(tc, 0, z.canfree);
  CuAssertIntEquals(tc, 0, z.length);
  CuAssertIntEquals(tc, 0, z.memsize);
  CuAssertPtrEquals(tc, NULL, z.string);
  cstring_free(&a);
}
#endif // NOTEST
