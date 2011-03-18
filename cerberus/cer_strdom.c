#include "cerberus.h"

// ##########################################################################
// #################=- Check if a domain is in a string -=###################
// ##########################################################################
int cer_strdom(CLOG_INFO* log, CSTRING* haystack, CSTRING* needle) {
  CSTRING *h=NULL;
  CSTRING *n=NULL;

  unsigned char *a=NULL;
  unsigned char *b=NULL;

  // check parameters
  if(NULL==haystack || NULL==haystack->string || 1>haystack->length
           || NULL==needle || NULL==needle->string || 1>haystack->length) {
    return 0;
  }

  // copy the xml info so we can make it all lowercase
  h = cstring_strdup(haystack);
  n = cstring_strdup(needle);

  // make sure we have data from the malloc's in the cstring_strdup()
  if(NULL==h || NULL==n) {
    return 0;
  }

  // lowercase the needle and haystack
  cstring_strlower(h);
  cstring_strlower(n);

  // the end of the haystack (put the ptr on the trailing zero)
  b = h->string + h->length;
  a = h->string;

  // find the @ sign which is in email addresses
  while(NULL!=(a=strchr(a, '@'))) {
    // get past the @ sign
    ++a;

    // check for the domain
    while ((b-a) >= n->length) {

      // check the needle vs current position
      if(0==memcmp(a, n->string, n->length)) {
        // it's the same!

        // free the copies
        cstring_free(&h);
        cstring_free(&n);

        // return successful
        return 1;
      }

      while(0==isspace(*a) && '.'!=*a && 0!=*a) {
        ++a;
      }

      if(isspace(*a)) {
        ++a; // go past the space or period
        break; // get back to the outer loop and search for the next @ sign
      }
      else { // must have been a period
        ++a; // go past the space or period
      }

    }
  }

  // free the copies
  cstring_free(&h);
  cstring_free(&n);

  // return failure
  return 0;

}

