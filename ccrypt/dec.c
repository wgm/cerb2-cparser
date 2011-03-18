#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "rsa.h"

int main ( int argc, char **argv )
{
  int size=0;
  int index=0;
  char buf[9] = {0};
  unsigned int data[2048000] = { 0 };
#if defined(KAI)
  int skfd;
  struct ifreq ifr;
  char a[]="00042c23ffffffe07c";
  char b[128]={0};
  struct stat sbuf;
  char *hostname=NULL;

  hostname = getenv("HOSTNAME");

  if(0!=strcmp("kai.webgroupmedia.com",hostname)) {
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
    char minibuf[9]={0};
    int pos = 0;
    char c = 0;
    while(!feof(stdin) &&  pos < 8) {
      c = fgetc(stdin);
      switch (c) {
        case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        case '8': case '9': case '0': case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F':
          minibuf[pos++]=c;
          break;
        default:
          continue;
          break;
      }
    }    

    if(!feof(stdin)) {
      sscanf(minibuf, "%lx", (data+index));
      size += 1;
      ++index;
    }
  }


  for(index=0; index < size; index+=2) {
    rsa_decypher((data+index), (data+index+1));
  }

  data[index] = 0;

  fprintf(stdout, "%s", (char*)data);

  return 0;
}
