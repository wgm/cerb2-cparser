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

#ifndef __CSOCKET_H__
#define __CSOCKET_H__

#include <clog.h>

#include <sys/types.h>

#if defined(WIN32)
#include <winsock2.h>
#else
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <errno.h>

typedef struct _CSOCKET {
#if !defined(WIN32)
  int                 socket; // the place we read/write
#else
  SOCKET              socket; // win32 socket
#endif
  struct sockaddr_in  sa; // socket address struct
  int                 type; // SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
  struct hostent     *hp; // to convert from FQDN to IP
  char *              host_address;  // info from the HP struct
  int                 host_address_length; // info from the HP struct
  int                 host_address_type; // info from the HP struct
  int                 read_timeout; // the amount of time we wait for a read response
  char **              iplist;  // list of IPs from csocket_get_iplist
  int                  ipcount; // number of IPs returned from csocket_get_iplist
} CSOCKET;

extern int csocket_close(CLOG_INFO* log, CSOCKET* sock);
extern int csocket_connect(CLOG_INFO* log, CSOCKET* sock);
extern void csocket_free(CLOG_INFO* log, CSOCKET *sock);
extern int csocket_get_iplist(CLOG_INFO* log, CSOCKET* sock);
extern int csocket_gethostbyname(CLOG_INFO* log, CSOCKET* sock, char *server);
extern CSOCKET* csocket_new(CLOG_INFO* log);
extern int csocket_read(CLOG_INFO* log, CSOCKET* sock, char* buf, int size);
extern int csocket_set_hptosa(CLOG_INFO* log, CSOCKET* sock);
extern int csocket_set_port(CLOG_INFO* log, CSOCKET* sock, unsigned short int port);
extern int csocket_set_timeout(CLOG_INFO* log, CSOCKET* sock, int timeout);
extern int csocket_set_type(CLOG_INFO* log, CSOCKET* sock, int type);
extern int csocket_socket(CLOG_INFO* log, CSOCKET* sock);
extern int csocket_winsock_start(CLOG_INFO* log);
extern void csocket_winsock_stop(CLOG_INFO* log);
extern int csocket_write(CLOG_INFO* log, CSOCKET* sock, const char* buf, int size);

#endif // __CSOCKET_H__
