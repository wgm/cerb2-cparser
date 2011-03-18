/* cMIME - MIME decoding library
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

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#define INT32(x) (unsigned int)((unsigned char)(x)[0] \
                          +((unsigned char)(x)[1]<<8) \
                         +((unsigned char)(x)[2]<<16) \
                        +((unsigned char)(x)[3]<<24))

#define INT16(x) (unsigned int)((unsigned char)(x)[0] \
                         +((unsigned char)(x)[1]<<8))

void cmime_parse_tnef(CLOG_INFO* log, CFILE** pfile, CSTRING* file_pattern, CXMLNODE* psub)
{
  CXMLNODE* filenode = NULL;
  CXMLNODE* sub = NULL;
  CFILE* tmpfile=NULL;
  unsigned int abort=0;
  CFILE* file = NULL;
  unsigned short type=0;
  unsigned short name=0;
  size_t length=0;
  char *data=NULL;
  unsigned int x=0;
  unsigned char buf[5] = {0};

  if(NULL==pfile || NULL==*pfile) return;

  // file is the file we are reading from
  file = *pfile;

  // build the subnode header area with info from above
  sub = cxml_node_create(NULL, psub, "sub", 3);

  if(0<cfile_read(&file, buf, 4)) {
    // check the MIME magic number
    if(0x223e9f78==INT32(buf)) {
      // read past the TNEF key (2 bytes)
      cfile_lseek(&file, 2, SEEK_CUR);

      buf[0]=buf[1]=buf[2]=buf[3]=0;
      while(!abort && 0<cfile_read(&file, buf, 1)) {
        // switch on the TYPE
        switch(buf[0]) {
          case 0x01: // message
            {
              cfile_lseek(&file, 4, SEEK_CUR);
              if(0<cfile_read(&file, buf, 4)) {
                length = INT32(buf);
                cfile_lseek(&file, length+2, SEEK_CUR); // pass the data (length) + checksum (2)
              }
              break;
            }
          case 0x02: // attachment
            {
              if(0<cfile_read(&file, buf, 4)) {
                x=INT32(buf);
                type = x >> 16;
                name = x & 0x0000FFFF;

                if(0<cfile_read(&file, buf, 4)) {
                  length = INT32(buf);

                  switch(name) {
                    case 0x800F: // file data
                      {
                        if(NULL!=sub && NULL!=tmpfile) {
                          unsigned char filebuffer[CFILE_BUFFER_SIZE];
                          while(0<length && 0<(x=cfile_read(&file, filebuffer, CFILE_BUFFER_SIZE<length?CFILE_BUFFER_SIZE:length))) {

                            // do we need to start a new file?
                            if(CMIMEMAXFILESIZE < tmpfile->file_pos) {
                              CXMLNODE* tmpnode;
                              // start a new file
                              // close the old one
                              cfile_close(&tmpfile);
                              cfile_free(&tmpfile);

                              tmpfile=cfile_mkstemp(&(file->cfsys), file_pattern->string);

                              // add the header info so the subfunctions can check against it
                              tmpnode = cxml_node_create(NULL, filenode, "tempname", 8);
                              cxml_node_adddata(NULL, tmpnode, tmpfile->filename, strlen(tmpfile->filename));
                            }

                            cfile_write(&tmpfile, filebuffer, x);
                            length-=x;
                          }

                          cfile_lseek(&file, 2, SEEK_CUR); // go past the checksum
                        }
                        else {
                          cfile_lseek(&file, length+2, SEEK_CUR);
                        }
                        break;
                      }
                    case 0x8010: // file name
                      {
                        CXMLNODE* tnode = NULL;
                        data = (char*)malloc(length+1);
                        if(NULL!=data && NULL!=sub) {
                          memset(data, 0, length+1);
                          cfile_read(&file, data, length);

                          // add the filename to the 'FILE' element
                          tnode = cxml_node_create(NULL, filenode, "filename", 8);
                          cxml_node_adddata(NULL, tnode, data, length-1);

                          cxml_node_addattribute(log, filenode, "name", data);
                          tnode = cxml_node_create(NULL, sub, "content-disposition", 19);
                          filenode = cxml_node_create(NULL, tnode, "filename", 8);
                          cxml_node_adddata(NULL, filenode, data, length-1);
                          free(data);
                          data = NULL;
                        }
                        else {
                          // couldn't allocate the memory, skip past the data.
                          cfile_lseek(&file, length, SEEK_CUR);
                        }
                        // +2 for the checksum
                        cfile_lseek(&file, 2, SEEK_CUR);
                        break;
                      }
                    case 0x9002: // beginning of the attachment
                      {
                        if(NULL!=tmpfile) { // close last file if it was open
                          cfile_close(&tmpfile);
                          cfile_free(&tmpfile);
                        }

                        tmpfile = cfile_mkstemp(&(file->cfsys), file_pattern->string);

                        if(NULL!=sub && NULL!=tmpfile) {
                          CXMLNODE* tnode = NULL;

                          // add the header info so the subfunctions can check against it
                          filenode = cxml_node_create(NULL, sub, "file", 4);

                          tnode = cxml_node_create(NULL, filenode, "tempname", 8);
                          cxml_node_adddata(NULL, tnode, tmpfile->filename, strlen(tmpfile->filename));
                        }
                        // don't break
                      }
                    default:
                      { // skip the default data ( +2 for the checksum )
                        cfile_lseek(&file, length+2, SEEK_CUR);
                        break;
                      }
                  }
                }
                else {
                  // couldn't read!
                  abort=1;
                  break;
                }
              }
              break;
            }
          default:
            {
              abort=1;
              break;
            }
        } // switch(buf[0])
      } // while()
    }
  }

  if(NULL!=tmpfile) {
    cfile_close(&tmpfile);
    cfile_free(&tmpfile);
    tmpfile=NULL;
  }

  return;
}

#ifndef NOTEST
void Test__cmime_parse_tnef(CuTest* tc)
{
}
#endif // NOTEST


