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

char * cxml_node_getattribute(CLOG_INFO* info, CXMLNODE *node, char *name) {
  dnode_t *dn = NULL;

  clog( info, CTRACE, "XML: cxml_node_getattribute(), checking arguments");

  if(NULL==node || NULL==name) {
    clog( info, CTRACE, "XML: xml_gcxml_node_getattribute(), bad argument (%p, %p) returning NULL", node, name);
    return NULL;
  }

  clog( info, CTRACE, "XML: cxml_node_getattribute(), start search for parameter");

  if(NULL==node->att) {
    clog( info, CTRACE, "XML: cxml_node_getattribute(), attribute branch was NULL");
    return NULL;
  }

  dn = dict_lookup(node->att, name);
  if(NULL!=dn) {
    char *value=NULL;
    // found a winner
    clog( info, CTRACE, "XML: cxml_node_getattribute(), match found, returning value");
    value = dnode_get( dn );
    return value;
  }

  clog( info, CTRACE, "XML: xml_get_cxml_node_getattribute(), attribute not found, returning NULL");
  return NULL;
}

#ifndef NOTEST
void Test__cxml_node_getattribute(CuTest* tc)
{
  CXMLNODE *node = NULL;
  char * value = NULL;

  // create a node
  node = cxml_node_create(NULL, NULL, "test", 4);

  // add an attribute to a node
  cxml_node_addattribute(NULL, node, "test", "value");

  // get the attribute from the node
  value = cxml_node_getattribute(NULL, node, "test");

  // compare the attribute value
  CuAssertStrEquals(tc, "value", value);

  // look for an invalid attirbute
  value = cxml_node_getattribute(NULL, node, "testing");
  CuAssertPtrEquals(tc, NULL, value);

  cxml_node_free(NULL, &node);
}
#endif // NOTEST

