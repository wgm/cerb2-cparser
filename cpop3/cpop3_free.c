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

#include "cpop3.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int cpop3_free(CLOG_INFO* log, CPOP3** ppop3)
{
  int *vp=NULL;
  CPOP3*  pop3=NULL;
  clog(log, CTRACE, "cpop3_free(): free'ing pop3 structure");

  if(NULL==ppop3 || NULL==*ppop3) {
    clog(log, CDEBUG, "cpop3_free(): failed, was passed a NULL ptr");
    return -1;
  }

  // get POP3 data to free
  pop3 = *ppop3;

  csocket_free(log, pop3->sock);

  linkedlist_iterate(pop3->list);
  while(NULL!=(vp=linkedlist_remove_last(pop3->list))) {
    free(vp);
  }

  if(NULL!=pop3->user) free(pop3->user);
  pop3->user = NULL;

  if(NULL!=pop3->pass) free(pop3->pass);
  pop3->pass = NULL;

  linkedlist_del(pop3->list, free);
  pop3->list = NULL;

  free(pop3);
  pop3=NULL;

  clog(log, CTRACE, "cpop3_free(): finished free'ing cpop3 structure");
  return 1;
}
