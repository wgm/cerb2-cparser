/* cFile - file I/O library with line buffering
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

#ifndef __CFILE_H__
#define __CFILE_H__

#ifndef WIN32
#define HAVE_MKSTEMP 1
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cdict.h>
#include <cstring.h>

/* O_BINARY isn't required (or even defined sometimes) under Unix */
#ifndef O_BINARY
#define O_BINARY 0
#endif

#define CFILE_BUFFER_SIZE 8192
// for debugging
//#define CFILE_BUFFER_SIZE 16

typedef struct _CFILESYSTEM {
  dict_t *files;      // branches
  int     settings;   // if the file is an in-memory only file
  int     filenumber; // counter for file numbers
} CFSYS ; 

// structure for the files
typedef struct _CFILE {
  char*   filename;   // full path + name of file
  int     fileno;     // returned by 'man 2 open'
  int     fileflags;  // the flags used in the fopen command
  off_t   file_pos;   // current file offset
  off_t   line_pos;   // last line offset (for rewinding)
  int     line_used;  // if the last line was used or not
  char *  buffer;     // the read buffer
  char *  buffer_ptr; // current ptr in the read buffer
  char *  buffer_end; // end of the read buffer
  CFSYS * cfsys;      // CFILE Obj
  dnode_t *me;        // In the cfsys list of files, this is me
} CFILE ;

extern CFILE CFILE_STDIN;
extern CFILE CFILE_STDOUT;
extern CFILE CFILE_STDERR;
extern CFILE* PCFILE_STDIN;
extern CFILE* PCFILE_STDOUT;
extern CFILE* PCFILE_STDERR;

#define CFSYS_INMEMORY 0x01
#define CFSYS_FALLBACK 0x02

extern int      cfile_buffer(CFILE** pfile, off_t change);
extern void     cfile_cleanup(CFSYS** cfp);
extern int      cfile_close(CFILE** pfile);
extern void     cfile_free(CFILE** pfile);
extern CSTRING* cfile_getline(CFILE** pfile, char *term);
extern CSTRING* cfile_getline_buffer(CFILE** pfile, char *term, CSTRING* line);
extern void     cfile_getline_next(CFILE** pfile);
extern CFSYS*   cfile_init(int flags);
extern int      cfile_lseek(CFILE** pfile, off_t offset, int whence);
extern CFILE*   cfile_mkstemp(CFSYS** cfp, char* path);
extern CFILE*   cfile_new();
extern CFILE*   cfile_open(CFSYS** cfp, const char *pathname, int flags);
extern CFILE*   cfile_open_create(CFSYS** cfp, const char *pathname, int flags, mode_t mode);
extern int      cfile_pipe(CFILE** ptarget, CFILE** psource, int convert);
extern ssize_t  cfile_read(CFILE** pfile, void*buf, size_t count);
extern int      cfile_sync(CFILE** pfile);
extern ssize_t  cfile_write(CFILE** pfile, void*buf, size_t count);
extern int      cfile_unlink(CFSYS** cfp, char* filename);

extern CuSuite* TestSuite__cfile(void);
extern void Test__cfile_buffer(CuTest* tc);
extern void Test__cfile_cleanup(CuTest* tc);
extern void Test__cfile_close(CuTest* tc);
extern void Test__cfile_free(CuTest* tc);
extern void Test__cfile_getline(CuTest* tc);
extern void Test__cfile_getline_buffer(CuTest* tc);
extern void Test__cfile_getline_next(CuTest* tc);
extern void Test__cfile_init(CuTest* tc);
extern void Test__cfile_lseek(CuTest* tc);
extern void Test__cfile_mkstemp(CuTest* tc);
extern void Test__cfile_new(CuTest* tc);
extern void Test__cfile_open(CuTest* tc);
extern void Test__cfile_open_create(CuTest* tc);
extern void Test__cfile_pipe(CuTest* tc);
extern void Test__cfile_read(CuTest* tc);
extern void Test__cfile_sync(CuTest* tc);
extern void Test__cfile_write(CuTest* tc);
extern void Test__cfile_unlink(CuTest* tc);

#endif // __CFILE_H__
