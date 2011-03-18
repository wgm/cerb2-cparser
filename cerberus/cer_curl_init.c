
#include "cerberus.h"

// CURL Dynamic Lib defines:
void *                    dl_curl = NULL;
CURLFORMcode            (*dl_curl_formadd)        (struct curl_httppost ** firstitem, struct curl_httppost ** lastitem, ...);
void                    (*dl_curl_formfree)       (struct curl_httppost * form); 
CURLcode                (*dl_curl_global_init)    (long flags);
CURL *                  (*dl_curl_easy_init)      (void);
CURLcode                (*dl_curl_easy_setopt)    (CURL *handle, CURLoption option, ...);
CURLcode                (*dl_curl_easy_perform)   (CURL *handle);
void                    (*dl_curl_easy_cleanup)   (CURL *handle);
void                    (*dl_curl_global_cleanup) (void);
curl_version_info_data* (*dl_curl_version_info)   (CURLversion type);

  // ##########################################################################
  // ##############=- INITIALIZE CURL -=#######################################
  // ##########################################################################
int cer_curl_init(CLOG_INFO* log, CERBFIG** pcer_config, CSTRING* curl_location) {
  CERBFIG* cer_config = *pcer_config;
  // dynamic linking error var
  char *dl_error=NULL;
  int return_code=0;
#ifndef WIN32

  if(NULL!=curl_location && NULL!=curl_location->string) {
    // use run time linking
    dl_curl = dlopen(curl_location->string, RTLD_LAZY);
    if(!dl_curl) {
      dl_error=(char*)dlerror();
      // ERROR!
      clog(log, CDEBUG, "DL: Could not load library! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded %s to %p", curl_location->string, dl_curl);
    }
  }
  // fallback to libcurl.so.3
  if(NULL==dl_curl) {
    // try to use libcurl in local dir
    dl_curl = dlopen(_LIBCURL3, RTLD_LAZY);
    if(!dl_curl) {
      dl_error=(char*)dlerror();
      // ERROR!
      clog(log, CFATAL, "DL: Could not load library! (%s)", _LIBCURL3, dl_error);
      return_code=EX_SOFTWARE;
    }
    else {
      clog(log, CDEBUG, "DL: Loaded %s to %p", _LIBCURL3, dl_curl);
    }
  }
  // fallback to libcurl.so.2
  if(NULL==dl_curl) {
    // try to use libcurl in local dir
    dl_curl = dlopen(_LIBCURL2, RTLD_LAZY);
    if(!dl_curl) {
      dl_error=(char*)dlerror();
      // ERROR!
      clog(log, CFATAL, "DL: Could not load library! (%s)", _LIBCURL2, dl_error);
      return_code=EX_SOFTWARE;
    }
    else {
      clog(log, CDEBUG, "DL: Loaded %s to %p", _LIBCURL2, dl_curl);
    }
  }

  // if we couldn't load the library, set an error
  if(NULL==dl_curl) {
    clog(log, CFATAL, "DL: Could not load the cURL dynamic library.");
    clog(log, CFATAL, "DL: The email has been fixed in the temp directory.");
    clog(log, CFATAL, "DL: Please fix the error and try to re-import the email via piping.");
    dl_error="ERROR";
  }

  if(NULL==dl_error) {
    // library was loaded OK!
    dl_curl_global_init = dlsym(dl_curl, "curl_global_init");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_global_init from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_global_init to %p", dl_curl_global_init);
    }
  }
  if(NULL==dl_error) {
    dl_curl_easy_init = dlsym(dl_curl, "curl_easy_init");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_easy_init from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_easy_init to %p", dl_curl_easy_init);
    }
  }
  if(NULL==dl_error) {
    dl_curl_easy_setopt = dlsym(dl_curl, "curl_easy_setopt");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_easy_setopt from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_easy_setopt to %p", dl_curl_easy_setopt);
    }
  }
  if(NULL==dl_error) {
    dl_curl_formadd = dlsym(dl_curl, "curl_formadd");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_formadd from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_formadd to %p", dl_curl_formadd);
    }
  }
  if(NULL==dl_error) {
    dl_curl_formfree = dlsym(dl_curl, "curl_formfree");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_formfree from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_formfree to %p", dl_curl_formadd);
    }
  }  
  if(NULL==dl_error) {
    dl_curl_easy_perform = dlsym(dl_curl, "curl_easy_perform");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_easy_perform from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_easy_perform to %p", dl_curl_easy_perform);
    }
  }
  if(NULL==dl_error) {
    dl_curl_easy_cleanup = dlsym(dl_curl, "curl_easy_cleanup");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_easy_cleanup from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_easy_cleanup to %p", dl_curl_easy_cleanup);
    }
  }
  if(NULL==dl_error) {
    dl_curl_global_cleanup = dlsym(dl_curl, "curl_global_cleanup");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_global_cleanup from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_global_cleanup to %p", dl_curl_global_cleanup);
    }
  }
  if(NULL==dl_error) {
    dl_curl_version_info = dlsym(dl_curl, "curl_version_info");
    dl_error=(char*)dlerror();
    if(NULL!=dl_error) {
      return_code=EX_SOFTWARE;
      clog(log, CFATAL, "DL: Could not load function curl_version_info from %s! (%s)", curl_location->string, dl_error);
    }
    else {
      clog(log, CDEBUG, "DL: Loaded curl_version_info to %p", dl_curl_version_info);
    }
  }
  if(NULL!=dl_error) {
    if(NULL!=dl_curl) {
      dlclose(dl_curl);
      dl_curl=NULL;
    }
  }
#endif

#ifdef WIN32
  // use the compiled in version of curl (win32)
  dl_curl_formadd          = curl_formadd;
  dl_curl_formfree          = curl_formfree;
  dl_curl_global_init      = curl_global_init;
  dl_curl_easy_init        = curl_easy_init;
  dl_curl_easy_setopt      = curl_easy_setopt;
  dl_curl_easy_perform     = curl_easy_perform;
  dl_curl_easy_cleanup     = curl_easy_cleanup;
  dl_curl_global_cleanup   = curl_global_cleanup;
  dl_curl_version_info     = curl_version_info;

  // clearn the TEMP and TMP environment vars on Win32
  putenv("TEMP=");
  putenv("TMP=");
#endif

  if(0==return_code) {
    dl_curl_global_init(CURL_GLOBAL_ALL);
    cer_config->curl = dl_curl_easy_init();
  }

  if(0==return_code) {
    curl_version_info_data *version = NULL;
    version = dl_curl_version_info(CURLVERSION_NOW);
    if(NULL!=version) {
      clog(log, CDEBUG, "cURL: version %s", version->version);
      if(version->features ^= CURL_VERSION_SSL) {
        clog(log, CDEBUG, "cURL: using SSL version %s", version->ssl_version);
      }
      else {
        clog(log, CDEBUG, "cURL: no SSL support");
      }
    }
  }
  return return_code;
}
