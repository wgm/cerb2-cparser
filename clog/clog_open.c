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

#include <sys/types.h> // getpid()
#include <unistd.h>    // getpid()
#include <stdlib.h>    // malloc
#include <assert.h>

#include "clog.h"      // clog_* && C*

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif


/** \brief Opens the log file

  Opens the log file and returns a CLOG_INFO structure that has been allocated with malloc().

  \param file Filename of the log file you want to open. Directory must already exist, File will be appended to.
  \param f_level A message will only be saved to file if the message's log level is at least this level.
  \param callback A function that will also be called on a logging.
  \param c_level The callback function will only be called if the message's log level is at least this level.

  \sa clog_getlevel clog_close clog_getlevel C_CALLBACK clog_errorlevels
*/

CLOG_INFO* clog_open(char * file, unsigned int f_level, C_CALLBACK callback, unsigned int c_level)
{
  CLOG_INFO *info=NULL;
  g_clog_pid = 0;

  // create the object to hold the info
  info = (CLOG_INFO*) malloc(sizeof(CLOG_INFO));
  assert(NULL!=info);  // make sure it's not null

  // set the values after opening the file
  info->s_logfile         = clog_fileopen(file);
  if(NULL != info->s_logfile) {
    info->s_logfile_level   = f_level;
  }
  else {
    info->s_logfile_level   = 0;
  }

  // set the callback values
  info->s_callback        = callback;
  if(NULL != info->s_callback) {
    info->s_callback_level  = c_level;
  }
  else {
    info->s_callback_level  = 0;
  }

  g_clog_pid = getpid();

  return info;
}

/*@}*/
