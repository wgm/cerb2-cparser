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
#include <assert.h>

#include <expat.h>

#include <cdata.h>
#include <clog.h>
#include <cstring.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

void cxml_fn_start(void *userData, const XML_Char *name, const XML_Char **atts)
{
  CXMLROOT* xml = userData;
  CXMLNODE* node = NULL;
  CXMLNODE* pnode = NULL;
  int x=0;
  // fprintf(stderr, "start: %s\n", name);

  assert(NULL!=userData);
  assert(NULL!=name);
  assert(NULL!=atts);

  clog( xml->log, CTRACE, "XML: xml_fn_start(), creating node for \"%.80s\"", name);
  // create a node for us
  node = cxml_node_new(xml->log);

  clog( xml->log, CTRACE, "XML: xml_fn_start(), saving name to node name");
  // save the name of our node
  node->name = cstring_import((char *)name, strlen(name));

  clog( xml->log, CTRACE, "XML: xml_fn_start(), Checking for attachments");
  // set our attributes in our node
  for(x=0; atts[x]; x+=2) {
    cxml_node_addattribute(xml->log, node, (char*)atts[x], (char*)atts[x+1]);
  }

  clog( xml->log, CTRACE, "XML: xml_fn_start(), Saving self to xml dom");
  // if we have a parent on the stack, add ourselves to their children list
  pnode = (CXMLNODE*) stack_peek(xml->stack);
  if(NULL!=pnode) {
    cxml_node_addnode(xml->log, pnode, node);
  }
  // if there was no parent, make us the parent in the xml_data structure
  else {
    xml->data = node;
  }

  // put us on the top of the stack
  stack_push(xml->stack, node);
  clog( xml->log, CTRACE, "XML: xml_fn_start(), done creating node");

}

#ifndef NOTEST
void Test__cxml_fn_start(CuTest* tc)
{
  // TODO: Add test for cxml_fn_start()
}
#endif // NOTEST

