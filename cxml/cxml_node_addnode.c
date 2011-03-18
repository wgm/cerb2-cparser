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

#include <assert.h>
#include <string.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int cxml_node_addnode(CLOG_INFO* info, CXMLNODE *parent, CXMLNODE *child)
{
  int return_value=0;

  assert(NULL!=parent);
  assert(NULL!=child);

  // if the sub node is NULL create a new sub table
  if(NULL==parent->sub) {
    clog( info, CTRACE, "XML: xml_node_addnode(), creating sub node");
    parent->sub = dict_create(DICTCOUNT_T_MAX, (dict_comp_t)strcmp);
    // allow duplicates
    dict_allow_dupes(parent->sub);
  }

  // if the previous stuff worked, add the name/value pair
  if(NULL!=parent->sub && NULL!=child->name && NULL!=child->name->string) {

    dnode_t *node = (parent->sub)->dict_allocnode((parent->sub)->dict_context);
    char *key = strdup(child->name->string);

    clog( info, CTRACE, "XML: xml_node_adddata(), Saving sub node name=\"%.80s\"", child->name->string);

    if (node) {
      dnode_init(node, child);
      dict_insert(parent->sub, node, key);

      // let the child know who they are
      child->me = node;

      // success!
      return_value=1;
    }
  }

  // let the child know whow their parent is
  if(NULL!=parent && NULL!=child) {
    child->parent = parent;
  }

  return return_value;
}

#ifndef NOTEST
void Test__cxml_node_addnode(CuTest* tc)
{
  CXMLNODE* a = NULL;
  CXMLNODE* b = NULL;
  dnode_t * dnode= NULL;

  a = cxml_node_create(NULL, NULL, "a", 1);
  b = cxml_node_create(NULL, NULL, "b", 1);

  CuAssertPtrNotNull(tc, a);
  CuAssertPtrNotNull(tc, b);

  // make sure the sub starts out null
  CuAssertPtrEquals(tc, NULL, a->sub);

  // add b to a
  cxml_node_addnode(NULL, a, b);

  // check that the child knows their dict node
  CuAssertPtrNotNull(tc, b->me);

  // check that the child knows their parent
  CuAssertPtrEquals(tc, a, b->parent);

  // check that the sub isn't null any more
  CuAssertPtrNotNull(tc, a->sub);

  // check that the a->sub contains b
  dnode = dict_last(a->sub);
  CuAssertPtrEquals(tc, b, (CXMLNODE*)dnode->dict_data);

  cxml_node_free(NULL, &a);
}
#endif // NOTEST



