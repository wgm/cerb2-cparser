/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Wed Jan 15 19:11:56 GMT 2003
    copyright            : (C) 2003 by Ben Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include <cstring.h>
#include <stdio.h>
#include <stdlib.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int main(int argc, char *argv[])
{
  XMLROOT *root = NULL;
  XMLNODE *node = NULL;
  struct cstring *xml_string=NULL;
  CLOG_INFO *log = NULL;

  printf("cXML test program\n");

  log = clog_open("test.log", clog_getlevel("TRACE"),NULL,0);

  root = xml_root_new(log);
  if(0==xml_fread(log, root, argv[1])) {
    fprintf(stderr, "Something was wrong with the XML File, please check the log!\n");
    xml_root_free(log, root);
    exit(1);
  }

  xml_string = xml_node_tostring(log, root->data);
  if(NULL!=xml_string) {
    fprintf(stderr, "%s\n", xml_string->string);
    cstring_free(&xml_string);
  }

  xml_root_free(log, root);


  { // test xml creation functionality..
    XMLNODE* tmpx = NULL;
    XMLNODE* tmpy = NULL;
    node = xml_node_create(log, NULL, "tickets_container", 17);
    xml_node_addattribute(log, node, "bob", "3");
    tmpx = xml_node_create(log, node, "ticket", 6);
    tmpy = xml_node_create(log, tmpx, "id", 2);
    xml_node_adddata(log, tmpy, "3", 1);

    xml_string = xml_node_tostring(log, node);

    if(0!=xml_string->length) {
      fprintf(stderr, "%s\n", xml_string->string);
      cstring_free(&xml_string);
    }

    xml_node_free(log, node);
  }

  clog_close(log);

  return EXIT_SUCCESS;
}
