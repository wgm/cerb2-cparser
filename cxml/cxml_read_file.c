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

#include <errno.h>
#include "cxml.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

int cxml_read_file(CLOG_INFO* info, CXMLROOT *doc, char *fname)
{
  FILE        * stream  = NULL;
  XML_Parser    p       = NULL;
  void        * buf     = NULL;
  size_t        n;
  int           ret;
  int           done;

  if(NULL==fname) {
    clog( info, CERROR, "XML: xml_read_file() no filename was given!");
    return 0;
  }

  stream = fopen(fname, "r");

  // some error happend, couldn't read file
  if(NULL==stream && 0!=errno) {
    clog_perror( info, CFATAL, "XML: xml_read_file(): ");
    return 0;
  }

  if (doc == NULL) {
    clog( info, CERROR, "XML: xml_read_file(), Did the file contain valid XML?");
    return 0;
  }

  p = XML_ParserCreate(NULL);
  if (p == NULL) {
    clog( info, CERROR, "XML: xml_read_file(), Could not create XML Parser!");
    fclose(stream);
    return 0;
  }

  clog( info, CTRACE, "XML: xml_read_file(), starting to parse %.80s", fname);
  XML_SetElementHandler(p, cxml_fn_start, cxml_fn_end);
  XML_SetCharacterDataHandler(p, cxml_fn_data);

//  XML_SetCdataSectionHandler(p, cxml_fn_cdatastart, cxml_fn_cdataend);
//  XML_SetCommentHandler(p, comment_fn);
//  XML_SetProcessingInstructionHandler(p , processing_fn);
//  XML_SetUserData(p, &ud);
  XML_SetUserData(p, doc);

  ret = 1;
  for ( ;; ) {
    if ((buf = XML_GetBuffer(p, BUFSIZ)) == NULL) {
      clog( info, CERROR, "XML: xml_read_file(), buf=NULL!");
      ret = 0;
      break;
    }
    if ((n = fread(buf, 1, BUFSIZ, stream)) == 0 && ferror(stream)) {
      clog( info, CERROR, "XML: xml_read_file(), fread error!");
      ret = 0;
      break;
    }
    if (XML_ParseBuffer(p, n, (done = feof(stream))) == 0) {
      clog( info, CFATAL, "XML: xml_read_file(), %.80s: line %u",
            XML_ErrorString(XML_GetErrorCode(p)),
            XML_GetCurrentLineNumber(p));
      ret = 0;
      break;
    }
    if (done) {
      break;
    }
  }

  clog( info, CTRACE, "XML: xml_read_file(), done reading in file");

  XML_ParserFree(p);
  fclose(stream);
  return ret;
}

#ifndef NOTEST
void Test__cxml_read_file(CuTest* tc)
{
}
#endif // NOTEST


