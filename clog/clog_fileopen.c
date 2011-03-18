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

#include <stdio.h> // fopen
#include <stdio.h> // perror
#include <string.h> // strerror()
#include <errno.h> // errno

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/** \brief Opens a file and returns the file pointer.
 *
 *  This is a private funciton that is only called by other clog_* functions. It opens a file in append mode for logging to.
 *  \param file A character string of the file to try and open for logging.
 *
 *  \see clog_open
 */
FILE * clog_fileopen(char *file)
{
  FILE * retval = NULL;

  if(NULL==file) return NULL;

  // open the file if the filename is not NULL
  retval = fopen(file, "a");

  // check for an error opening the file
  if(NULL == retval) {
    fprintf(stderr, "Could not log to file \"%s\" - \"%s\"\n", file, strerror(errno));
  }

  // return the file pointer
  return retval;
}

/*@}*/
