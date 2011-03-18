/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Sun Dec 22 01:12:33 GMT 2002
    copyright            : (C) 2002 by Ben Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

/**************************************************************************
 * COPYRIGHT AND PERMISSION NOTICE                                        *
 *                                                                        *
 * Copyright (c) 2002 WebGroup Media, LLC, All rights reserved.           *
 *                                                                        *
 * Permission is hereby granted, free of charge, to any person obtaining  *
 * a copy of this software and associated documentation files (the        *
 * "Software"), to deal in the Software without restriction, including    *
 * without limitation the rights to use, copy, modify, merge, publish,    *
 * distribute, sublicense, and/or sell copies of the Software, and to     *
 * permit persons to whom the Software is furnished to do so, subject to  *
 * the following conditions:                                              *
 *                                                                        *
 * The above copyright notice and this permission notice shall be         *
 * included in all copies or substantial portions of the Software.        *
 *                                                                        *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "clog.h"

void callback(int level, char *msg) {
  fprintf(stdout, "Level: %d - Message: \"%s\"\n", level, msg);
}

#ifndef PACKAGE
#define PACKAGE "cLogger"
#endif

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int main(int argc, char *argv[])
{
  CLOG_INFO *info=NULL;
  int x=0;

  info = clog_open("test.log", clog_getlevel("MARK"), NULL, 0);

  clog(info, CMARK , "%s", PACKAGE);

  clog(info, CMARK , "%s %d", "MARK", x++);
  clog(info, CFATAL, "%s %d", "MARK", x++);
  clog(info, CERROR, "%s %d", "MARK", x++);
  clog(info, CWARN , "%s %d", "MARK", x++);
  clog(info, CDEBUG, "%s %d", "MARK", x++);
  clog(info, CTRACE, "%s %d", "MARK", x++);

  clog_setfilelevel(info, clog_getlevel("FATAL"));

  clog(info, CMARK , "%s %d", "FATAL", x++);
  clog(info, CFATAL, "%s %d", "FATAL", x++);
  clog(info, CERROR, "%s %d", "FATAL", x++);
  clog(info, CWARN , "%s %d", "FATAL", x++);
  clog(info, CDEBUG, "%s %d", "FATAL", x++);
  clog(info, CTRACE, "%s %d", "FATAL", x++);

  clog_setfilelevel(info, clog_getlevel("ERROR"));

  clog(info, CMARK , "%s %d", "ERROR", x++);
  clog(info, CFATAL, "%s %d", "ERROR", x++);
  clog(info, CERROR, "%s %d", "ERROR", x++);
  clog(info, CWARN , "%s %d", "ERROR", x++);
  clog(info, CDEBUG, "%s %d", "ERROR", x++);
  clog(info, CTRACE, "%s %d", "ERROR", x++);

  clog_setfilelevel(info, clog_getlevel("WARN"));

  clog(info, CMARK , "%s %d", "WARN", x++);
  clog(info, CFATAL, "%s %d", "WARN", x++);
  clog(info, CERROR, "%s %d", "WARN", x++);
  clog(info, CWARN , "%s %d", "WARN", x++);
  clog(info, CDEBUG, "%s %d", "WARN", x++);
  clog(info, CTRACE, "%s %d", "WARN", x++);

  clog_setfilelevel(info, clog_getlevel("DEBUG"));

  clog(info, CMARK , "%s %d", "DEBUG", x++);
  clog(info, CFATAL, "%s %d", "DEBUG", x++);
  clog(info, CERROR, "%s %d", "DEBUG", x++);
  clog(info, CWARN , "%s %d", "DEBUG", x++);
  clog(info, CDEBUG, "%s %d", "DEBUG", x++);
  clog(info, CTRACE, "%s %d", "DEBUG", x++);

  clog_setfilelevel(info, clog_getlevel("TRACE"));

  clog(info, CMARK , "%s %d", "TRACE", x++);
  clog(info, CFATAL, "%s %d", "TRACE", x++);
  clog(info, CERROR, "%s %d", "TRACE", x++);
  clog(info, CWARN , "%s %d", "TRACE", x++);
  clog(info, CDEBUG, "%s %d", "TRACE", x++);
  clog(info, CTRACE, "%s %d", "TRACE", x++);

  clog_close(info);
  info=NULL;

  clog(info, CTRACE, "testing %s ", "one");

  // callback tests
  info = clog_open(NULL, 0, callback, clog_getlevel("MARK"));

  clog(info, CMARK , "%s", PACKAGE);

  clog(info, CMARK , "%s %d", "MARK", x++);
  clog(info, CFATAL, "%s %d", "MARK", x++);
  clog(info, CERROR, "%s %d", "MARK", x++);
  clog(info, CWARN , "%s %d", "MARK", x++);
  clog(info, CDEBUG, "%s %d", "MARK", x++);
  clog(info, CTRACE, "%s %d", "MARK", x++);

  clog_setcallbacklevel(info, clog_getlevel("FATAL"));

  clog(info, CMARK , "%s %d", "FATAL", x++);
  clog(info, CFATAL, "%s %d", "FATAL", x++);
  clog(info, CERROR, "%s %d", "FATAL", x++);
  clog(info, CWARN , "%s %d", "FATAL", x++);
  clog(info, CDEBUG, "%s %d", "FATAL", x++);
  clog(info, CTRACE, "%s %d", "FATAL", x++);

  clog_setcallbacklevel(info, clog_getlevel("ERROR"));

  clog(info, CMARK , "%s %d", "ERROR", x++);
  clog(info, CFATAL, "%s %d", "ERROR", x++);
  clog(info, CERROR, "%s %d", "ERROR", x++);
  clog(info, CWARN , "%s %d", "ERROR", x++);
  clog(info, CDEBUG, "%s %d", "ERROR", x++);
  clog(info, CTRACE, "%s %d", "ERROR", x++);

  clog_setcallbacklevel(info, clog_getlevel("WARN"));

  clog(info, CMARK , "%s %d", "WARN", x++);
  clog(info, CFATAL, "%s %d", "WARN", x++);
  clog(info, CERROR, "%s %d", "WARN", x++);
  clog(info, CWARN , "%s %d", "WARN", x++);
  clog(info, CDEBUG, "%s %d", "WARN", x++);
  clog(info, CTRACE, "%s %d", "WARN", x++);

  clog_setcallbacklevel(info, clog_getlevel("DEBUG"));

  clog(info, CMARK , "%s %d", "DEBUG", x++);
  clog(info, CFATAL, "%s %d", "DEBUG", x++);
  clog(info, CERROR, "%s %d", "DEBUG", x++);
  clog(info, CWARN , "%s %d", "DEBUG", x++);
  clog(info, CDEBUG, "%s %d", "DEBUG", x++);
  clog(info, CTRACE, "%s %d", "DEBUG", x++);

  clog_setcallbacklevel(info, clog_getlevel("TRACE"));

  clog(info, CMARK , "%s %d", "TRACE", x++);
  clog(info, CFATAL, "%s %d", "TRACE", x++);
  clog(info, CERROR, "%s %d", "TRACE", x++);
  clog(info, CWARN , "%s %d", "TRACE", x++);
  clog(info, CDEBUG, "%s %d", "TRACE", x++);
  clog(info, CTRACE, "%s %d", "TRACE", x++);
  clog(info, CTRACE, "%s %d", "TRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACETRACE", x++);

  clog_close(info);
  info=NULL;

  clog(info, CTRACE, "testing %s ", "one");

  // test both

  // callback tests
  info = clog_open("test.log", clog_getlevel("MARK"), callback, clog_getlevel("MARK"));

  clog(info, CMARK , "%s", PACKAGE);

  clog(info, CMARK , "%s %d", "MARK", x++);
  clog(info, CFATAL, "%s %d", "MARK", x++);
  clog(info, CERROR, "%s %d", "MARK", x++);
  clog(info, CWARN , "%s %d", "MARK", x++);
  clog(info, CDEBUG, "%s %d", "MARK", x++);
  clog(info, CTRACE, "%s %d", "MARK", x++);

  clog_setfilelevel(info, clog_getlevel("FATAL"));
  clog_setcallbacklevel(info, clog_getlevel("FATAL"));

  clog(info, CMARK , "%s %d", "FATAL", x++);
  clog(info, CFATAL, "%s %d", "FATAL", x++);
  clog(info, CERROR, "%s %d", "FATAL", x++);
  clog(info, CWARN , "%s %d", "FATAL", x++);
  clog(info, CDEBUG, "%s %d", "FATAL", x++);
  clog(info, CTRACE, "%s %d", "FATAL", x++);

  clog_setfilelevel(info, clog_getlevel("ERROR"));
  clog_setcallbacklevel(info, clog_getlevel("ERROR"));

  clog(info, CMARK , "%s %d", "ERROR", x++);
  clog(info, CFATAL, "%s %d", "ERROR", x++);
  clog(info, CERROR, "%s %d", "ERROR", x++);
  clog(info, CWARN , "%s %d", "ERROR", x++);
  clog(info, CDEBUG, "%s %d", "ERROR", x++);
  clog(info, CTRACE, "%s %d", "ERROR", x++);

  clog_setfilelevel(info, clog_getlevel("WARN"));
  clog_setcallbacklevel(info, clog_getlevel("WARN"));

  clog(info, CMARK , "%s %d", "WARN", x++);
  clog(info, CFATAL, "%s %d", "WARN", x++);
  clog(info, CERROR, "%s %d", "WARN", x++);
  clog(info, CWARN , "%s %d", "WARN", x++);
  clog(info, CDEBUG, "%s %d", "WARN", x++);
  clog(info, CTRACE, "%s %d", "WARN", x++);

  clog_setfilelevel(info, clog_getlevel("DEBUG"));
  clog_setcallbacklevel(info, clog_getlevel("DEBUG"));

  clog(info, CMARK , "%s %d", "DEBUG", x++);
  clog(info, CFATAL, "%s %d", "DEBUG", x++);
  clog(info, CERROR, "%s %d", "DEBUG", x++);
  clog(info, CWARN , "%s %d", "DEBUG", x++);
  clog(info, CDEBUG, "%s %d", "DEBUG", x++);
  clog(info, CTRACE, "%s %d", "DEBUG", x++);

  clog_setfilelevel(info, clog_getlevel("TRACE"));
  clog_setcallbacklevel(info, clog_getlevel("TRACE"));

  clog_setcallback(info, NULL);

  clog(info, CMARK , "%s %d", "TRACE", x++);
  clog(info, CFATAL, "%s %d", "TRACE", x++);
  clog(info, CERROR, "%s %d", "TRACE", x++);
  clog(info, CWARN , "%s %d", "TRACE", x++);
  clog(info, CDEBUG, "%s %d", "TRACE", x++);
  clog(info, CTRACE, "%s %d", "TRACE", x++);

  clog_close(info);
  info=NULL;

  clog(info, CTRACE, "testing %s ", "one");


  return EXIT_SUCCESS;
}
