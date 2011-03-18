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

#include "./cstring.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/* converts entire cstring to upper case */
CSTRING* cstring_strupper(CSTRING* s)
{
  register char *p;

  if(NULL==s) return NULL;

  p=s->string;

  while(*p) {
    if(96<*p && 123>*p) {
      *p -= 32;
    }
    ++p;
  }

  return s;
}

#ifndef NOTEST
void Test__cstring_strupper(CuTest* tc)
{
  CSTRING* a=NULL;
  CSTRING  b;

  // pass NULL
  CuAssertPtrEquals(tc, NULL, cstring_strlower(NULL));

  // pass non-alloc'd cstring
  a = cstring_import("abcde", 5);
  b = cstring_strchr(a, 'a');
  cstring_strupper(&b);
  CuAssertTrue(tc, 0==b.memsize);
  CuAssertIntEquals(tc, 0, b.canfree);
  CuAssertIntEquals(tc, 5, b.length);
  CuAssertPtrNotNull(tc, b.string);
  CuAssertStrEquals(tc, "ABCDE", b.string);
  cstring_free(&a);

  // pass alloc'd cstring
  a = cstring_import("abcde", 5);
  cstring_strupper(a);
  CuAssertPtrNotNull(tc, a);
  CuAssertTrue(tc, 0!=a->memsize);
  CuAssertIntEquals(tc, 1, a->canfree);
  CuAssertIntEquals(tc, 5, a->length);
  CuAssertPtrNotNull(tc, a->string);
  CuAssertStrEquals(tc, "ABCDE", a->string);
  cstring_free(&a);
}
#endif // NOTEST
