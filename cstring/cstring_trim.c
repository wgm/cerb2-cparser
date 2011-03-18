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

/* trims the left and right side of a cstring */
CSTRING* cstring_trim(CSTRING* s)
{
  cstring_ltrim(s);
  cstring_rtrim(s);
  return s;
}

#ifndef NOTEST
void Test__cstring_trim(CuTest* tc)
{
  CSTRING* a=NULL;

  // passing NULL
  a = cstring_trim(NULL);
  CuAssertPtrEquals(tc, NULL, a);

  // passing 0 length string
  a = cstring_new(0);
  cstring_trim(a);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 0, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "", a->string);
  cstring_free(&a);

  // passing ' \\n\\tabcde\\n\\t '
  a = cstring_import(" \n\tabcde\n\t ", 8);
  cstring_trim(a);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 5, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "abcde", a->string);
  cstring_free(&a);

  // passing 'abcde'
  a = cstring_import("abcde", 5);
  cstring_trim(a);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 5, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "abcde", a->string);
  cstring_free(&a);
}
#endif // NOTEST
