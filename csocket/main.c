/*
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

#include <stdlib.h>
#include <string.h>
 
#include <cstring.h>
 
#include "csocket.h"

int main(int argc, char ** argv)
{
  int bytes = 0;
  CSOCKET* sock=NULL;
  CLOG_INFO* log=NULL;
  CSTRING* buf = NULL;

  // open the logfile
  log = clog_open("./test_csocket.log", CTRACE, NULL, 0);

  if(3>argc) {
    clog(log, CFATAL, "USAGE: %s host user password", argv[0]);
    clog_close(log);
    return 1;
  }
  
  // create the new socket  
  sock = csocket_new(log);

  // initialize the socket
  csocket_init(log, sock, argv[1], 110);

  // connect
  csocket_connect(log, sock);

  // read/write
  buf = cstring_new(1024);
  bytes=csocket_read(log, sock, buf->string, 1024);
  fprintf(stdout, "%s", buf->string);

  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  cstring_strcat_imp(buf, "USER ", 5);
  cstring_strcat_imp(buf, argv[2], strlen(argv[2]));  
  cstring_strcat_imp(buf, "\r\n", 2);
  csocket_write(log, sock, buf->string, buf->length);
  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  bytes=csocket_read(log, sock, buf->string, 1024);
  fprintf(stdout, "%s", buf->string);

  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  cstring_strcat_imp(buf, "PASS ", 5);
  cstring_strcat_imp(buf, argv[3], strlen(argv[3]));
  cstring_strcat_imp(buf, "\r\n", 2);
  csocket_write(log, sock, buf->string, buf->length);
  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  bytes=csocket_read(log, sock, buf->string, 1024);
  fprintf(stdout, "%s", buf->string);

  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  cstring_strcat_imp(buf, "STAT\r\n", 6);
  csocket_write(log, sock, buf->string, buf->length);
  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  bytes=csocket_read(log, sock, buf->string, 1024);
  fprintf(stdout, "%s", buf->string);

  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  cstring_strcat_imp(buf, "QUIT\r\n", 6);
  csocket_write(log, sock, buf->string, buf->length);
  buf->length=0;
  memset(buf->string, 0, buf->memsize);
  bytes=csocket_read(log, sock, buf->string, 1024);
  fprintf(stdout, "%s", buf->string);

  cstring_free(&buf);

  // close the connection
  csocket_close(log, sock);

  // free the socket
  csocket_free(log, sock);

  // close the log
  clog_close(log);
  
  return 0;
}
