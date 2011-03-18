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

void cxml_node_iterate(CLOG_INFO* info, CXMLNODE *node)
{
  if(NULL!=node && NULL!=node->last) {
    node->last=NULL;
  }
}

#ifndef NOTEST
void Test__cxml_node_iterate(CuTest* tc)
{
  CXMLNODE* a = NULL;
  CXMLNODE* b = NULL;

  // create 4 nodes, a, b, c, d. Put all on A
  a = cxml_node_create(NULL, NULL, "a", 1);
  b = cxml_node_create(NULL, a, "b", 1);

  // make sure they're all not NULL
  CuAssertPtrNotNull(NULL, a);
  CuAssertPtrNotNull(NULL, b);

  // Check that node->last is NULL
  CuAssertPtrEquals(tc, NULL, a->last);

  // iterate
  cxml_node_iterate(NULL, a);
  CuAssertPtrEquals(tc, NULL, a->last);

  // next
  cxml_node_next(NULL, a, "b");
  CuAssertPtrNotNull(tc, a->last);

  // iterate
  cxml_node_iterate(NULL, a);
  CuAssertPtrEquals(tc, NULL, a->last);

  // free a
  cxml_node_free(NULL, &a);

  // make sure a is NULL
  CuAssertPtrEquals(NULL, NULL, a);
}
#endif // NOTEST

