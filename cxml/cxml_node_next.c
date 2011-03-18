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

#include <string.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CXMLNODE *cxml_node_next(CLOG_INFO* info, CXMLNODE *node, char *match)
{
  CXMLNODE* rval=NULL;
  dnode_t *dn = NULL;

  if(NULL==node || NULL==match || NULL==node->sub || dict_isempty(node->sub)) return NULL;

  if(NULL==node->last && NULL!=node->sub) { // find the first occurrance
    dn = dict_lookup(node->sub, match);
    if(NULL!=dn) { // if there was something found
      node->last = dn;
      rval = (CXMLNODE*) dnode_get( dn );
    }
  }
  else { // there was a 'last'
    dn = dict_next(node->sub, node->last);
    if(NULL!=dn) { // if there was something found
      node->last = dn;
      rval = (CXMLNODE*) dnode_get( dn );
      if(0!=strcmp(rval->name->string, match)) {
        rval=NULL;
      }
    }
  }

  return rval;
}

#ifndef NOTEST
void Test__cxml_node_next(CuTest* tc)
{
  CXMLNODE* a = NULL;
  CXMLNODE* b = NULL;
  CXMLNODE* c = NULL;
  CXMLNODE* d = NULL;
  CXMLNODE* node = NULL;

  // create 4 nodes, a, b, c, d. Put b on a, c on a and d on b.
  a = cxml_node_create(NULL, NULL, "a", 1);
  b = cxml_node_create(NULL, a, "bcd", 3);
  c = cxml_node_create(NULL, a, "bcd", 3);
  d = cxml_node_create(NULL, a, "bcd", 3);

  // make sure they're all not NULL
  CuAssertPtrNotNull(NULL, a);
  CuAssertPtrNotNull(NULL, b);
  CuAssertPtrNotNull(NULL, c);
  CuAssertPtrNotNull(NULL, d);

  // iterate through them and check the values
  cxml_node_iterate(NULL, a);

  // get B node
  node = cxml_node_next(NULL, a, "bcd");
  CuAssertPtrEquals(tc, b, node);

  // get C node
  node = cxml_node_next(NULL, a, "bcd");
  CuAssertPtrEquals(tc, c, node);

  // get D node
  node = cxml_node_next(NULL, a, "bcd");
  CuAssertPtrEquals(tc, d, node);

  // free a
  cxml_node_free(NULL, &a);
}
#endif // NOTEST


