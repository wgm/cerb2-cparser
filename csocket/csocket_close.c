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

#include <unistd.h>

#include "csocket.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// -1 - error, 0 = ok, close the socket connection
int csocket_close(CLOG_INFO* log, CSOCKET* sock)
{
  clog(log, CTRACE, "csocket_close(): closing socket");
  if(NULL==sock) {
  	clog(log, CERROR, "csocket_close(): sock was NULL");
  	return -1;
  }

  if(sock->socket>0){
    for(;;) {
#if !defined(WIN32)
      if(0==close(sock->socket)) {
#else
      if(0==closesocket(sock->socket)) {
#endif
        clog(log, CDEBUG, "csocket_close(): socket closed");
        sock->socket = 0;
        break;
      }
      else {
      	// error happened
        if(EINTR == errno ){
          clog(log, CTRACE, "csocket_close(): was interupted, trying to close again");
          continue; // continue for loop
        } else { // unknown error
      	  clog_perror(log, CERROR, "csocket_close()::close()");
          sock->socket = -1;
          break;
        }
      }
    }
    // clean up socket info
  }

  return sock->socket;
}
