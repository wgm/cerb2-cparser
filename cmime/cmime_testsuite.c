/***************************************************************************
                          cmime.h  -  description
                             -------------------
    begin                : Wed Mar 19 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/
 
#ifndef NOTEST
#include <CuTest.h>
#include "cmime.h"

CuSuite* TestSuite__cmime(void)
{
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, Test__cmime_body_parse);
  SUITE_ADD_TEST(suite, Test__cmime_file_parse);
  SUITE_ADD_TEST(suite, Test__cmime_header_parse_ct);
  SUITE_ADD_TEST(suite, Test__cmime_header_parse);
  SUITE_ADD_TEST(suite, Test__cmime_parse_822);
  SUITE_ADD_TEST(suite, Test__cmime_parse_b64_string);
  SUITE_ADD_TEST(suite, Test__cmime_parse_b64);
  SUITE_ADD_TEST(suite, Test__cmime_parse_qptext_line);
  SUITE_ADD_TEST(suite, Test__cmime_parse_qptext);
  SUITE_ADD_TEST(suite, Test__cmime_parse_subject);
  SUITE_ADD_TEST(suite, Test__cmime_parse_text);
  SUITE_ADD_TEST(suite, Test__cmime_parse_tnef);
  SUITE_ADD_TEST(suite, Test__cmime_parse_uu_begin);
  SUITE_ADD_TEST(suite, Test__cmime_parse_uu);
  return suite;
}

#endif // NOTEST

