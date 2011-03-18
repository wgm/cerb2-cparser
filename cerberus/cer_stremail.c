#include "cerberus.h"

// ##########################################################################
// #################=- Check if an email address is in a string -=###########
// ##########################################################################
int cer_stremail(CLOG_INFO* log, CSTRING* haystack, CSTRING* needle) {
  CSTRING *h=NULL;
  CSTRING *n=NULL;

  unsigned char *a=NULL;
  unsigned char *b=NULL;
  int emailchar=0;

  // check parameters
  if(NULL==haystack || NULL==haystack->string || 1>haystack->length
           || NULL==needle || NULL==needle->string || 1>haystack->length) {
    return -1;
  }

  // make sure the needle has an at sign (@)
  if(NULL==(a=strchr(needle->string, '@'))) {
    // didn't have an @ sign
    return -1;
  }
  else {
    // check that the @ sign isn't the first thing in the needle
    if(a==(unsigned char*)(needle->string)) {
      // it was
      return -1;
    }
    // reset 'a' variable
    a=NULL;
  }

  // copy the xml info so we can make it all lowercase
  h = cstring_strdup(haystack);
  n = cstring_strdup(needle);

  // make sure we have data from the malloc's in the cstring_strdup()
  if(NULL==h || NULL==n) {
    return -1;
  }

  // lowercase the needle and haystack
  cstring_strlower(h);
  cstring_strlower(n);

  // the end of the haystack (put the ptr on the trailing zero)
  b = h->string + h->length;
  a = h->string;

  // check for the email address
  while ((b-a) >= n->length) {
    // find a valid character to start the comparison from
    emailchar = 0;
    while(!emailchar && 0!=*a) {
      switch (*a) {
        // special chars
        case '.':
        case '_':
        case '-':
        // numbers
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        // alphabetic
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
          emailchar=1;
          break;

        default:
          // everything else keep looping
          ++a;
          break;
      }
    }

    // check the needle vs current position
    if(0==memcmp(a, n->string, n->length)) {
      // it's the same!

      // free the copies
      cstring_free(&h);
      cstring_free(&n);

      // return successful
      return 2;
    }
    ++a;
  }

  // free the copies
  cstring_free(&h);
  cstring_free(&n);

  // return failure
  return -1;

}



