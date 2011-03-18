#include <stdio.h>
#include <stdlib.h>
#ifndef BSD
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#endif

#include "rsa.h"

int main ( int argc, char **argv )
{
  int size=0;
  int index=0;
  unsigned int data[2048] = { 0 };
#ifndef BSD
  int skfd;
  struct ifreq ifr;
  char a[]="00ffffffc034ffffff9f7749";
  char b[128]={0};
  struct stat sbuf;
  char *hostname=NULL;
  
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


  while(!feof(stdin)) {
    size += fread(data+index, 8, 1, stdin);
    index+=2;
  }

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
