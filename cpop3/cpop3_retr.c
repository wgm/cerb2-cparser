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

#include <cfile.h>

#include "cpop3.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#define CPOP3_RETR_BUFSIZE 2048

void cpop3_retr_clean(char *string, int size)
{
  int x=0;
  if(NULL==string || 0==size) {
    return;
  }

  for(x=0; x<size; x++) {
    if(0==string[x]) {
      string[x]=' ';
    }
  }
}


char* cpop3_retr(CLOG_INFO* log, CPOP3* pop3, char *tmppath)
{
  int *vp = NULL;
  CFILE* file = NULL;
  char *rval = NULL;
  int size = 0;
  int slen = 0;
  char msg[CPOP3_RETR_BUFSIZE] = {0};
  int msglen = 0;
  char *pmsg=msg;
  char *sendbuf = NULL;

  if(NULL==pop3 || NULL==pop3->list) {
    clog(log, CERROR, "cpop3_retr(): pop3 (%p) or pop3->list was NULL", pop3);
    return NULL;
  }

  if(0==linkedlist_size(pop3->list)) {
    clog(log, CDEBUG, "cpop3_retr(); no message to download");
    return NULL;
  }

  vp = (int*) linkedlist_next(pop3->list);

  if(NULL==vp) {
    clog(log, CDEBUG, "cpop3_retr(): there was not a message number to RETR");
    return NULL;
  }

  pop3->last = *vp;

  clog(log, CTRACE, "cpop3_retr(): getting message %d", pop3->last);

  snprintf(msg, CPOP3_RETR_BUFSIZE-1, "%d", pop3->last);
  slen = strlen(msg);

  sendbuf = (char*) malloc(7 + slen + 1);

  if(NULL==sendbuf) {
    clog(log, CERROR, "cpop3_retr(): malloc returned NULL, could not allocate memory");
    return NULL;
  }

  memset(sendbuf, 0, (8 + slen));
  memcpy(sendbuf, "RETR ", 5);
  memcpy(sendbuf + strlen(sendbuf), msg, slen);
  memcpy(sendbuf + strlen(sendbuf), "\r\n", 2);
  slen = strlen(sendbuf);

  if(slen == csocket_write(log, pop3->sock, sendbuf, slen)) {
    memset(msg, 0, CPOP3_RETR_BUFSIZE);
    while(NULL==strstr(msg, "\n") && msglen<CPOP3_RETR_BUFSIZE-1) {
      size=csocket_read(log, pop3->sock, pmsg+msglen, CPOP3_RETR_BUFSIZE-msglen);
      cpop3_retr_clean(pmsg+msglen, size);
      if(0>size) {
        // ERROR!
        size = -1;
        break;
      }
      if(0==size) {
        break;
      }
      msglen += size;
    }
    if(0<=size) {
      size = cpop3_iserror(log, (char*)msg);
    }
    if(0==size) {
      char * posa=NULL;

      // get number of messages
      sscanf(msg, "+OK %d", &size);
      clog(log, CDEBUG, "cpop3_retr(): %d bytes", size);

      file = cfile_mkstemp(&(pop3->cfsys), tmppath);
      if(NULL==file) {
        clog(log, CERROR, "cpop3_retr(): could not open temp file for saving message");
        return NULL;
      }

      posa = cpop3_nextline(msg);

      msglen = msglen - (posa - msg);
      memmove(msg, posa, msglen);
      msg[msglen] = 0;

      for(;;) {
        char buf[CPOP3_RETR_BUFSIZE]={0};

        posa = msg;
        // this loop will un double dot lines that begin with dot
        while(NULL!=(posa=(char*)strstr(posa, "\n."))) {
          if(2<strlen(posa)) { // make sure there's enough chars to test with
            posa+=1; // sit on the dot
            // make sure this isn't the email terminator LF.CR
            if('\r'!=*(posa+1)) { // if there is not CR following the dot
              // shift the data on top of the dot
              memcpy(posa, posa+1, msglen - (posa - msg) - 1);
              msglen -= 1;
              msg[msglen]=0;
            }
            else {
              posa-=1; // go back to beginning of LF.CR
              break;
            }
          }
          else {
            break;
          }
        }

        // check for termination char
        if(NULL!=posa
            && '\n'==*(posa)
            && '.' ==*(posa+1)
            && '\r'==*(posa+2)) {
          *(posa+1)=0; // cap it. :-)
// TODO: figure out the proper length to remove
//          msglen -= 2;
          cfile_write(&file, msg, msglen);
          memset((char*)msg, 0, CPOP3_RETR_BUFSIZE); // clean up the memory
          break;
        }

        if(0 < msglen - 4) {
          cfile_write(&file, msg, msglen-4);
           memcpy(msg, msg+msglen-4, 4);
           msglen=4;
           msg[msglen]=0;

          slen = csocket_read(log, pop3->sock, (char*)buf, CPOP3_RETR_BUFSIZE-16 );
          if(-1==slen) {
            // if we got this error then we assume the message is finished.
            break;
          }
          cpop3_retr_clean(buf, slen);
          memcpy((char*)msg+4, (char*)buf, slen);
          msglen+=slen;
          msg[msglen]=0;
          memset((char*)buf, 0, CPOP3_RETR_BUFSIZE-16); // clean up the memory
        }
        else {
          // no bytes in buffer, fill it!
          msglen = csocket_read(log, pop3->sock, (char*)msg, CPOP3_RETR_BUFSIZE );
          if(-1==msglen) {
            // if we got this error then we assume the message is finished.
            break;
          }
          cpop3_retr_clean(msg, msglen);
          msg[msglen]=0;
        }
      }

      rval = (char*) malloc(strlen(file->filename)+1);
      memcpy(rval, file->filename, strlen(file->filename)+1);
      cfile_close(&file);
      cfile_free(&file);
    }
    clog(log, CDEBUG, "cpop3_retr(): finished getting message %d", pop3->last);
  }
  else {
    clog(log, CERROR, "cpop3_retr(): could not write all chars to server");
    rval = NULL;
  }

  memset(sendbuf, 0, strlen(sendbuf));
  free(sendbuf);
  sendbuf = NULL;

  return rval;
}

