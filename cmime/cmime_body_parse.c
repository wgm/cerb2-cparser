/***************************************************************************
                          cmime_body_parse.c  -  description
                             -------------------
    begin                : Fri Mar 28 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include <string.h>

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

CXMLNODE* cmime_body_parse(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CXMLNODE* sub, char* topnode)
{
  CFILE* file = NULL;
//  CXMLNODE* node = NULL;
  CXMLNODE* tnode = NULL;
  char *tmp = NULL;
  int parse_tnef=0;

  if(NULL==pfile) return NULL;
  file = *pfile;

  tnode = cxml_node_get(log, sub, topnode, "headers", "content-type", NULL);
  tmp=cxml_node_getattribute(log, tnode, "case");

  // if there is no 'case' decode the body as text, etc.
  if(NULL==tmp) {
    goto CMIME_BODY_PARSE_DEFAULT;
  }

  switch(*tmp) {
    case 'a': // multipart
      {
        CXMLNODE* boundary;

        // get the boundary
        boundary = cxml_node_get(log, tnode, "content-type","boundary",NULL);

        if(NULL!=boundary) { // multipart NEEDS boundarys
          CSTRING* line = cstring_new(0);

          // with multipart we need to find boundaries, let's find one.
          // look for significance (border / uudecode)
          // --[boundary]\r\n (length must be at least 5)
          // length must be at least 5 to match 'begin' also
          while( NULL!=cfile_getline_buffer(&file, "\r\n", line)) {
            // advance to next line
            cfile_getline_next(&file);
            if(4<line->length) {
              if(*(line->string) == '-') { // - == possible boundary
                if(*(line->string+1) == '-') { // beginning of boundary
                  if(0==strncmp((line->string+2), boundary->data->string, boundary->data->length)) {
                    if(0==strncmp((line->string+(2+boundary->data->length)), "--", 2)) {
                      cstring_free(&line);
                      return NULL;
                    }
                    else {
                      CXMLNODE *header = NULL;
                      CXMLNODE *tmp = NULL;
                      CXMLNODE *part = NULL;

                      header = cmime_header_parse( log, &file, file_pattern);
                      // make sure the header is in good standing and
                      // make sure this isn't the ending boundary
                      if(NULL!=header && NULL!=header->sub
                        && '-'!=*(line->string+boundary->data->length+2)
                        && '-'!=*(line->string+boundary->data->length+3)) {

                        // build the subnode header area with info from above
                        tmp = cxml_node_create(log, sub, "sub", 3);
                        if(NULL!=header) cxml_node_addnode(log, tmp, header);

                        // add the header info so the subfunctions can check against it
                        part = cxml_node_create(log, tmp, "parent-boundary", 15);
                        cxml_node_adddata(log, part, boundary->data->string, boundary->data->length);

                        // parse the body
                        cmime_body_parse( log, &file, file_pattern, tmp, "sub");
                      }
                      else {
                        cxml_node_free(log, &header);
                        cstring_free(&line);
                        return NULL;
                      }
                    }
                  }
                }
              }
              else if(*(line->string) == 'b') { // b == possible "begin" for uuencoded
                CSTRING* ctmp = cmime_parse_uu_begin( log, line);
                if(NULL!=ctmp) {
                  cfile_getline_next(&file);
                  cmime_parse_uu( log, &file, file_pattern, ctmp, sub, boundary);
                  cstring_free(&ctmp);
                  continue;
                }
              }
            }
          }
          cstring_free(&line);
        }
        break;
      }
    // G is above the others so the TNEF flag can get set.
    case 'g': // /ms-tnef
      {
        CXMLNODE* boundary;
        CFILE* winmail=NULL;

        parse_tnef=1;
        winmail=cfile_mkstemp(&(file->cfsys), file_pattern->string);

        // get the boundary if there is one
        boundary = cxml_node_get(log, sub, "sub","parent-boundary",NULL);

        cmime_parse_b64( log, &file, file_pattern, &winmail, NULL, NULL, boundary, 0);

        cfile_sync(&winmail);
        cfile_lseek(&winmail, 0, SEEK_SET);

        cmime_parse_tnef( log, &winmail, file_pattern, sub);

        cfile_close(&winmail);
        cfile_unlink(&(file->cfsys), winmail->filename);
        cfile_free(&winmail);

        break;
      }
    case 'b': // text/plain
    case 'c': // text/html
    case 'd': // text/
    case 'e': // message/rfc822
    case 'f': // /octet-stream
    default: // *unknown*
CMIME_BODY_PARSE_DEFAULT:
      {
        CXMLNODE* filenode;
        CXMLNODE* boundary;
        CFILE* text=NULL;

        text=cfile_mkstemp(&(file->cfsys), file_pattern->string);

        // add the header info so the subfunctions can check against it
        filenode = cxml_node_create(log, sub, "file", 4);

        tnode = cxml_node_create(log, filenode, "tempname", 8);
        cxml_node_adddata(log, tnode, text->filename, strlen(text->filename));

        if(NULL!=(tnode = cxml_node_get(log, sub, "sub", "headers", "content-disposition", "filename", NULL))) {
          CXMLNODE* _fnamenode;
          cxml_node_addattribute(log, filenode, "name", tnode->data->string);
          _fnamenode = cxml_node_create(NULL, filenode, "filename", 8);
          cxml_node_adddata(NULL, _fnamenode, tnode->data->string, tnode->data->length);
        }
        else if(NULL!=(tnode = cxml_node_get(log, sub, "sub", "headers", "content-type", "name", NULL))) {
          CXMLNODE* _fnamenode;
          cxml_node_addattribute(log, filenode, "name", tnode->data->string);
          _fnamenode = cxml_node_create(NULL, filenode, "filename", 8);
          cxml_node_adddata(NULL, _fnamenode, tnode->data->string, tnode->data->length);
        }

        if(NULL!=sub && NULL!=sub->name && NULL!=sub->name->string) {
          switch(sub->name->string[0]) {
            case 's':
              tnode = cxml_node_get(log, sub, "sub", "headers", "content-transfer-encoding", NULL);
              break;
            case 'e':
              tnode = cxml_node_get(log, sub, "email", "headers", "content-transfer-encoding", NULL);
              break;
            default:
              tnode=NULL;
              break;
          }
          tmp=cxml_node_getattribute(log, tnode, "case");
        }
        else {
          tnode=NULL;
          tmp=NULL;
        }

        // get the boundary if there is one
        boundary = cxml_node_get(log, sub, "sub","parent-boundary",NULL);

        if(NULL!=tmp) {
          switch(*tmp) {
            case 'a': // base64
              {
                cmime_parse_b64( log, &file, file_pattern, &text, sub, filenode, boundary, 1);
                break;
              }
            case 'b': // 7bit
              {
                cmime_parse_text( log, &file, file_pattern, &text, sub, filenode, boundary);
                break;
              }
            case 'c': // 8 bit
              {
                cmime_parse_text( log, &file, file_pattern, &text, sub, filenode, boundary);
                break;
              }
            case 'd': // QP
              {
                cmime_parse_qptext( log, &file, file_pattern, &text, sub, filenode, boundary);
                break;
              }
            case 'e': // UU
              {
                break;
              }
            default: // unknown as text
              {
                cmime_parse_text( log, &file, file_pattern, &text, sub, filenode, boundary);
                break;
              }
          }
        }
        else {
          cmime_parse_text( log, &file, file_pattern, &text, sub, filenode, boundary);
        }

//          tline=cxml_node_tostring(NULL, sub, 0);
//          printf("text: \n%s\n", tline->string);
//          cstring_free(&tline);

        cfile_close(&text);
        cfile_free(&text);
        break;
      }
  }

  return sub;
}

#ifndef NOTEST
void Test__cmime_body_parse(CuTest* tc)
{
}
#endif // NOTEST

