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

#include <string.h>

#include "csocket.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#ifdef WIN32
#include <winsock.h>
#endif

// -1 = error, 0 = success
int csocket_winsock_start(CLOG_INFO* log)
{
#ifdef WIN32
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;

  clog(log, CDEBUG, "csocket_winsock_start(): doing WIN32 socket init");

  wVersionRequested = MAKEWORD( 2, 2 );

  err = WSAStartup( wVersionRequested, &wsaData );
  if ( err != 0 ) {
    clog( log, CFATAL, "csocket_winsock_start(): WinSock version 2.2 not available");
    return -1;
  }

  if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
    clog( log, CFATAL, "csocket_winsock_start(): WinSock version 2.2 not available");
    WSACleanup( );
    return -1;
  }
#endif
	// we made it!
	return 0;
}
