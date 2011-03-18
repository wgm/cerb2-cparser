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

int cxml_read_buf(CLOG_INFO* info, CXMLROOT *doc, char *buf)
{
  XML_Parser    p = NULL;
  int           ret=1;

  if (doc == NULL || buf == NULL) {
    clog( info, CERROR, "XML: xml_read_buf(), NULL Pointer error! doc=%p, buf=%p", doc, buf);
    return 0;
  }

  p = XML_ParserCreate(NULL);
  if (p == NULL) {
    clog( info, CERROR, "XML: xml_read_buf(), Could not create XML Parser!");
    return 0;
  }

  clog( info, CTRACE, "XML: xml_read_buf(), starting to parse buffer");
  XML_SetElementHandler(p, cxml_fn_start, cxml_fn_end);
  XML_SetCharacterDataHandler(p, cxml_fn_data);
  XML_SetUserData(p, doc);

  if (0 == XML_Parse(p, buf, strlen(buf), 1)) {
    clog( info, CFATAL, "XML: xml_read_buf(), %.80s: line %u",
          XML_ErrorString(XML_GetErrorCode(p)),
          XML_GetCurrentLineNumber(p));
    ret = 0;
  }

  clog( info, CTRACE, "XML: xml_read_buf(), done reading in file");

  XML_ParserFree(p);

  return ret;
}

#ifndef NOTEST
void Test__cxml_read_buf(CuTest* tc)
{
}
#endif // NOTEST


