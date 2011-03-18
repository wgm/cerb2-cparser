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


void cxml_node_delete(CLOG_INFO* info, CXMLNODE **node)
{
  CXMLNODE *parent=NULL;

  if(NULL==node && NULL==*node) {
    clog(info, CWARN, "cxml_node_delete(): node passed was NULL");
    return;
  }

  // find the parent so we can notify of deleted child
  if(NULL!=(*node)->parent) {
    parent = (*node)->parent;
  }

  // if the 'last' in the parent node is this child, set to null
  if(NULL!=parent->last && parent->last == (*node)->me) {
    clog(info, CDEBUG, "cxml_node_delete(): node to be deleted was 'last' in parent, Resetting.");
    parent->last=NULL;
  }

  // if there is a parent, remove from parent before deleting from memory
  if(NULL!=parent && NULL!=(*node)->me) {

    // make sure the parent contains the child!
    if(1==dict_contains(parent->sub, (*node)->me)) {
      // delete the child from the parent
      clog(info, CDEBUG, "cxml_node_delete(): Deleting child from parent.");
      dict_delete(parent->sub, (*node)->me);
    }
  }

  if(NULL!=dnode_getkey((*node)->me)) {
    char * key = NULL;
    key = (char *)dnode_getkey((*node)->me);
    free(key);
  }

  // free the dict node
  dnode_destroy((*node)->me);

  // remove the data
  cxml_node_free(info, node);

  (*node) = NULL;

  return;
}

#ifndef NOTEST
void Test__cxml_node_delete(CuTest* tc)
{
  CXMLNODE *node = NULL;
  CXMLNODE *parent = NULL;
  dnode_t * dnode= NULL;

  // create the parent node
  parent = cxml_node_create(NULL, NULL, "parent", 6);

  // check that the node is being created as a subnode of a parent
  node = cxml_node_create(NULL, parent, "test", 4);
  dnode = dict_last(parent->sub);
  CuAssertPtrEquals(tc, node, (CXMLNODE*)dnode->dict_data);

  // delete the subnode
  cxml_node_delete(NULL, &node);
  CuAssertPtrEquals(tc, NULL, node);

  // check the parent's sub
  CuAssertIntEquals(tc, 0, (int)dict_count(parent->sub));

  // free the parent
  cxml_node_free(NULL, &parent);
}
#endif // NOTEST


