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

#ifndef __CSTRING_H__
#define __CSTRING_H__

#ifndef CSTRING_BUFSIZE
#  define CSTRING_BUFSIZE 64
#endif

typedef struct _CSTRING {
  char *string;
  int length;
  int memsize;
  int canfree;
} CSTRING;

#define cstring_index cstring_strchr
#define cstring_rindex cstring_strrchr

#ifndef NOTEST
#include <CuTest.h>
extern CuSuite* TestSuite__cstring(void);
extern void Test__cstring_free(CuTest* tc);
extern void Test__cstring_new(CuTest* tc);
extern void Test__cstring_strdup(CuTest* tc);
extern void Test__cstring_strndup(CuTest* tc);
extern void Test__cstring_import(CuTest* tc);
extern void Test__cstring_strcat(CuTest* tc);
extern void Test__cstring_strcat_imp(CuTest* tc);
extern void Test__cstring_strncat(CuTest* tc);
extern void Test__cstring_strlower(CuTest* tc);
extern void Test__cstring_strupper(CuTest* tc);
extern void Test__cstring_trim(CuTest* tc);
extern void Test__cstring_ltrim(CuTest* tc);
extern void Test__cstring_rtrim(CuTest* tc);
extern void Test__cstring_strcmp(CuTest* tc);
extern void Test__cstring_strncmp(CuTest* tc);
extern void Test__cstring_stricmp(CuTest* tc);
extern void Test__cstring_strnicmp(CuTest* tc);
extern void Test__cstring_strlen(CuTest* tc);
extern void Test__cstring_strstr(CuTest* tc);
extern void Test__cstring_strchr(CuTest* tc);
extern void Test__cstring_strrchr(CuTest* tc);
#endif

/* creates a new cstring */
extern CSTRING* cstring_new(int size);

/* deletes a cstring */
extern void cstring_free(CSTRING **p);

/* duplicates a cstring string */
extern CSTRING* cstring_strdup(CSTRING* s);

/* duplicates a cstring string of n length */
extern CSTRING* cstring_strndup(CSTRING* s, int n);

/* imports a char* into a new cstring of n length */
extern CSTRING* cstring_import(char *s, int n);

/* concatinates s2 onto s1 */
extern CSTRING* cstring_strcat(CSTRING* s1, CSTRING* s2);

/* concatinates s2 onto s1 of n length */
extern CSTRING* cstring_strcat_imp(CSTRING* s1, char* s2, int n);

/* concatinates s2 onto s1 but only of n length */
extern CSTRING* cstring_strncat(CSTRING* s1, CSTRING* s2, int n);

/* converts entire cstring to lower case */
extern CSTRING* cstring_strlower(CSTRING* s);

/* converts entire cstring to upper case */
extern CSTRING* cstring_strupper(CSTRING* s);

/* trims the left and right side of a cstring */
extern CSTRING* cstring_trim(CSTRING* s);

/* trims the left side of a cstring */
extern CSTRING* cstring_ltrim(CSTRING* s);

/* trims the right side of a cstring */
extern CSTRING* cstring_rtrim(CSTRING* s);

/* compares s1 and s2 */
extern int cstring_strcmp(CSTRING* s1, CSTRING* s2);

/* compares s1 and s2 up to n length */
extern int cstring_strncmp(CSTRING* s1, CSTRING* s2, int n);

/* compares (case insensitive) s1 and s2 */
extern int cstring_stricmp(CSTRING* s1, CSTRING* s2);

/* compares (case insensitive) s1 and s2 up to n length */
extern int cstring_strnicmp(CSTRING* s1, CSTRING* s2, int n);

/* returns the string length */
extern int cstring_strlen(CSTRING* s);

/* search for a cstring in a cstring */
extern CSTRING cstring_strstr(CSTRING* haystack, CSTRING* needle);

/* search for a chr in a cstring */
extern CSTRING cstring_strchr(CSTRING* haystack, int chr);

/* search for a chr in a cstring (reverse) */
extern CSTRING cstring_strrchr(CSTRING* haystack, int chr);

#endif // __CSTRING_H__
