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

#ifndef __CXML_H__
#define __CXML_H__

#include <expat.h>
#include <cdict.h>
#include <cstring.h>
#include <clog.h>

// this sets the MAX number of nodes you can have at the same level.
#ifndef XML_NODE_LIMIT
#define XML_NODE_LIMIT 512
#endif

#ifndef NOTEST
#include <CuTest.h>
extern CuSuite* TestSuite__cxml(void);
extern void Test__cxml_fn_data(CuTest* tc);
extern void Test__cxml_fn_end(CuTest* tc);
extern void Test__cxml_fn_start(CuTest* tc);
extern void Test__cxml_node_addattribute(CuTest* tc);
extern void Test__cxml_node_adddata(CuTest* tc);
extern void Test__cxml_node_addnode(CuTest* tc);
extern void Test__cxml_node_create(CuTest* tc);
extern void Test__cxml_node_delete(CuTest* tc);
extern void Test__cxml_node_free(CuTest* tc);
extern void Test__cxml_node_getattribute(CuTest* tc);
extern void Test__cxml_node_get(CuTest* tc);
extern void Test__cxml_node_iterate(CuTest* tc);
extern void Test__cxml_node_new(CuTest* tc);
extern void Test__cxml_node_next(CuTest* tc);
extern void Test__cxml_node_tostring(CuTest* tc);
extern void Test__cxml_read_buf(CuTest* tc);
extern void Test__cxml_read_file(CuTest* tc);
extern void Test__cxml_root_free(CuTest* tc);
extern void Test__cxml_root_new(CuTest* tc);
extern void Test__cxml_strndup(CuTest* tc);
#endif

typedef struct _CXMLNODE {
  CSTRING *name; // name of the node / parameter
  CSTRING *data;
  dict_t *att;          // nodes with filled in data's
  dict_t *sub;          // branches
  dnode_t *last;        // used for getting the next, holds the last.
  dnode_t *me;          // this node's pointer, used when deleting from parent
  struct _CXMLNODE *parent;      // pointer used when traversing up the tree
} CXMLNODE ;

typedef struct _CXMLROOT {
  CXMLNODE *data;
  struct stack *stack;
  CLOG_INFO *log;
} CXMLROOT ;

// xml parsing funciton pointers
extern void     cxml_fn_data (void *userData, const XML_Char *text, int len);
extern void     cxml_fn_end  (void *userData, const XML_Char *name);
extern void     cxml_fn_start(void *userData, const XML_Char *name, const XML_Char **atts);

// to read an xml file into the xml struct
extern int      cxml_read_file(CLOG_INFO* info, CXMLROOT *doc, char *fname);
extern int      cxml_read_buf(CLOG_INFO* info, CXMLROOT *doc, char *buf);

// xml node functions
extern int       cxml_node_addattribute(CLOG_INFO* info, CXMLNODE *node, char *name, char *value);
extern CSTRING * cxml_node_adddata(CLOG_INFO* info, CXMLNODE *node, char *data, int len);
extern int       cxml_node_addnode(CLOG_INFO* info, CXMLNODE *parent, CXMLNODE *child);
extern CXMLNODE* cxml_node_create(CLOG_INFO* info, CXMLNODE *node, char *name, int len);
extern void      cxml_node_delete(CLOG_INFO* info, CXMLNODE **node);
extern void       cxml_node_free(CLOG_INFO* info, CXMLNODE **node);
extern CXMLNODE* cxml_node_get(CLOG_INFO* info, CXMLNODE *node, ...);
extern char    * cxml_node_getattribute(CLOG_INFO* info, CXMLNODE* node, char *name);
extern void      cxml_node_iterate(CLOG_INFO* info, CXMLNODE *node);
extern CXMLNODE* cxml_node_new(CLOG_INFO* info);
extern CXMLNODE* cxml_node_next(CLOG_INFO* info, CXMLNODE *node, char *match);
extern CSTRING * cxml_node_tostring(CLOG_INFO* info, CXMLNODE* node, int formatted);

// xml root functions
extern CXMLROOT* cxml_root_new(CLOG_INFO* info);
extern void      cxml_root_free(CLOG_INFO* info, CXMLROOT **data);

// xml string assist functions
extern char    * cxml_strndup(CLOG_INFO* info, const char* str, unsigned len);

#endif // __CXML_H__
