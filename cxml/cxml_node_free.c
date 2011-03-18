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

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif
/*
void cxml_node_free(CLOG_INFO* info, void * data) {
  CXMLNODE *node = (CXMLNODE*) data;
*/
void cxml_node_free(CLOG_INFO* info, CXMLNODE **node) {
  dnode_t *dn = NULL;

  if(NULL==node || NULL==*node) {
    return;
  }

  clog( info, CTRACE, "XML: xml_node_free(), free the attributes");
  // free the attributes
  if(NULL!=(*node)->att) {
    if(!dict_isempty((*node)->att)) {
      for (dn = dict_first((*node)->att); dn; dn = dict_next((*node)->att, dn)) {
        char *key=(char*)dnode_getkey(dn);
        char *data=(char*)dnode_get(dn);
        if(NULL!=key) free(key); key=NULL;
        if(NULL!=data) free(data); data=NULL;
      }
    }
    dict_free_nodes((*node)->att);
    dict_destroy((*node)->att);
    (*node)->att=NULL;
  }

  clog( info, CTRACE, "XML: xml_node_free(), free the name");
  // free the name
  if(NULL!=(*node)->name) cstring_free(&((*node)->name));

  clog( info, CTRACE, "XML: xml_node_free(), free the data");
  // free the data
  if(NULL!=(*node)->data) cstring_free(&((*node)->data));

  clog( info, CTRACE, "XML: xml_node_free(), free the subs");
  // free the children
  if(NULL!=(*node)->sub) {
    if(!dict_isempty((*node)->sub)) {
      for (dn = dict_first((*node)->sub); dn; dn = dict_next((*node)->sub, dn)) {
        char *key=(char*)dnode_getkey(dn);
        CXMLNODE *data=(CXMLNODE*)dnode_get(dn);
        if(NULL!=key) free(key); key=NULL;
        cxml_node_free(info, &data);
      }
    }
    dict_free_nodes((*node)->sub);
    dict_destroy((*node)->sub);
    (*node)->sub=NULL;
  }

  free((*node));
  (*node)=NULL;
  node=NULL;
}

#ifndef NOTEST
void Test__cxml_node_free(CuTest* tc)
{
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

  // free a
  cxml_node_free(NULL, &a);

  // make sure a is NULL
  CuAssertPtrEquals(NULL, NULL, a);
}
#endif // NOTEST

