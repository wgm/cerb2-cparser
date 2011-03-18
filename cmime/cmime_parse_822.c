/***************************************************************************
                          cmime_parse_822.c  -  description
                             -------------------
    begin                : Thu May 15 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#ifndef isspace
#define isspace(c) ((' '==c || '\f'==c || '\n'==c || '\r'==c || '\t'==c || '\v'==c) ? 1 : 0 )
#endif

CXMLNODE* cmime_parse_822(CLOG_INFO* log, CXMLNODE* root, CSTRING* line, CXMLNODE* last)
{
  char *pos=NULL;
  CXMLNODE* node=NULL;

  if(NULL==root || NULL==line) return NULL;

  pos=line->string;

  if('\f'==*pos || '\t'==*pos || '\v'==*pos || ' '==*pos) {
    ++pos;
    // add to 'last' node and unwrap
    while('\f'==*pos || '\t'==*pos || '\v'==*pos || ' '==*pos) {
      ++pos;
    }
    --pos; *pos=' ';  // backup one character and replace the character with a space so the unwrapping is neat.
    cxml_node_adddata(NULL, last, pos, line->length - (pos - line->string) - 2); // length - whitespace - \r\n
    // for some reason, sometimes you get a space for the \r\n for the header seps, we'll just check to
    // see if they did this by stripping the wite space out from the beginning of the line
    if(*(pos+1)=='\r' && *(pos+2)=='\n') {
      node = NULL;
    }
    else {
      node = last;
    }
  }
  else if(*pos=='\r' && *(pos+1)=='\n') {
    node = NULL; // end of headers
  }
  else {
    // will break out later.
    while(line->length > (pos-line->string) ) {
      if(*pos==':' && (*(pos+1)==' ' || *(pos+1)=='\t')) { // if it's the end of the header
        break;
      }
      // if we can, make the header name lower case
      if(64<*pos && 91>*pos) {
        *pos += 32;
      }

      ++pos;
    } // while()

    // it didn't find the 'header' part of the line
    if(0==(line->length-(pos-line->string))) {
      // If it doesn't find anything we return the root node.
      // if used in a loop and the next line begins with a 'whitespace'
      // then the root will get added to. :-) this is usually only
      // matched with the 'from' generic email header
      // and does not hurt anything because the next line is not
      // beginning with whitespace.
      return root;
    }
    else { // it found it
      node = cxml_node_create( NULL, root, line->string, (pos - line->string) );
      if(2 < line->length - (pos - line->string) ) { // if hte remaining data is longer than the separator
        char *end=line->string+line->length;
        pos+=2;
        // trim the data, beginning and end
        while(isspace(*pos)) ++pos;
        while(isspace(*end)) {
          *end=0;
          ++end;
          line->length--;
        }

        cxml_node_adddata(NULL, node, pos, (line->length - (pos - line->string)) - 2 ); // do not add the \r\n
      }
    }
  } // end of if/else

  return node;

}


#ifndef NOTEST
void Test__cmime_parse_822(CuTest* tc)
{
}
#endif // NOTEST
