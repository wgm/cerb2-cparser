#ifndef NOTEST
#include <CuTest.h>
#include "cstring.h"

CuSuite* TestSuite__cstring(void)
{
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, Test__cstring_free);
  SUITE_ADD_TEST(suite, Test__cstring_import);
  SUITE_ADD_TEST(suite, Test__cstring_ltrim);
  SUITE_ADD_TEST(suite, Test__cstring_new);
  SUITE_ADD_TEST(suite, Test__cstring_rtrim);
  SUITE_ADD_TEST(suite, Test__cstring_strcat);
  SUITE_ADD_TEST(suite, Test__cstring_strcat_imp);
  SUITE_ADD_TEST(suite, Test__cstring_strchr);
  SUITE_ADD_TEST(suite, Test__cstring_strcmp);
  SUITE_ADD_TEST(suite, Test__cstring_strdup);
  SUITE_ADD_TEST(suite, Test__cstring_stricmp);
  SUITE_ADD_TEST(suite, Test__cstring_strlen);
  SUITE_ADD_TEST(suite, Test__cstring_strlower);
  SUITE_ADD_TEST(suite, Test__cstring_strncat);
  SUITE_ADD_TEST(suite, Test__cstring_strncmp);
  SUITE_ADD_TEST(suite, Test__cstring_strndup);
  SUITE_ADD_TEST(suite, Test__cstring_strnicmp);
  SUITE_ADD_TEST(suite, Test__cstring_strrchr);
  SUITE_ADD_TEST(suite, Test__cstring_strstr);
  SUITE_ADD_TEST(suite, Test__cstring_strupper);
  SUITE_ADD_TEST(suite, Test__cstring_trim);
  return suite;
}

#endif // NOTEST
