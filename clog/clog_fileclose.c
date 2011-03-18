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

#include <stdio.h> // fflush && fclose

#include "clog.h" // CLOG_INFO

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/** \brief Flushes all remaining data to file and closes the file.
 *
 *  This is a private function that is called only by other clog_* functions.
 *
 *  \param info The CLOG_INFO structure that was returned from the clog_open() call.
 *
 *  \see clog_close
 */
void clog_fileclose(CLOG_INFO * info)
{
  if(NULL!=info) {
    // flush/close logfile
    if(NULL!=info->s_logfile) {
      fflush(info->s_logfile);
      fclose(info->s_logfile);
    }
    info->s_logfile = NULL;
  }
}

/*@}*/
