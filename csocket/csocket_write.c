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

#include "csocket.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// writes data to the socket, returns number of bytes written
int csocket_write(CLOG_INFO* log, CSOCKET* sock, const char* buf, int size)
{
	size_t byteswritten = 0;
  size_t bytesleft = 0;
	const char *ptr = NULL;

  clog(log, CTRACE, "csocket_write(): starting data write");

  if(NULL==buf) {
    clog(log, CTRACE, "csocket_write(): buffer was NULL");
    return -1;
  }

  // init some vars
  bytesleft = size;
  ptr = buf;

  // write all data to socket
  while(0<bytesleft) {
  	byteswritten = send(sock->socket, ptr, bytesleft, 0);
    // check for error
		if(0>=byteswritten) {
			if(EINTR==errno) {
      	byteswritten = 0;
				continue;
      }
			else {
        clog_perror(log, CERROR, "csocket_write():send()");
	      return -1;
      }
    }
    bytesleft -= byteswritten;
    ptr += byteswritten;
  }

  clog(log, CDEBUG, "csocket_write(): wrote %d bytes", size);

  return size;
}
