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

/* compares s1 and s2 up to n length */
int cstring_strncmp(CSTRING* s1, CSTRING* s2, int n)
{
  return strncmp(s1->string, s2->string, n);
}

#ifndef NOTEST
void Test__cstring_strncmp(CuTest* tc)
{
  CSTRING* A = cstring_import("ABCDE", 5);
  CSTRING* a = cstring_import("abcde", 5);
  CSTRING* one = cstring_import("1234567890", 10);
  CSTRING* nine = cstring_import("9876543210", 10);

  CuAssertTrue(tc, 0>cstring_strncmp(A, a, 3));
  CuAssertTrue(tc, 0<cstring_strncmp(a, A, 3));
  CuAssertTrue(tc, 0==cstring_strncmp(A, A, 3));

  CuAssertTrue(tc, 0>cstring_strncmp(one, nine, 3));
  CuAssertTrue(tc, 0<cstring_strncmp(nine, one, 3));
  CuAssertTrue(tc, 0==cstring_strncmp(nine, nine, 3));

  CuAssertTrue(tc, 0<cstring_strncmp(a, one, 3));
  CuAssertTrue(tc, 0<cstring_strncmp(a, nine, 3));
  CuAssertTrue(tc, 0<cstring_strncmp(A, one, 3));
  CuAssertTrue(tc, 0<cstring_strncmp(A, nine, 3));

  cstring_free(&A);
  cstring_free(&a);
  cstring_free(&one);
  cstring_free(&nine);
}
#endif // NOTEST
