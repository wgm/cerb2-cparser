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

#include "clog.h" // C* && clog_*

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/** \brief sets the logging to a new file

 Closes existing log file (if open) and then opens a new file to log to

 \param info The CLOG_INFO struct that was returned from the clog_open call.
 \param file The file name that you want to now log to.

 \return If successfull it returns 1, On failure it returns 0.

 \see clog_open clog_setfilelevel

 Example:
 \code
 // variable to hold the logging info
 CLOG_INFO* log=NULL;

 // open the log file
 log = clog_open("/var/tmp1.txt", clog_getlevel("TRACE"), NULL, 0);

 // change the log file and change the log level
 clog_setfile(log, "/var/tmp2.txt");

 // close the log and free the memory 'log' is pointing to
 clog_close(log);
 \endcode
*/

int clog_setfile(CLOG_INFO* info, char * file)
{
  int retval=0;

  if(NULL==info) return 0;

  clog_fileclose(info);
  info->s_logfile = clog_fileopen(file);
  if(NULL!=info->s_logfile) {
    retval = 1;
  }

  return retval;
}

/*@}*/
