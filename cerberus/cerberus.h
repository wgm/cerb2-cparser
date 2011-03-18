#ifndef __CERBERUS_H__
#define __CERBERUS_H__

// ##########################################################################
// ################=- SYS LIBS -=############################################
// ##########################################################################

// memory checking
// #include <mcheck.h>

#ifndef WIN32
#include <dlfcn.h>      // dlopen/dlclose/dlsym/dlerror
#undef _LIBCURL
#if MAC
#define _LIBCURL2 "./libcurl.2.dylib"
#define _LIBCURL3 "./libcurl.2.dylib"
#else
#define _LIBCURL2 "./libcurl.so.2"
#define _LIBCURL3 "./libcurl.so.2"
#endif
#else
#include <stdlib.h>  // putenv() for win32
#endif

#define _GNU_SOURCE
#include <string.h>     // strndup

#include <stdio.h>      // perror

#include <sys/types.h>  // open
#include <sys/stat.h>   // open
#include <fcntl.h>      // open

#include <unistd.h>     // read

#include <sys/time.h>      // wait4
#ifndef WIN32
#include <sys/resource.h>  // wait4
#include <sys/wait.h>      // wait4
#endif


// ##########################################################################
// ################=- OUR LIBS -=############################################
// ##########################################################################

#include <cdata.h>
#include <cfile.h>
#include <clog.h>
#include <cmime.h>
#include <cstring.h>
#include <cxml.h>
#include <cpop3.h>
#ifndef NOTEST
# include <CuTest.h>
extern CuSuite* CuGetSuite();
extern CuSuite* CuStringGetSuite();
#endif
#include <rsa.h>

// ##########################################################################
// ################=- AUX LIBS -=############################################
// ##########################################################################

#ifndef isspace
#define isspace(c) ((' '==c || '\f'==c || '\n'==c || '\r'==c || '\t'==c || '\v'==c) ? 1 : 0 )
#endif

// for posting the XML
#include <curl/curl.h>

#if HAVE_ASSERT
#include <assert.h>
#else
#define assert
#endif

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"

extern int EF_ALIGNMENT;
extern int EF_PROTECT_BELOW;
extern int EF_PROTECT_FREE;

#endif

#define EX_OK           0       /* successful termination */
#define EX_USAGE        64      /* command line usage error */
#define EX_SOFTWARE     70      /* internal software error */
#define EX_TEMPFAIL     75      /* temp failure; user is invited to retry */

// CURL Dynamic Lib defines:
extern void *                    dl_curl;
extern CURLFORMcode            (*dl_curl_formadd)        (struct curl_httppost ** firstitem, struct curl_httppost ** lastitem, ...);
extern void                    (*dl_curl_formfree)       (struct curl_httppost * form); 
extern CURLcode                (*dl_curl_global_init)    (long flags);
extern CURL *                  (*dl_curl_easy_init)      (void);
extern CURLcode                (*dl_curl_easy_setopt)    (CURL *handle, CURLoption option, ...);
extern CURLcode                (*dl_curl_easy_perform)   (CURL *handle);
extern void                    (*dl_curl_easy_cleanup)   (CURL *handle);
extern void                    (*dl_curl_global_cleanup) (void);
extern curl_version_info_data* (*dl_curl_version_info)   (CURLversion type);

typedef struct _CERBFIG {
  // parser url to post to
  CSTRING*  parser_url;
  CSTRING*  parser_user;
  CSTRING*  parser_pass;
  unsigned int parser_https;

  // xsp data
  CSTRING*  xsp;

  // temp variable names
  CSTRING*  tmp_cerbmail;
  CSTRING*  tmp_cerbmime;

  // pop3 storage values
  struct linkedlist* poplist;

  // Max POP3 messages per run
  unsigned int pop3_max; // 1024
  unsigned int pop3_max_delete; // 0

  // The timeout for the POP3 read
  int  pop3_timeout; // 30

  // Curl Configuration
  CURL *           curl;
  CSTRING*         curl_cainfo;
  CSTRING*         curl_capath;
  long             curl_verifyhostpeer; // -1
  CSTRING*         curl_location;

  // debug info
  int       print_xml;
  int       print_curl;
  int       super_clean;
  int				debug_parse;
  
  // CFILE info
  CFSYS*  cfsys;
} CERBFIG;

// cerberus specific function calls
extern int        cer_add_sub_files(CLOG_INFO* log, CXMLNODE* sub, struct curl_httppost ** post, struct curl_httppost ** last);
extern int        cer_check_domain(CLOG_INFO* log, CXMLNODE* xml, CSTRING* domain, int key_type);
extern int        cer_curl_error(CURL *curl, curl_infotype it, char *mesg, size_t size, void *data);
extern int        cer_curl_init(CLOG_INFO* log, CERBFIG** pcer_config, CSTRING* curl_location);
extern size_t     cer_curl_write(void *buffer, size_t size, size_t nmemb, void *userp);
extern int        cer_del_sub_files(CLOG_INFO* log, CFSYS* cfsys, CXMLNODE* sub);
extern CXMLNODE*  cer_key_info(CLOG_INFO* log, CSTRING*dk);
extern int        cer_load_config(CLOG_INFO* log, CXMLROOT** pxml_root, char *configfile, CERBFIG** pcer_config);
extern int        cer_parse_files(CLOG_INFO* log, CERBFIG** pcer_config, CXMLROOT* xml_root, struct linkedlist** pmessages);
extern CFILE*     cer_save_input(CLOG_INFO* log, CFSYS* cfsys, char *pathfile);
extern int        cer_strdom(CLOG_INFO* log, CSTRING* haystack, CSTRING* needle);
extern int        cer_stremail(CLOG_INFO* log, CSTRING* haystack, CSTRING* needle);

#endif // __CERBERUS__

