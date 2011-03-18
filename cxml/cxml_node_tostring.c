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

// takes the cstring to clean, and returns a new string that is clean
CSTRING* cxml_node_tostring_clean_attribute(CSTRING* string)
{
  char *a=NULL;
  char *b=NULL;
  char *e=NULL;
  CSTRING* rval = NULL;
  if(NULL==string) return NULL;
  if(NULL==string->string) return NULL;

  rval = cstring_new(string->length);

  a=string->string;
  b=rval->string;
  e=string->string+string->length;


  while(e>a) {
    if( ( ('?' <= *a && '~' >= *a ) ||
        ('\'' <= *a && ';' >= *a ) ||
        ' ' == *a || '!' == *a || '#'==*a ||
        '$' == *a || '%' == *a || '='==*a) ) {
      *b=*a;
      ++b;
    }
    ++a;
  }

  rval->length=b-rval->string;
  return rval;
}

// takes the cstring to clean, and returns a new string that is clean
CSTRING* cxml_node_tostring_clean_element(CSTRING* string)
{
  char *a=NULL;
  char *b=NULL;
  char *e=NULL;
  CSTRING* rval = NULL;
  if(NULL==string) return NULL;
  if(NULL==string->string) return NULL;

  a=string->string;
  e=string->string+string->length;
  rval = cstring_new(string->length+1); // +1 just in case we prefix it with something later

  // if the first character is not a XML compliant, prefix it with an underscore
  if( ! ( ('A' <= *(string->string) && 'Z' >= *(string->string)) ||
    ('a' <= *(string->string) && 'z' >= *(string->string)) ||
    '_' == *(string->string) ) ) {
    cstring_strcat_imp(rval, "_", 1);
  }

  // point at the next place we want to write to in the destination
  b=rval->string+rval->length;

  while(e>a) {
    if( ( ('A' <= *a && 'Z' >= *a ) ||
        ('a' <= *a && 'z' >= *a ) ||
        ('0' <= *a && '9' >= *a ) ||
        '_' == *a || '.' == *a || '-'==*a ) ) {
      *b=*a;
      ++b;
    }
    ++a;
  }

  rval->length=b-rval->string;
  return rval;
}

// This function is recursive and will end up traversing the entire data structure
// it will convert it to XML as it goes.
CSTRING * cxml_node_tostring(CLOG_INFO* info, CXMLNODE *node, int level)
{
  CSTRING *cs=NULL;
  CSTRING *clean=NULL;
  CSTRING *blank=NULL;
  dnode_t *dn = NULL;
  int cdata=0;
  unsigned char *posa=NULL;
  int x=0;
  int closed=0;

  if(NULL==node) return NULL;

  cs = cstring_new(0);
  if(0==level) { // add the xml utf8 headers
    cstring_strcat_imp(cs, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", 39);
  }

  // indent the tags to the proper level
  for(x=0; x<level; x++) cstring_strcat_imp(cs, "  ", 2);

  // begin the tag
  cstring_strcat_imp(cs, "<", 1);
  clean = cxml_node_tostring_clean_element(node->name);
  cstring_strcat(cs, clean);
  cstring_free(&clean);

  // add the attributes to the struture
  if(NULL!=node->att && !dict_isempty(node->att)) {
    for(dn = dict_first(node->att); dn; dn = dict_next(node->att, dn)) {
      char *key=(char*)dnode_getkey(dn);
      char *data=(char*)dnode_get(dn);

      // add the space after the tag name or last attribute
      cstring_strcat_imp(cs, " ", 1);
      blank=cstring_new(strlen(key));
      cstring_strcat_imp(blank, key, strlen(key));
      clean=cxml_node_tostring_clean_element(blank);
      cstring_free(&blank); blank=NULL;
      cstring_strcat(cs, clean);
      cstring_free(&clean);

      // add the equal sign, quotes and value
      cstring_strcat_imp(cs, "=\"", 2);
      blank=cstring_new(strlen(data));
      blank=cstring_strcat_imp(blank, data, strlen(data));
      clean=cxml_node_tostring_clean_attribute(blank);
      cstring_free(&blank);
      cstring_strcat(cs, clean);
      cstring_free(&clean);
      cstring_strcat_imp(cs, "\"", 1);
    }
  }

  // if there is a sub element
  if(NULL!=node->sub && !dict_isempty(node->sub)) {
    // loop through the sub elements, recursing.
    for(dn = dict_first(node->sub); dn; dn = dict_next(node->sub, dn)) {
      CSTRING* dm=NULL;
      dm=cxml_node_tostring(info, dnode_get(dn), level+1);
      // if the last tag (from above) is not closed, we should close it here
      // we do not close it above as we may be able to use a \> close tag if
      // it has no children or data
      if(NULL!=dm && 0<dm->length && !closed) {
        cstring_strcat_imp(cs, ">\n", 2);
        closed=1;
      }

      // add the value of the recursed xml to this xml
      cstring_strcat(cs, dm);
      cstring_free(&dm);
    }
  }

  // check to see if we need to use cdata < &
  if(NULL!=node->data && 0<node->data->length) {
    // if the last tag (from above) is not closed, we should close it here
    // we do not close it above as we may be able to use a \> close tag if
    // it has no children or data
    if(!closed) {
      cstring_strcat_imp(cs, ">\n", 2);
      closed=1;
    }

    posa=node->data->string;
    if(NULL!=posa) {
      while(*posa!=0 && 0==cdata) {
        if(33>*posa || 126<*posa || 38==*posa || 60==*posa) {
          cdata=1;
          break;
        }

        posa++;
      }
    }
    if(1==cdata) {
      // CDATA begin
      for(x=0; x<level+1; x++) cstring_strcat_imp(cs, "  ", 2);
      cstring_strcat_imp(cs, "<![CDATA[\n", 10);

      // CDATA Contents
      for(x=0; x<level+2; x++) cstring_strcat_imp(cs, "  ", 2);
      cstring_strcat(cs, node->data);
      cstring_strcat_imp(cs, "\n", 1);

      // end CDATA
      for(x=0; x<level+1; x++) cstring_strcat_imp(cs, "  ", 2);
      cstring_strcat_imp(cs, "]]>\n", 4);
    }
    else {
      if(NULL!=node->data) {
        // DATA
        for(x=0; x<level+1; x++) cstring_strcat_imp(cs, "  ", 2);
        cstring_strcat(cs, node->data);
        cstring_strcat_imp(cs, "\n", 1);
      }
    }

  }
  else if(!closed) {
    // if the element tag hasn't been closed (the > at the end) we use the
    // entire element close \> instead
    cstring_strcat_imp(cs, " />\n", 4);
  }

  if(closed) {
    // Since the element tag already had it's > put at the end
    // we need to add a full close tag here
    for(x=0; x<level; x++) cstring_strcat_imp(cs, "  ", 2);
    cstring_strcat_imp(cs, "</", 2);
    clean=cxml_node_tostring_clean_element(node->name);
    cstring_strcat(cs, clean);
    cstring_free(&clean);
    cstring_strcat_imp(cs, ">\n", 2);
  }

  return cs;
}

#ifndef NOTEST
void Test__cxml_node_tostring(CuTest* tc)
{
  CXMLNODE* a = NULL;
  CXMLNODE* b = NULL;
  CXMLNODE* c = NULL;
  CXMLNODE* d = NULL;
  CSTRING *str= NULL;

  // create 4 nodes, a, b, c, d. Put b on a, c on a and d on b.
  a = cxml_node_create(NULL, NULL, "a", 1);
  b = cxml_node_create(NULL, a, "b", 1);
  c = cxml_node_create(NULL, a, "c", 1);
  d = cxml_node_create(NULL, b, "d", 1);

  // add some data
  cxml_node_adddata(NULL, d, "test", 4);

  // add a attribute
  cxml_node_addattribute(NULL, d, "test", "value");

  str = cxml_node_tostring(NULL, a, 0);

  CuAssertStrEquals(tc, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<a>\n  <b>\n    <d test=\"value\">\n      test\n    </d>\n  </b>\n  <c />\n</a>\n", str->string);

  // free a
  cxml_node_free(NULL, &a);
  
  cstring_free(&str);
}
#endif // NOTEST


