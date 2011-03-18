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

// -1 = failure, 0 = success
int csocket_set_timeout(CLOG_INFO* log, CSOCKET* sock, int timeout)
{
  clog(log, CTRACE, "csocket_set_timeout(): checking parameters");

  // check parameters
  if(NULL==sock) {
    clog(log, CERROR, "csocket_set_timeout(): sock was NULL");
    return -1;
  }

  if(0>timeout) {
    clog(log, CWARN, "csocket_set_timeout(): timeout was negative, returning");
    return -1;
  }

  sock->read_timeout=timeout;

  clog(log, CDEBUG, "csocket_set_timeout(): timeout set to %d", sock->read_timeout);

  return 0;
}
