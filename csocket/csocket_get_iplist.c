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

int csocket_get_iplist(CLOG_INFO* log, CSOCKET* sock)
{
#ifndef WIN32
  struct ifreq ifr;
  struct sockaddr_in *ip_addr;
  int     s;

  struct in_addr addr;
  struct if_nameindex * ifs;
  char *  ifname=NULL;
  int i = 0;

  s = socket(PF_INET, SOCK_DGRAM, 0);
  if (s == -1)
  {
    clog_perror(log, CERROR, "csocket_get_iplist");
    return -1;
  }

  // get a list of the interface names
  ifs = if_nameindex();

  // loop through the interface names getting the IP for each
  for (i = 0 ; NULL!=ifs[i].if_name; i++) {
    ifname = ifs[i].if_name;
    clog(log, CDEBUG, "csocket: checking interface %s for IP addresses", ifname);

    // copy the interface name to the ifreq structure
    strcpy(ifr.ifr_name, ifname);

    // try to get an address from the interface
    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
    {
      // this one didn't work out, try the next one
      clog(log, CDEBUG, "csocket:  - Was not able to get an IP address");
      continue;
    }

    // fill the sockaddr_in structure
    ip_addr = (struct sockaddr_in *) &ifr.ifr_broadaddr;

    // get the IP address (in_addr structure)
    addr = ip_addr->sin_addr;

    // save the IP address to the list
    sock->iplist = realloc(sock->iplist, sizeof(char*) * (sock->ipcount+2));
    sock->iplist[sock->ipcount]=strdup(inet_ntoa(addr));
    sock->iplist[sock->ipcount+1]=NULL;
    ++sock->ipcount;
  }

  // free the list of interface names
  if_freenameindex(ifs);
  ifs=NULL;

  close(s);

#endif

  return sock->ipcount;
}
