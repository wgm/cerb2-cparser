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

int cxml_node_addattribute(CLOG_INFO* info, CXMLNODE *node, char *name, char *value)
{
  char *key=NULL;
  char *data=NULL;

  assert(NULL!=node);

  // if the attributes is NULL create a new atts table
  if(NULL==node->att) {
    clog( info, CTRACE, "XML: xml_node_addattribute(), creating attributes node");
    node->att = dict_create(DICTCOUNT_T_MAX, (dict_comp_t)strcmp);
    // allow duplicates
    dict_allow_dupes(node->att);
  }

  // if the previous stuff worked, add the name/value pair
  if(NULL!=node->att && NULL!=name) {
    clog( info, CTRACE, "XML: xml_node_addattribute(), Saving attribute name=\"%.80s\" value=\"%.80s\"", name, value);
    key = strdup(name);
    data = strdup(value);
    return (dict_alloc_insert( node->att, key, data));
  }

  return 0;
}

#ifndef NOTEST
void Test__cxml_node_addattribute(CuTest* tc)
{
  CXMLNODE* node = NULL;
  dnode_t * dnode= NULL;

  node = cxml_node_create(NULL, NULL, "test", 4);
  CuAssertPtrNotNull(tc, node);

  // make sure the att's start out null
  CuAssertPtrEquals(tc, NULL, node->att);

  // add an atrtribute
  cxml_node_addattribute(NULL, node, "att_name", "att_value");

  // check that the att's are not null any longer
  CuAssertPtrNotNull(tc, node->att);

  // check that the att dictionary contains "att_name"
  dnode = dict_last(node->att);
  CuAssertPtrNotNull(tc, dnode);
  CuAssertStrEquals(tc, "att_name", (char*)dnode->dict_key);
  CuAssertStrEquals(tc, "att_value", (char*)dnode->dict_data);

  cxml_node_free(NULL, &node);
}
#endif // NOTEST



