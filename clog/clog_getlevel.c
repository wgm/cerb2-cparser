/* cLog - cross platform logging library
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

/**
 * \addtogroup clog
 */
/*@{*/

#include "clog.h" // CLEVEL

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/** \brief Convert char* log level to int value of log level

 Convert text to int log level. This function takes a char* and compares the
 first letter of the string and returns the associated log level.

 \param level Char pointer containing one of "FATAL","ERROR","WARN","DEBUG" ,"TRACE".

 \return returns an int of the associated ENUM (Should be greater than 0)

 \sa clog_setfilelevel clog_setcallbacklevel clog_errorlevels

 Example:
 \code
 // define variables
 int loglevel=0;
 CLOG_INFO *log=NULL;

 // get log level (perhaps the string was from a config file)
 loglevel=clog_getlevel("ERROR");

 // open the log
 log = clog_open("/tmp/mylog.txt", loglevel, NULL, 0);

 // close the log
 clog_close(log);
 \endcode
*/

int clog_getlevel(const char *level) {
  int retval=0;
  char tmp=0;

  if(NULL==level) return CMARK;

  tmp = level[0];

  if(0!=tmp) {

    // just check the first character.. it's fast and we also know people can't spell simple things.
    switch (tmp) {
      case 'M':
      case 'm':
        retval=CMARK; break;
      case 'F':
      case 'f':
        retval=CFATAL; break;
      case 'E':
      case 'e':
        retval=CERROR; break;
      case 'W':
      case 'w':
        retval=CWARN;  break;
      case 'D':
      case 'd':
        retval=CDEBUG; break;
      case 'T':
      case 't':
        retval=CTRACE; break;
      default:
        retval=0;
        break;
    }
  }

  return retval;
}

/*@}*/
