/* cSocket - library for connecting to TCP/IP servers.
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
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#endif

#include "csocket.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// reads data from the socket, returns number of bytes read
int csocket_read(CLOG_INFO* log, CSOCKET* sock, char* buf, int size)
{
  int             select_ret=0;
  int              bytes=0;
  fd_set          fdin;
  struct timeval   timeout;

  clog(log, CTRACE, "csocket_read(): starting data read");

  if(NULL==buf) {
    clog(log, CTRACE, "csocket_read(): buffer was NULL");
    return -1;
  }

  FD_ZERO(&fdin);
  timeout.tv_sec = sock->read_timeout;
  timeout.tv_usec = 0;

  for(;;) {
    FD_SET(sock->socket, &fdin);
    select_ret = select(sock->socket+1, &fdin, NULL, NULL, &timeout);
    if(-1==select_ret) {
      clog_perror(log, CERROR, "csocket_read()::select():");
    }
    else if(0==select_ret) {
      clog(log, CWARN, "csocket_read()::select timed out");

      // return -1 so that we know there was some sort of error
      return -1;
    }
    else if(0<select_ret) {
      clog(log, CDEBUG, "csocket_read()::file descriptor ready for reading");

      bytes = recv(sock->socket, buf, size, 0);
      if(0>bytes) {
        if(EINTR==errno) {
          // recv was interupted, continue reading.
          continue;
        }
        clog_perror(log, CERROR, "csocket_read():read()");
        return -1;
      }
      else {
        // have some bytes and there was not an error, get out of for loop
        break;
      }
    }
  }

  // cap the end so we can log it.
  buf[bytes]=0;

  clog(log, CDEBUG, "csocket_read(): read %d bytes", bytes);

  // [BGH]: This can cause crashes sometimes..
  // clog(log, CTRACE, "csocket_read(): data read:\n%s", buf);

  return bytes;
}
