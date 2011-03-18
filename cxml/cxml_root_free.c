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

#include <stdlib.h>

#include <expat.h>

#include <cdata.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

void cxml_root_free(CLOG_INFO* info, CXMLROOT **data)
{
  // free the stack
  if(NULL!=data && NULL!=*data) {
    clog( info, CTRACE, "XML: starting to free xml data");
    // first we pop everything, we do not free anything on the stack because
    // it is just a reference to the xml_data->data area and will be free'd there
    if(NULL!=(*data)->stack) {
      while(!stack_is_empty((*data)->stack)) { stack_pop((*data)->stack); }
      stack_del((*data)->stack, free);
      (*data)->stack=NULL;
    }

    // free the xml_data
    if(NULL!=(*data)->data) {
      cxml_node_free(info, &(*data)->data);
    }
    free((*data));
    (*data) = NULL;
    clog( info, CTRACE, "XML: done free'ing xml data");
  }
}

#ifndef NOTEST
void Test__cxml_root_free(CuTest* tc)
{
  CXMLROOT *data = NULL;
  CXMLNODE* a = NULL;
  CXMLNODE* b = NULL;
  CXMLNODE* c = NULL;
  CXMLNODE* d = NULL;

  // pass NULL
  cxml_root_free(NULL, &data);
  CuAssertPtrEquals(tc, NULL, data);

  // free properly
  data = cxml_root_new(NULL);
  CuAssertPtrNotNull(tc, data);
  cxml_root_free(NULL, &data);
  CuAssertPtrEquals(tc, NULL, data);

  // try a double free
  data = cxml_root_new(NULL);
  CuAssertPtrNotNull(tc, data);
  cxml_root_free(NULL, &data);
  CuAssertPtrEquals(tc, NULL, data);
  cxml_root_free(NULL, &data);
  CuAssertPtrEquals(tc, NULL, data);

  // try a free with a node added
  data = cxml_root_new(NULL);
  CuAssertPtrNotNull(tc, data);
  // create 4 nodes, a, b, c, d. Put b on a, c on a and d on b.
  a = cxml_node_create(NULL, NULL, "a", 1);
  b = cxml_node_create(NULL, a, "b", 1);
  c = cxml_node_create(NULL, a, "c", 1);
  d = cxml_node_create(NULL, b, "d", 1);
  data->data = a;
  cxml_root_free(NULL, &data);
  CuAssertPtrEquals(tc, NULL, data);

}
#endif // NOTEST

