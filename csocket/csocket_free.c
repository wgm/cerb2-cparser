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

#include <stdlib.h>
#include <string.h>

#include "csocket.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

// free's the memory malloc'd
void csocket_free(CLOG_INFO* log, CSOCKET *sock)
{
  int x = 0;

  clog(log, CTRACE, "csocket_free(): free'ing malloc'd socket memory");
  if(NULL!=sock) {
    // clean up the memory
    sock->socket = 0;
    sock->type = 0;
    // free the host_address working area
    if(NULL!=sock->host_address) {
      if(0<sock->host_address_length) {
        memset(sock->host_address, 0, sock->host_address_length);
      }
      free(sock->host_address);
      sock->host_address=NULL;
    }
    // blank the memory for this struct
    sock->hp = NULL;
    //memset(&sock->hp, 0, sizeof(struct hostent));
    // blank the memory for this struct
    memset(&sock->sa, 0, sizeof(struct sockaddr_in));

    // free the IP list
    if(NULL!=sock->iplist) {
      for(x=0; NULL!=sock->iplist[x]; x++) {
        free(sock->iplist[x]);
        sock->iplist[x]=NULL;
      }
      free(sock->iplist);
      sock->iplist=NULL;
    }

    // free the struct
    free(sock);
    sock=NULL;
  }
  clog(log, CTRACE, "csocket_free(): finished free'ing malloc'd socket memory");
}
