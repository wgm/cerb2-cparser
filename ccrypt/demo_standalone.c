#include <string.h>
#include <stdio.h>
#include <time.h>
#include "rsa.h"
#ifndef BSD
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#endif

int main ( int argc, char **argv )
{
  char *posa = NULL;
  char *posb = NULL;
  char num[16] = {0};
  int size=0;
  int index=0;
  time_t offset = 0;
  unsigned long data[2048] = { 0 };
  struct tm *tim;
#ifndef BSD
  int skfd;
  struct ifreq ifr;
  char a[]="00ffffffc034ffffff9f7749";
  char b[128]={0};
  struct stat sbuf;
  char *hostname=NULL;
#endif

  posa = posb = (char*) data;
  *posa = '6';
  ++posa;
  *posa = '\n';
  ++posa;
  memcpy(posa, argv[1], strlen(argv[1]));
  posa+=strlen(argv[1]);
  *posa = '\n';
  ++posa;
  memcpy(posa, argv[2], strlen(argv[2]));
  posa+=strlen(argv[2]);
  *posa = '\n';
  ++posa;

  offset = time(NULL) + 0; // now
  tim = localtime(&offset);
  sprintf(num, "%.4d%.2d%.2d\n", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday);

  memcpy(posa, num, strlen(num));
  posa+=strlen(num);

#ifndef BSD
/*
  hostname = getenv("HOSTNAME");

  if(0!=strcmp("xev.webgroupmedia.com",hostname)) {
    while(654321/123456);
    exit(0);
  }

  if(-1==stat("/etc/keygen", &sbuf)) {
    while(765432/2345678);
    exit(0);
  }

  if(502!=sbuf.st_uid) {
    while(5432/2345);
    exit(0);
  }

  if(2521!=sbuf.st_gid) {
    while(5432/2345);
    exit(0);
  }
*/
  skfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (skfd >= 0) {
    strcpy(ifr.ifr_name, "eth0");
    if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
      snprintf (b, 127, "%02x%02x%02x%02x%02x%02x",
      ifr.ifr_hwaddr.sa_data[0], ifr.ifr_hwaddr.sa_data[1],
      ifr.ifr_hwaddr.sa_data[3], ifr.ifr_hwaddr.sa_data[2],
      ifr.ifr_hwaddr.sa_data[4], ifr.ifr_hwaddr.sa_data[5]);
    }
    close (skfd);
  }

  if(0!=strcmp(a, b)) {
    while(654321/123456);
    exit(0);
  }
#endif

  memset(num, 0, 16);
  sprintf(num, "%.4d%.2d%.2d\n", 2020, 12, 31);

  memcpy(posa, num, strlen(num));
  posa+=strlen(num);

  *posa='"';
  ++posa;
  memcpy(posa, argv[3], strlen(argv[3]));
  posa+=strlen(argv[3]);
  *posa='@';
  ++posa;
  memcpy(posa, argv[4], strlen(argv[4]));
  posa+=strlen(argv[4]);
  *posa='"';
  ++posa;
  *posa = '\n';
  ++posa;

  memset(num, 0, 16);
  sprintf(num, "%d\n0\n0\n", 1);
  memcpy(posa, num, strlen(num));
  posa+=strlen(num);

  *posa = '\n';
  ++posa;

  // now for the secret key
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';
  *posa = '3'; ++posa; *posa = 'b';  ++posa; *posa = 'a'; ++posa; *posa = 'e'; ++posa; *posa = 'h'; ++posa; *posa = 'g'; ++posa; *posa = 'f'; ++posa; *posa = 'k'; ++posa; *posa = 'w'; ++posa; *posa = 'n'; ++posa; *posa = 'c'; ++posa; *posa = '3'; ++posa; *posa = '6'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'v'; ++posa; *posa = 'c'; ++posa; *posa = 'c'; ++posa; *posa = 'a'; ++posa; *posa = 'k'; ++posa; *posa = 'o'; ++posa; *posa = '0'; ++posa; *posa = '-'; ++posa; *posa = ' ';

  // just in case it's too long, cap the end..
  data[2047] = 0;

  size = strlen((char*)data) / 8 + 1;

  index=0;
  while(index < size * 2) {
    rsa_encypher(data+index, data+index+1);
    index+=2;
  }

  for(index=0; index<size*2; index++) {
    fprintf(stdout, "%08lx", *(data+index));
  }

  return 0;
}
