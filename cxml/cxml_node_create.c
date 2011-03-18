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

CXMLNODE* cxml_node_create(CLOG_INFO* info, CXMLNODE *node, char *name, int len)
{
  CXMLNODE *retval=NULL;

  // we NEED a name. =)
  if(NULL==name || 1>len) {
    return NULL;
  }

  retval = cxml_node_new(info);

  retval->name = cstring_import(name, len);

  // set the new node as a child of another..
  if(NULL!=node) {
    cxml_node_addnode(info, node, retval);
  }

  return retval;
}

#ifndef NOTEST
void Test__cxml_node_create(CuTest* tc)
{
  CXMLNODE *node = NULL;
  CXMLNODE *parent = NULL;
  dnode_t * dnode= NULL;

  // pass a NULL name and big len
  node = cxml_node_create(NULL, NULL, NULL, 100);
  CuAssertPtrEquals(tc, NULL, node);

  // pass a valid name and 0 len
  node = cxml_node_create(NULL, NULL, "test", 0);
  CuAssertPtrEquals(tc, NULL, node);

  // check the name of the node
  parent = cxml_node_create(NULL, NULL, "parent", 6);
  CuAssertStrEquals(tc, "parent", parent->name->string);

  // check that the node is being created as a subnode of a parent
  node = cxml_node_create(NULL, parent, "test", 4);
  dnode = dict_last(parent->sub);
  CuAssertPtrEquals(tc, node, (CXMLNODE*)dnode->dict_data);

  cxml_node_free(NULL, &parent);
}
#endif // NOTEST

