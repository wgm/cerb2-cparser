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

/** \brief Sets the callback log level.

  Changes the callback log level of the logger after the clog_open function has already been called.

  \param info The CLOG_INFO struct that was returned from the clog_open call.
  \param level The int that was returned from the clog_getlevel call.

  \return If successfull it returns 1, On failure it returns 0.

  \see clog_errorlevels
*/

int  clog_setcallbacklevel(CLOG_INFO* info, int level)
{
  int retval = 0;

  if(NULL==info) return 0;

  if(CMARK <= level && CTRACE >= level) {
    info->s_callback_level=level;
    retval = 1;
  }
  else {
    info->s_callback_level=0;
  }

  return retval;
}

/*@}*/
