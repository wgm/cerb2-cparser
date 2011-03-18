/***************************************************************************
                          cmime.h  -  description
                             -------------------
    begin                : Wed Mar 19 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#ifndef __CMIME_H__
#define __CMIME_H__

#include <cfile.h>
#include <cxml.h>

//#define CMIMEMAXFILESIZE 1048580
//#define CMIMEMAXFILESIZE 1024
#define CMIMEMAXFILESIZE 524290

extern CuSuite* TestSuite__cmime(void);
extern void Test__cmime_body_parse(CuTest* tc);
extern void Test__cmime_file_parse(CuTest* tc);  
extern void Test__cmime_header_parse_ct(CuTest* tc);
extern void Test__cmime_header_parse(CuTest* tc);
extern void Test__cmime_parse_822(CuTest* tc);
extern void Test__cmime_parse_b64_string(CuTest* tc);
extern void Test__cmime_parse_b64(CuTest* tc);
extern void Test__cmime_parse_qptext_line(CuTest* tc);
extern void Test__cmime_parse_qptext(CuTest* tc);
extern void Test__cmime_parse_subject(CuTest* tc);
extern void Test__cmime_parse_text(CuTest* tc);
extern void Test__cmime_parse_tnef(CuTest* tc);
extern void Test__cmime_parse_uu_begin(CuTest* tc);
extern void Test__cmime_parse_uu(CuTest* tc);

extern CXMLNODE* cmime_body_parse    (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CXMLNODE* header, char* topnode);
extern CXMLNODE* cmime_file_parse    (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern);
extern CXMLNODE* cmime_header_parse  (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern);
extern CXMLNODE* cmime_parse_822     (CLOG_INFO* log, CXMLNODE* root, CSTRING* line, CXMLNODE* last);
extern void      cmime_parse_b64     (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CFILE** save, CXMLNODE* sub, CXMLNODE* files, CXMLNODE* boundary, int split);
extern int       cmime_parse_b64_string(char *str, int len);
extern void      cmime_parse_qptext  (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CFILE** save, CXMLNODE* sub, CXMLNODE* files, CXMLNODE* boundary);
extern CSTRING*  cmime_parse_subject (CLOG_INFO* log, CSTRING* subject);
extern void      cmime_parse_text    (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CFILE** save, CXMLNODE* sub, CXMLNODE* files, CXMLNODE* boundary);
extern void      cmime_parse_tnef    (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CXMLNODE* sub);
extern CXMLNODE* cmime_parse_uu      (CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CSTRING* filename, CXMLNODE* sub, CXMLNODE* boundary);
extern CSTRING * cmime_parse_uu_begin(CLOG_INFO* log, CSTRING* line);

#endif // __CMIME_H__
