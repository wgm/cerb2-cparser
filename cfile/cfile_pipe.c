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

#include <stdio.h> // perror

#include <unistd.h>
#include "cfile.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#if defined(WIN32)
#define fsync(fd) _commit(fd)
#endif

// convert=0 (none), convert=1 (to dos), convert=2 (to unix, to be implemented)
int cfile_pipe(CFILE** ptarget, CFILE** psource, int convert) {
  CFILE* target=NULL;
  CFILE* source=NULL;
  unsigned char buf[CFILE_BUFFER_SIZE];
  int tsize = 0;
  int rsize = 0;
  int wsize = 0;

  if(NULL==ptarget || NULL==psource) {
    return -1;
  }

  target=*ptarget;
  source=*psource;

  switch(convert) {
    case 0: // no conversion
      {
        // read from source
        while(0<(rsize = read(source->fileno, buf, CFILE_BUFFER_SIZE))) {
          // write to target
          wsize = write(target->fileno, buf, rsize);
          // check for write errors
          if(-1==wsize) {
            perror("cfile_pipe()::write()");
            // return -1 because we do not want the results to be used.
            return -1;
          }
          // add the written size to the total size
          tsize+=wsize;
        }
        break;
      }
    case 1: // unix 2 dos
      {
        unsigned char dosbuf[2*CFILE_BUFFER_SIZE];
        register int a=0;
        register int b=0;
        register int c=0;
        register int slashr = 0;
        register char chr=0;
        // read from source
        while(0<(c = read(source->fileno, buf, CFILE_BUFFER_SIZE))) {
          // convert to dos

          // process the first char so that we can safely check the previous char later
          if('\n'==buf[a]) {
            dosbuf[b]='\r';
            ++b;
          }
          dosbuf[b]=buf[a];
          ++b;
          ++a;
          
          // expand \n to \r\n (but only if it's not a \r\n already)
          while(a<c) {
            chr=buf[a];
            switch(chr) {
              case '\r':
                {
                  slashr=1;
                  dosbuf[b]=chr;
                  ++b;
                  ++a;
                  break;
                }
              case '\n':
                {
                  if(0==slashr) {
                    dosbuf[b]='\r';
                    ++b;
                  }
                }
              default:
                {
                  dosbuf[b]=chr;
                  ++b;
                  ++a;
                  slashr=0;
                  break;
                }
            }
          }
          
          // write to target
          b = write(target->fileno, dosbuf, b);
          // check for write errors
          if(-1==b) {
            perror("cfile_pipe()::write()");
            // return -1 because we do not want the results to be used.
            return -1;
          }
          // add the written size to the total size
          tsize+=b;
          // clear out the info
          a=0;
          b=0;
          c=0;
          chr=0;
        }
        break;
      }
    case 2: // dos to unix
      {
        register int a=0;
        register int b=0;
        register int c=0;
        register char chr=0;
        // read from source
        while(0<(rsize = read(source->fileno, buf, CFILE_BUFFER_SIZE))) {
          c = rsize-1;
          while(a<c) {
            chr=buf[a];
            if('\r'==chr && '\n'==buf[a+1]) {
              ++a;
              rsize--;
            }
            buf[b]=chr;
            ++b;
            ++a;
          }
          // write to target
          wsize = write(target->fileno, buf, rsize);
          // check for write errors
          if(-1==wsize) {
            perror("cfile_pipe()::write()");
            // return -1 because we do not want the results to be used.
            return -1;
          }
          // add the written size to the total size
          a=0;
          b=0;
          c=0;
          chr=0;
          tsize+=wsize;
        }
        break;
      }
  }

  // make sure everything is written to file.
  fsync(target->fileno);

  // return the total bytes written
  return tsize;
}

#ifndef NOTEST
void Test__cfile_pipe(CuTest* tc)
{
}
#endif // NOTEST
