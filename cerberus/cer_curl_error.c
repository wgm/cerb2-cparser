
#include "cerberus.h"

// ##########################################################################
// ################=- curl_error() -=########################################
// ##########################################################################

int cer_curl_error(CURL *curl, curl_infotype it, char *mesg, size_t size, void *data)
{
  char *err = NULL;

  if(0<size && ( CURLINFO_TEXT==it || CURLINFO_HEADER_IN==it || CURLINFO_HEADER_OUT==it ) ) {
    err=malloc(size+1);
    if(NULL!=err) {
      memset(err, 0, size+1);
      memcpy(err, mesg, size);
      if(CURLINFO_TEXT==it) {
        clog((CLOG_INFO*)data, CDEBUG, "CURL: %s", err);
      }
      else if(CURLINFO_HEADER_IN==it || CURLINFO_HEADER_IN==it) {
        clog((CLOG_INFO*)data, CTRACE, "CURL: %s", err);
      }
      free(err);
    }
  }
  return 0;
}

