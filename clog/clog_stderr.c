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

#include <time.h> // localtime
#include "clog.h"

extern char **g_text;


/**
 * \addtogroup clog
 */
/*@{*/

/** \brief A callback function used to log to stderr
 *
 *  Use this functionwhen you would like to log to stderr using the clog_setcallback() function.
 *
 *  \param level The log level that this log entry is at
 *  \param logtext The text that should be logged
 *
 *  \see clog_setcallback C_CALLBACK
 */
void clog_stderr(int level, char *logtext)
{
  time_t    t;
  struct tm *ts;

  // print out the message to stdout
  time(&t);
  ts = localtime(&t);

  if(NULL!=ts) { // add the time to the beginning
    fprintf(stderr, "[%d.%.2d.%.2d %.2d:%.2d:%.2d] %s ", ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec, clog_text[level]);
  }

  fprintf(stderr, logtext);

#ifdef WIN32
  fprintf(stderr, "\r");
#endif

  fprintf(stderr, "\n");
  fflush(stderr);
  ts = NULL;
}

/*@}*/
