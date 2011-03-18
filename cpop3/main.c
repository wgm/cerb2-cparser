/* cPOP3 - library for the POP3 protocol
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

#include <stdlib.h>

#include "cpop3.h"

int main(int argc, char ** argv)
{
  int r = 0;
  CPOP3* pop3 = NULL;
  CLOG_INFO* log = NULL;

  log = clog_open("test_cpop3.log",CTRACE, NULL, 0);

  pop3 = cpop3_init(log, argv[1], 110, 30);

  if(0==cpop3_connect(log, pop3)){
    if(0==cpop3_user(log, pop3, argv[2])) {
      if(0==cpop3_pass(log, pop3, argv[3])) {

        r = cpop3_stat(log, pop3);
        if(0<r) {
          char * filename=NULL;
          while(NULL!=(filename = cpop3_retr(log, pop3, "/tmp/cerbmail_XXXXXX"))) {
            printf("Saving message to %s\n", filename);
            free(filename);
          }
        }
      }
    }
    cpop3_quit(log, pop3);
  }

  cpop3_disconnect(log, pop3);

  clog_close(log);

  return r;
}
