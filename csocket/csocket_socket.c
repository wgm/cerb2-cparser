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

// returns 0 for success or -1 for error
int csocket_socket(CLOG_INFO* log, CSOCKET* sock)
{
  clog(log, CTRACE, "csocket_socket(): initializing socket");

  if(NULL==sock) {
		clog(log, CERROR, "csocket_socket(): socket parameter was NULL");
    return -1;
	}

  sock->socket = socket(sock->sa.sin_family, sock->type, 0);

  if(0>sock->socket){
    clog_perror(log, CFATAL, "csocket_socket()::socket()");
    return -1;
  }

  clog(log, CDEBUG, "csocket_socket()::socket() returned %d", sock->socket);

  clog(log, CTRACE, "csocket_socket(): finished initializing socket");

  return 0;
}
