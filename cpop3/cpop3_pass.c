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


#include <string.h>
#include <stdlib.h>

#include "cpop3.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// -1 = bad, 0 = ok
int cpop3_pass(CLOG_INFO* log, CPOP3* pop3, char *pass)
{
  char msg[1024]={0};
  char *pmsg=msg;
  int plen=0;
  int length=0;
  int rval = 0;
  char *sendbuf = NULL;

  clog(log, CTRACE, "cpop3_pass(): sending password");

  if(NULL==pop3 || NULL==pass) {
    clog(log, CERROR, "cpop3_pass(): pop3 struct or pass was NULL");
    return -1;
  }

  length = strlen(pass);

	sendbuf = (char*) malloc(8 + length);

	if(NULL==sendbuf) {
  	clog(log, CERROR, "cpop3_pass(): malloc returned NULL, could not allocate memory");
    return -1;
  }

  memset(sendbuf, 0, (8 + length));
  memcpy(sendbuf, "PASS ", 5);
  memcpy(sendbuf + strlen(sendbuf), pass, length);
	memcpy(sendbuf + strlen(sendbuf), "\r\n", 2);
  length = strlen(sendbuf);

  if(length == csocket_write(log, pop3->sock, sendbuf, length)) {
    while(NULL==strstr(msg, "\n") && plen<1023) {
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
    }
    clog(log, CDEBUG, "cpop3_pass(): finished sending password");
  }
  else {
    clog(log, CERROR, "cpop3_pass(): could not write all chars to server");
    rval = -1;
  }

  memset(sendbuf, 0, strlen(sendbuf));
  free(sendbuf);
	sendbuf = NULL;

  return rval;
}

