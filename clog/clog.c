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
#include <stdarg.h> // va_*
#include <string.h> // memset
#include <stdio.h> // vsnprintf
#include <time.h> // localtime

#include "clog.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/** \brief String array used for file log levels.
 *
 *  String array used for the log file error levels. These are the log levels that are written to the log file. If you wanted to use different log level text in the log file you would change this here.
 *
 */
char *(clog_text[]) = { "MARK ", "FATAL", "ERROR", "WARN ", "DEBUG", "TRACE" };

/** \brief Integer used to store the process ID
 *
 *  Integer used to store the process ID so we do not have to look up the process ID for each log entry
 */
int g_clog_pid;

/** \mainpage CLogger
 *
 *  \section license License
 *  Copyright (c) 2002 WebGroup Media, LLC, All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  \section introduction Introduction
 *  CLogger is a simple logger that logs to files. It is designed to be correct and portable to many OSs.
 *
 *  \section compiling Compiling
 *  \subsection configure Configure
 *    After unpacking the archive, you will need to edit the Makefile to ensure
 *    that the settings are correct. After you have done that, run gmake to
 *    compile the library. A lib will be made in the ../build/ directory.
 *
 *  \subsection make Make
 *    After the configure script has finished generating the makefiles
 *    simply run 'gmake' in the directory and everything will compile.
 *
 *  \subsection install Install
 *    When everything is installed just type 'gmake install' and the software
 *    installs itself.
 *
 *  \section problems Problems
 *    Please report any problems with this library to Ben Halsted of WebGroup Media, LLC.
 *    ben@webgroupmedia.com or http://www.webgroupmedia.com and find an email address there.
 *
 *  \author Ben Halsted (ben@webgroupmedia.com)
 *  \date Fri Aug 2 2002
 *
 */

/**
 * Logs to the file and function
 *
 *  Logs to the file and function that were specified in the clog_open function. This function's last 2 parameters are the same as printf's 2 parameters.
 *
 *  \param info The CLOG_INFO struct that was returned from the clog_open call.
 *  \param level The int that was returned from the clog_getlevel call.
 *  \param format see the manual on printf for the format parameter and the arguments
 *
 *  \sa clog_open clog_getlevel clog_errorlevels
 *
 *  Example:
 *  \code
 *  // define variables
 *  CLOG_INFO *info=NULL;
 *  int messagenumber = 0;
 *
 *  // open the log
 *  info = clog_open("/tmp/mylog.txt", clog_getlevel("DEBUG"), NULL, 0);
 *
 *  // will get logged
 *  clog(info, CFATAL, "fatal message %d", messagenumber++);
 *
 *  // will get logged
 *  clog(info, CERROR, "error message %d", messagenumber++);
 *
 *  // will get logged
 *  clog(info, CWARN , "warn  message %d", messagenumber++);
 *
 *  // will NOT get logged
 *  clog(info, CDEBUG, "debug message %d", messagenumber++);
 *
 *  // will NOT get logged
 *  clog(info, CTRACE, "trace message %d", messagenumber++);
 *
 *  // close the log and free the memory
 *  clog_close(log);
 *  \endcode
 *
 *
 */
void clog(CLOG_INFO* info, int level, const char *format, ...)
{
  // cannot put logging in this function due to recursion

  if(NULL==info) return;

  if(!(-1 < level && 6 > level)) {
    level = CMARK;
  }

  // if we are logging to the function or file
  if(level <= info->s_logfile_level || level <= info->s_callback_level) {
    va_list ap; // for the variable args
    time_t    t;
    struct tm *ts;
    char message[1024];

    memset((char*)message, 0, 1024);
    va_start(ap, format);
    vsnprintf(message, 1023, format, ap);
    va_end(ap);

    // log to function
    if(NULL!=info->s_callback && level <= info->s_callback_level) {
      info->s_callback(level, message);
    }

    // log to file
    if(NULL!=info->s_logfile && level <= info->s_logfile_level) {
      if(NULL!=info->s_logfile) { // log to text file
        time(&t);
        ts = localtime(&t);

        if(NULL!=ts) { // add the time to the beginning
          fprintf(info->s_logfile, "[%d.%.2d.%.2d %.2d:%.2d:%.2d %d] %s ", ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec, g_clog_pid, clog_text[level]);
        }

        fprintf(info->s_logfile, message);
#ifdef WIN32
        fprintf(info->s_logfile, "\r");
#endif
        fprintf(info->s_logfile, "\n");
        fflush(info->s_logfile);
        ts = NULL;
      }
    }
  }
}

/*@}*/
