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
#include <string.h>

#include <expat.h>

#include <cdata.h>

#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CXMLROOT *cxml_root_new(CLOG_INFO* info) {
  CXMLROOT *root = NULL;
  clog( info, CTRACE, "XML: cxml_root_new() creating new struct");
  root = (CXMLROOT*) malloc(sizeof(CXMLROOT));
  if(NULL!=root) {
    clog( info, CTRACE, "XML: cxml_root_new(), malloc returned non null");
    memset(root, 0, sizeof(CXMLROOT));
    clog( info, CTRACE, "XML: cxml_root_new(), creating stack for new struct");
    root->stack = stack_new(128);
    root->log = info;
  }
  else {
    clog( info, CTRACE, "XML: cxml_root_new(), malloc returned NULL!");
  }
  clog( info, CTRACE, "XML: cxml_root_new(), returning new CXMLROOT");
  return (root);
}

#ifndef NOTEST
void Test__cxml_root_new(CuTest* tc)
{

  CXMLROOT *xmlroot = NULL;
  CLOG_INFO* log=NULL;

  // with NULL log
  xmlroot = cxml_root_new(NULL);
  CuAssertPtrNotNull(tc, xmlroot);
  CuAssertPtrEquals(tc, NULL, xmlroot->data);
  CuAssertPtrEquals(tc,NULL, xmlroot->log);
  cxml_root_free(NULL, &xmlroot);

  // with actual log
  log = clog_open(NULL, 0, NULL, 0);
  xmlroot = cxml_root_new(log);
  CuAssertPtrNotNull(tc, xmlroot);
  CuAssertPtrEquals(tc, NULL, xmlroot->data);
  CuAssertPtrEquals(tc,log,xmlroot->log);
  cxml_root_free(NULL, &xmlroot);
  clog_close(log);
}
#endif // NOTEST

