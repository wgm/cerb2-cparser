/* cFile - file I/O library with line buffering
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

#ifndef NOTEST
#include <CuTest.h>
#include "cfile.h"

CuSuite* TestSuite__cfile(void)
{
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, Test__cfile_buffer);
  SUITE_ADD_TEST(suite, Test__cfile_cleanup);
  SUITE_ADD_TEST(suite, Test__cfile_close);
  SUITE_ADD_TEST(suite, Test__cfile_free);
  SUITE_ADD_TEST(suite, Test__cfile_getline);
  SUITE_ADD_TEST(suite, Test__cfile_getline_buffer);
  SUITE_ADD_TEST(suite, Test__cfile_getline_next);
  SUITE_ADD_TEST(suite, Test__cfile_init);
  SUITE_ADD_TEST(suite, Test__cfile_lseek);
  SUITE_ADD_TEST(suite, Test__cfile_mkstemp);
  SUITE_ADD_TEST(suite, Test__cfile_new);
  SUITE_ADD_TEST(suite, Test__cfile_open);
  SUITE_ADD_TEST(suite, Test__cfile_open_create);
  SUITE_ADD_TEST(suite, Test__cfile_pipe);
  SUITE_ADD_TEST(suite, Test__cfile_read);
  SUITE_ADD_TEST(suite, Test__cfile_sync);
  SUITE_ADD_TEST(suite, Test__cfile_unlink);
  SUITE_ADD_TEST(suite, Test__cfile_write);
  return suite;
}

#endif // NOTEST
