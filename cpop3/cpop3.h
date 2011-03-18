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

#ifndef __CPOP3_H__
#define __CPOP3_H__

#include <cdata.h>
#include <clog.h>
#include <csocket.h>
#include <cfile.h>

typedef struct _CPOP3 {
  CSOCKET*           sock;
  struct linkedlist* list;
  int                last;
  // pop3 storage values
  char*              user;
  char*              pass;
  int                dele;
  CFSYS*             cfsys;
} CPOP3;

extern int    cpop3_connect(CLOG_INFO* log, CPOP3* pop3);
extern int    cpop3_dele(CLOG_INFO* log, CPOP3* pop3);
extern int    cpop3_disconnect(CLOG_INFO* log, CPOP3* pop3);
extern int     cpop3_free(CLOG_INFO* log, CPOP3** ppop3);
extern CPOP3* cpop3_init(CLOG_INFO* log, char *server, unsigned short int port, int timeout, CFSYS* cfsys);
extern int    cpop3_iserror(CLOG_INFO* log, char *string);
extern char * cpop3_nextline(char *string);
extern int    cpop3_user(CLOG_INFO* log, CPOP3* pop3, char *user);
extern int    cpop3_pass(CLOG_INFO* log, CPOP3* pop3, char *pass);
extern int    cpop3_quit(CLOG_INFO* log, CPOP3* pop3);
extern char*  cpop3_retr(CLOG_INFO* log, CPOP3* pop3, char *tmppath);
extern int    cpop3_stat(CLOG_INFO* log, CPOP3* pop3);

#endif // __CPOP3_H__
