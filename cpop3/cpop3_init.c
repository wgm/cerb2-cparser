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

CPOP3* cpop3_init(CLOG_INFO* log, char *server, unsigned short int port, int timeout, CFSYS* cfsys)
{
  int failed = 1;
  CPOP3* pop3 = NULL;

  clog(log, CTRACE, "cpop3_init(): malloc'ing memory for structure");

  pop3 = (CPOP3*) malloc(sizeof(CPOP3));

  if(NULL==pop3) {
    clog_perror(log, CERROR, "cpop3_init():malloc()");
  }
  else {
    memset(pop3, 0, sizeof(CPOP3));
    pop3->user = NULL;
    pop3->pass = NULL;
    pop3->dele = 1;
    pop3->list = linkedlist_new(0);
    pop3->sock = csocket_new(log);
    if(NULL!=pop3->sock) {
      if(0==csocket_winsock_start(log)) {
        if(0==csocket_set_port(log, pop3->sock, port)) {
          if(0==csocket_set_timeout(log, pop3->sock, timeout)) {
            if(0==csocket_gethostbyname(log, pop3->sock, server)) {
              if(0==csocket_set_hptosa(log, pop3->sock)) {
                if(0==csocket_set_type(log, pop3->sock, SOCK_STREAM)) {
                  if(0==csocket_socket(log, pop3->sock)) {
                    failed = 0;
                  }
                }
              }
            }
          }
        }
      }
      if(0==failed) {
        pop3->cfsys = cfsys;
        clog(log, CDEBUG, "cpop3_init(): successful");
        
      }
      else {
        memset(pop3, 0, sizeof(CPOP3));
        free(pop3);
        pop3=NULL;
        clog(log, CERROR, "cpop3_init(): failed to init sockets");
      }
    }
  }
  return pop3;
}
