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

#include <cdata.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

void cxml_fn_data(void *userData, const XML_Char *text, int len)
{
  CXMLROOT *xml=userData;
  CXMLNODE *node=NULL;
  register char *tmp=NULL;
  int   size=len;


  if(NULL==xml || NULL==xml->stack) return;

  // add the data to the xml
  node=(CXMLNODE *)stack_peek(xml->stack);
  if(NULL!=node) {
    tmp=(char*)text+len-1;
    // trim whitespace from end of data
    while(0<size) {
      if('\n'==(*tmp) || '\r'==(*tmp) || '\t'==(*tmp)) {
        --tmp;
        --size;
      }
      else {
        break;
      }
    }

    tmp=(char*)text;
    // trim the whitespace from the beginning of data
    while(0<size) {
      if(' '==(*tmp)) {
        ++tmp;
        --size;
      }
      else {
        break;
      }
    }
  }

  if(0<size) {
    // add a separating space if there is existing data
    if(NULL!=node->data) {
      cxml_node_adddata(NULL, node, " ", 1);
    }
    // add the rest of the data
    cxml_node_adddata(NULL, node, tmp, size);
  }

}

#ifndef NOTEST
void Test__cxml_fn_data(CuTest* tc)
{
  // TODO: Add test for cxml_fn_data()
}
#endif // NOTEST

