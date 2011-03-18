
#include "cerberus.h"

// ##########################################################################
// ################=- curl_write() -=########################################
// ##########################################################################

size_t cer_curl_write(void *buffer, size_t size, size_t nmemb, void *userp)
{
  cstring_strcat_imp((CSTRING*)userp, (char*)buffer, size*nmemb);
  cstring_trim((CSTRING*)userp);
  return size*nmemb;
}

