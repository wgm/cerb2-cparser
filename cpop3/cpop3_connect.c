/* cPOP3 - library for the POP3 protocol
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

#include "cpop3.h"

#include <string.h>

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// 0 = OK, -1 = error
int cpop3_connect(CLOG_INFO* log, CPOP3* pop3)
{
  char msg[1024] = {0};
  char *pmsg=msg;
  int plen = 0;
  int rval = 0;

  clog(log, CTRACE, "cpop3_connect(): connecting");

  if(0==csocket_connect(log, pop3->sock)) {
    clog(log, CDEBUG, "cpop3_connect(): connection was successful");
    while(NULL==strstr(msg, "\n") && plen<1023) {
    	rval = csocket_read(log, pop3->sock, pmsg+plen, 1023-plen);
      if(0>rval) {
      	// ERROR!
        rval = -1;
        break;
      }
      if(0==rval) {
      	break;
      }
      plen += rval;
    }
    if(0<=rval) {
    	rval = cpop3_iserror(log, (char*)msg);
    }
  }
  else {
    clog(log, CERROR, "cpop3_connect(): failed to connect");
    rval = -1;
  }

  return rval;
}
