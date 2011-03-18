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

CSTRING *cxml_node_adddata(CLOG_INFO* info, CXMLNODE *node, char *data, int len)
{
  CSTRING * retval=NULL;

  if(NULL!=node) { // good, we have something to add it to.
    if(NULL!=data) { // add the data if it's not null.
      if(NULL==node->data) {
        node->data = cstring_import(data, len);
      }
      else {
        node->data = cstring_strcat_imp(node->data, data, len);
      }
    }
    if(NULL!=node->data) { // check to see if there is data.. this should be returned.
      retval=node->data;
    }
  }

  return retval;
}

#ifndef NOTEST
void Test__cxml_node_adddata(CuTest* tc)
{
  CXMLNODE* node = NULL;
  CSTRING *str = NULL;

  node = cxml_node_create(NULL, NULL, "test", 4);
  CuAssertPtrNotNull(tc, node);

  // make sure the data starts out null
  CuAssertPtrEquals(tc, NULL, node->data);

  // add some data
  cxml_node_adddata(NULL, node, "testing!", 8);

  // check that the data isn't null any more
  CuAssertPtrNotNull(tc, node->data);

  // check that the data equals "testing!"
  str = node->data;
  CuAssertStrEquals(tc, "testing!", str->string);

  cxml_node_free(NULL, &node);
}
#endif // NOTEST


