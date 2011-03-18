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

#include <stdlib.h>
#include <string.h>
 
#include "cpop3.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// 0 = OK, -1 = BAD
int cpop3_dele(CLOG_INFO* log, CPOP3* pop3)
{
  int rval = 0;
  int slen = 0;
  char msg[1024] = {0};
  char *pmsg=msg;
  int plen=0;
  char *sendbuf = NULL;

  if(NULL==pop3) {
    clog(log, CERROR, "cpop3_dele(): pop3 was NULL", pop3);
    return -1;
  }

  if(1>pop3->last) {
    clog(log, CERROR, "cpop3_dele(): called with no 'last' message");
  }

  clog(log, CTRACE, "cpop3_dele(): deleting message %d", pop3->last);

  snprintf(msg, 1024, "%d", pop3->last);
  slen = strlen(msg);

	sendbuf = (char*) malloc(7 + slen + 1);

	if(NULL==sendbuf) {
  	clog(log, CERROR, "cpop3_dele(): malloc returned NULL, could not allocate memory");
    return -1;
  }

  memset(sendbuf, 0, (8 + slen));
  memcpy(sendbuf, "DELE ", 5);
  memcpy(sendbuf + strlen(sendbuf), msg, slen);
	memcpy(sendbuf + strlen(sendbuf), "\r\n", 2);
  slen = strlen(sendbuf);

  if(slen ==csocket_write(log, pop3->sock, sendbuf, slen)) {
    memset(msg, 0, 1024);
    while(NULL==strstr(msg, "\n") && plen<(1023)) {
    	rval=csocket_read(log, pop3->sock, pmsg+plen, 1023-plen);
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
      if(0==rval) {
        clog(log, CDEBUG, "cpop3_dele(): deleted message %d", pop3->last);
      }
      else {
        clog(log, CERROR, "cpop3_dele(): failed to delete message %d", pop3->last);
      }
    }
  }
  else {
    rval = -1;
    clog(log, CERROR, "cpop_dele(): could not send all data");
  }

	memset(sendbuf, 0, strlen(sendbuf));
  free(sendbuf);
	sendbuf = NULL;

  return rval;
}
