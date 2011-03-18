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
#ifndef __CLOG_H__
#define __CLOG_H__ 1

#include <stdio.h>

/**
 * \defgroup clog cLog
 */
/*@{*/

/**
  This is the type definition for the function pointer that can also get called when logging.
 */
typedef void (*C_CALLBACK)(int, char*);

/** \enum errorlevels
 *  The log levels in int format.
 *
 *  Use these as the second parameter to clog
 *
 *  \sa clog
 */
enum clog_errorlevels {
  /** MARK level is used as a non-reporting level */
  CMARK  = 0,
  /** FATAL level only reports errors that cause the application to EXIT */
  CFATAL = 1,
  /** ERROR level also reports errors that are serious but it can keep processing */
  CERROR = 2,
  /** WARN level also reports errors that may be important */
  CWARN  = 3,
  /** DEBUG level also reports information on what is happening so errors can be reported better */
  CDEBUG = 4,
  /** TRACE level reports everything that is loggable */
  CTRACE = 5
};


/**
  This is the struct that holds the destination info. Do not modify the values directly
*/
struct clog_info_struct {
  /** The FILE* we are going to write to. */
  FILE          * s_logfile;
  /** Log level for logging to file. */
  int    s_logfile_level;
  /** The callback we call if it's not NULL. */
  C_CALLBACK      s_callback;
  /** Log level for logging to the callback. */
  int    s_callback_level;
};

/**
  typedef so that it is easier to create clog_info_struct variables.
*/
typedef struct clog_info_struct CLOG_INFO;

extern char *(clog_text[]);
extern int g_clog_pid;

extern int          clog_getlevel(const char *level);
extern int          clog_setfile(CLOG_INFO* info, char * file);
extern int          clog_setfilelevel(CLOG_INFO* info, unsigned int level);
extern int          clog_setcallback(CLOG_INFO* info, C_CALLBACK callback);
extern int          clog_setcallbacklevel(CLOG_INFO* info, int level);
extern void         clog_stderr(int level, char *logtext);
extern void         clog_stdout(int level, char *logtext);
extern CLOG_INFO*   clog_open(char * file, unsigned int f_level, C_CALLBACK callback, unsigned int c_level);
extern void         clog(CLOG_INFO* info, int level, const char *format, ...);
extern void         clog_close(CLOG_INFO* info);
extern void         clog_perror(CLOG_INFO* info, int level, const char *s);

// 'private' functions
void                clog_fileclose(CLOG_INFO * info);
FILE*               clog_fileopen(char *file);

/*@}*/

#endif // __CLOG_H__
