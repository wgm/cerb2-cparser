/* cXML - library for reading / creating XML files/objects
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

#include <stdarg.h>
#include <string.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

/**
  * Pass the node that you want to start on
  * then pass the names of the nodes "configuration", "pop3", "user"
  **/
CXMLNODE *cxml_node_get(CLOG_INFO* info, CXMLNODE *top_node, ...) {
  CXMLNODE *node=top_node;
  int found = 0;
  va_list argp;
  char *p=NULL;

  clog( info, CTRACE, "XML: xml_get_node(), checking the parameters");
  if(NULL==top_node) {
    clog( info, CTRACE, "XML: xml_get_node(), a parameter was bogus (%p)", top_node);
    return NULL;
  }

  clog( info, CTRACE, "XML: xml_get_node(), start to find the node");
  va_start(argp, top_node);

  clog( info, CTRACE, "XML: xml_get_node(), verify the top level node");
  // check the top level node
  if(NULL!=(p=va_arg(argp, char*))) {
    if(0==strcmp(node->name->string,p)){
      clog( info, CTRACE, "XML: xml_get_node(), top level node matched");
    }
    else {
      clog( info, CTRACE, "XML: xml_get_node(), top level node did not match");
      found = -1;
    }
  }
  else {
    clog( info, CTRACE, "XML: xml_get_node(), first argument was NULL");
    found = -1;
  }

  // go find the child node
  clog( info, CTRACE, "XML: xml_get_node(), traverse the nodes");
  while(0==found) {
    clog( info, CTRACE, "XML: xml_get_node(), check that parameter is not NULL");
    if(NULL!=(p=(char*)va_arg(argp, char*))) {
      dnode_t *dn = NULL;

      clog( info, CTRACE, "XML: xml_get_node(), find the correct child of this node");
      if(NULL!=node->sub) {
        dn = dict_lookup(node->sub, p);
        if(NULL==dn) {
          clog( info, CTRACE, "XML: xml_get_node(), child not found");
          found = -1;
        }
        else {
          clog( info, CTRACE, "XML: xml_get_node(), child found");
          node = dnode_get( dn );
        }
      }
      else {
        clog( info, CTRACE, "XML: xml_get_node(), no more children to check");
        found = -1;
      }
    }
    else {
      clog( info, CTRACE, "XML: xml_get_node(), parameter was NULL (must have matched)");
      found = 1;
    }
  }

  clog( info, CTRACE, "XML: xml_get_node(), done traversing the nodes");

  va_end(argp);

  // first level didn't match
  if(-1==found) return NULL;

  clog( info, CTRACE, "XML: xml_get_node(), parameters matched, returning the correct node");

  return node;
}

#ifndef NOTEST
void Test__cxml_node_get(CuTest* tc)
{
  CXMLNODE* node = NULL;
  CXMLNODE* a = NULL;
  CXMLNODE* b = NULL;
  CXMLNODE* c = NULL;
  CXMLNODE* d = NULL;

  // create 4 nodes, a, b, c, d. Put b on a, c on a and d on b.
  a = cxml_node_create(NULL, NULL, "a", 1);
  b = cxml_node_create(NULL, a, "b", 1);
  c = cxml_node_create(NULL, a, "c", 1);
  d = cxml_node_create(NULL, b, "d", 1);

  // make sure they're all not NULL
  CuAssertPtrNotNull(NULL, a);
  CuAssertPtrNotNull(NULL, b);
  CuAssertPtrNotNull(NULL, c);
  CuAssertPtrNotNull(NULL, d);

  // add some data
  cxml_node_adddata(NULL, d, "test", 4);

  // add a attribute
  cxml_node_addattribute(NULL, d, "test", "value");


  // try to get the 'd' node
  node = cxml_node_get(NULL, a, "a","b","d",NULL);
  CuAssertPtrEquals(tc, d, node);

  // try to get the 'e' node (doesn't exist)
  node = cxml_node_get(NULL, a, "a","c","e",NULL);
  CuAssertPtrEquals(tc, NULL, node);

  // free a
  cxml_node_free(NULL, &a);

  // make sure a is NULL
  CuAssertPtrEquals(NULL, NULL, a);
}
#endif // NOTEST


