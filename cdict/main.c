/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Tue Feb 18 11:38:32 GMT 2003
    copyright            : (C) 2003 by Ben Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int main(int argc, char *argv[])
{
  printf("Hello, world!\n");

  return EXIT_SUCCESS;
}
