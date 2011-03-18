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

#include <stdio.h> // perror
#include <string.h> // strerror
#include <errno.h> // errno

#include "clog.h" // C* clog_*

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/** \brief Acts as perror but logs to a file. Can also give it text to prefix error with.
 *
 *  Logs the string version of the errno (from <errno.h>/<string.h>) to the log file.
 *  You can also prefix the error with some text. This allows the log file to show more information
 *  about why/where the error happened.
 *
 *  \param info The CLOG_INFO structure that was returned from the clog_open() call.
 *  \param level The log level at which this error will show.
 *  \param s the prefix for the error.
 *
 *  \see clog_errorlevels
 */
void clog_perror(CLOG_INFO* info, int level, const char *s)
{
  // check here so we do not waste time doing strerror this will also
  // be done in clog (the first if below) so it is redundant to save time.
  if(level <= info->s_logfile_level || level <= info->s_callback_level) {
    if(NULL!=s && 0!=*s) {
      clog(info, level, "%s %s", s, strerror(errno));
    }
    else {
      clog(info, level, "%s", strerror(errno));
    }
  }
}

/*@}*/
