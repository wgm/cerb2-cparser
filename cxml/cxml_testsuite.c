#ifndef NOTEST
#include <CuTest.h>
#include "cxml.h"

CuSuite* TestSuite__cxml(void)
{

  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, Test__cxml_fn_data);
  SUITE_ADD_TEST(suite, Test__cxml_fn_end);
  SUITE_ADD_TEST(suite, Test__cxml_fn_start);
  SUITE_ADD_TEST(suite, Test__cxml_node_addattribute);
  SUITE_ADD_TEST(suite, Test__cxml_node_adddata);
  SUITE_ADD_TEST(suite, Test__cxml_node_addnode);
  SUITE_ADD_TEST(suite, Test__cxml_node_create);
  SUITE_ADD_TEST(suite, Test__cxml_node_delete);
  SUITE_ADD_TEST(suite, Test__cxml_node_free);
  SUITE_ADD_TEST(suite, Test__cxml_node_getattribute);
  SUITE_ADD_TEST(suite, Test__cxml_node_get);
  SUITE_ADD_TEST(suite, Test__cxml_node_iterate);
  SUITE_ADD_TEST(suite, Test__cxml_node_new);
  SUITE_ADD_TEST(suite, Test__cxml_node_next);
  SUITE_ADD_TEST(suite, Test__cxml_node_tostring);
  SUITE_ADD_TEST(suite, Test__cxml_read_buf);
  SUITE_ADD_TEST(suite, Test__cxml_read_file);
  SUITE_ADD_TEST(suite, Test__cxml_root_free);
  SUITE_ADD_TEST(suite, Test__cxml_root_new);

  return suite;
}

#endif // NOTEST

