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

#include "clog.h" // C*

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/**
 *  This is the type definition for the function pointer that can also get called when logging.
 *
 *  Example:
 *  \code
 *  // our callback function
 *  void my_callback(int level, char *logtext)
 *  {
 *    // print out the message to stdout
 *    printf("%d - %s", level, logtext);
 *  }
 *
 *  // our main function
 *  main(int argc, char **argv)
 *  {
 *    // define variables
 *    int loglevel1=0;
 *    int loglevel2=0;
 *    CLOG_INFO *log=NULL;
 *
 *    // get log level (perhaps the string was from a config file)
 *    loglevel1=clog_getlevel("ERROR");
 *    loglevel2=clog_getlevel("TRACE");
 *
 *    // open the log
 *    log = clog_open("/tmp/mylog.txt", loglevel1, my_callback, loglevel2);
 *
 *    // close the log and free the memory
 *    clog_close(log);
 *  }
 *  \endcode
 */


/** \brief Sets the callback function
 *
 *  Changes the callback of the logger after the clog_open function has already been called.
 *  You can pass NULL in the callback parameter to cause the logger to stop logging to the function.
 *
 *  \param info The CLOG_INFO struct that was returned from the clog_open call.
 *  \param callback The callback function you would like called.
 *
 *  \return If successfull it returns 1, On failure it returns 0.
 */
int  clog_setcallback(CLOG_INFO* info, C_CALLBACK callback)
{
  int retval=0;

  if(NULL==info) return 0;

  // if null == callback, clear the callback information
  if(NULL==callback) {
    info->s_callback = NULL;
  }
  else {
    info->s_callback = callback;
  }

  return retval;
}

/*@}*/
