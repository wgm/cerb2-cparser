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

/** \brief sets the logging to a new log level

 Sets the log level for logging to a file to the new log level.

 \param info The CLOG_INFO struct that was returned from the clog_open call.
 \param level The int that was returned from the clog_getlevel call.

 \return If successfull it returns 1, On failure it returns 0.

 \sa clog_open clog_setfile clog_getlevel clog_errorlevels

 Example:
 \code
 // variable to hold the logging info
 CLOG_INFO* log=NULL;

 // open the log file
 log = clog_open("/var/tmp1.txt", clog_getlevel("TRACE"), NULL, 0);

 // change the log file and change the log level
 clog_setfilelevel(log, clog_getlevel("FATAL"));

 // close the log and free the memory 'log' is pointing to
 clog_close(log);
 \endcode
*/

int clog_setfilelevel(CLOG_INFO* info, unsigned int level)
{
  int retval = 0;

  if(NULL==info) return 0;

  if(CTRACE >= level) {
    info->s_logfile_level=level;
    retval = 1;
  }
  else {
    info->s_logfile_level=CMARK;
  }

  return retval;
}

/*@}*/
