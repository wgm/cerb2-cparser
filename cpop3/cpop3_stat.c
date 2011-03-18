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

#include <cstring.h>
 
#include "cpop3.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// -1 = error, 0+ = number of messages
int cpop3_stat(CLOG_INFO* log, CPOP3* pop3)
{
  int rval = 0;
  int size = 0;
  char msg[1024] = {0};
  char *pmsg=msg;
  int plen=0;

  clog(log, CTRACE, "cpop3_stat(): statting");

  if(6==csocket_write(log, pop3->sock, "STAT\r\n", 6)) {
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
      plen+=rval;
    }
    if(0<=rval) {
    	rval = cpop3_iserror(log, (char*)msg);
    }
    if(0==rval) {
      // get number of messages
      sscanf(msg, "+OK %d %d", &rval, &size);
      clog(log, CDEBUG, "cpop3_stat(): %d messages, %d bytes", rval, size);
    }
    clog(log, CDEBUG, "cpop3_stat(): finished statting");
  }
  else {
    clog(log, CERROR, "cpop3_stat(): could not write all chars to server");
    rval = -1;
  }

  // if there are more than 0 messages, get the message number list
  if(0<rval) {
    CSTRING *buf = NULL;
    if(NULL!=(buf = cstring_new(0))) {
      if(6==csocket_write(log, pop3->sock, "LIST\r\n", 6)) {
        // get data until you have the \r\n.\r\n (dot).
        while(NULL==(char*)strstr(buf->string, "\r\n.\r\n")) {
          size = csocket_read(log, pop3->sock, (char*)msg, 1023);
          cstring_strcat_imp(buf, (char*)msg, size);
        }
        if(0==cpop3_iserror(log, buf->string)) {
          // data was returned
          if(0<buf->length) {
            char * line = buf->string;
            while(NULL!=(line = cpop3_nextline(line))) {
              int msgno = 0;
              // end of the input
              if(*line=='.') {
                linkedlist_iterate(pop3->list);
                clog(log, CTRACE, "cpop3_stat(): Found the end of the 'LIST' output.");
                break;
              }
              if(1==sscanf(line, "%d ", &msgno)) {
                int *vp=(int*) malloc(sizeof(int));
                if(NULL==vp) {
                  clog(log, CERROR, "cpop3_stat(): could not malloc memory!");
                }
                else {
                  clog(log, CTRACE, "cpop3_stat(): adding %d to the msgno list", msgno);
                  *vp = msgno;
                  linkedlist_add(pop3->list, vp);
                }
              }
            }
          }
        }
      }
      else {
        clog(log, CERROR, "cpop3_stat(): could not write all chars to server");
      }
      cstring_free(&buf);
    }
    else {
      clog(log, CERROR, "cpop3_stat(): could not allocate memory");
    }
  }

  return rval;  
}
