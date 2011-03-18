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

#include <stdlib.h> // malloc()
#include <string.h> // memcpy()

#include "csocket.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// returns 0 for success or -1 for failure
int csocket_gethostbyname(CLOG_INFO* log, CSOCKET* sock, char *server)
{
	clog(log, CTRACE, "csocket_gethostbyname(): checking parameters");

  if(NULL==server) {
  	clog(log, CERROR, "csocket_gethostbyname(): server was NULL");
    return -1;
  }

	if(NULL==sock) {
  	clog(log, CERROR, "csocket_gethostbyname(): sock was NULL");
    return -1;
  }

  clog(log, CTRACE, "csocket_gethostbyname(): searching for %s", server);

	sock->hp = gethostbyname(server);
  if(NULL==sock->hp) {
#if defined(WIN32) || defined(SOLARIS)
    clog(log, CERROR, "csocket_gethostbyname()::gethostbyname(%s): %d", server, h_errno);
#else
    clog(log, CERROR, "csocket_gethostbyname()::gethostbyname(%s): %s", server, hstrerror(h_errno));
#endif
    return -1;
  }

  clog(log, CTRACE, "csocket_gethostbyname(): copying data from static src");
	sock->host_address_type = sock->hp->h_addrtype;
  sock->host_address_length = sock->hp->h_length;
  sock->host_address = (char*) malloc(sock->host_address_length);
  memcpy(sock->host_address, sock->hp->h_addr, sock->hp->h_length);
  clog(log, CDEBUG, "csocket_gethostbyname(): success, returning");
  return 0;
}
