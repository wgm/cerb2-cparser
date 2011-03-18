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

CXMLNODE *cxml_node_new(CLOG_INFO* info)
{
  CXMLNODE *node = NULL;
  clog( info, CTRACE, "XML: xml_new_node(), creating new node");
  node = (CXMLNODE*) malloc(sizeof(CXMLNODE));
  if(NULL!=node) {
    clog( info, CTRACE, "XML: xml_new_node(), malloc returned non null");
    memset(node, 0, sizeof(CXMLNODE));
  }
  else {
    clog( info, CTRACE, "XML: xml_new_node(), malloc returned NULL!");
  }
  clog( info, CTRACE, "XML: xml_new_node(), returning new xml_node");
  return (node);
}

#ifndef NOTEST
void Test__cxml_node_new(CuTest* tc)
{
  CXMLNODE * node = NULL;

  node = cxml_node_new(NULL);
  CuAssertPtrNotNull(tc, node);

  cxml_node_free(NULL, &node);
}
#endif // NOTEST

