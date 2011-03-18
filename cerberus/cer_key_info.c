

#include "cerberus.h"


// ##########################################################################
// #################=- Check Domain in Key -=################################
// ##########################################################################

CXMLNODE* cer_key_info(CLOG_INFO* log, CSTRING*dk) {
  CXMLNODE* xml_node=NULL;
  CXMLNODE* node=NULL;
  int error=0;
  if(NULL!=dk && 0<dk->length) {
    char *_tmpa=NULL;
    char *_tmpb=NULL;
    unsigned int* data=NULL;

    // clean up the contents of the key
    _tmpa=_tmpb=dk->string;
    while(0!=*_tmpa) {
      switch (*_tmpa) {
        case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        case '8': case '9': case '0': case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F':
        {
          *_tmpb=*_tmpa;
          ++_tmpb;
          break;
        }
      }
      ++_tmpa;
    }

    // fix the size.
    if(_tmpa > dk->string) {
      dk->length=_tmpa-dk->string;
    }
    else {
      dk->length=0;
    }

    data = (unsigned int*) malloc(dk->length);
    if(NULL!=data) {
      unsigned int arch = 0x04030201;
      unsigned char *archtest = (unsigned char*) &arch;
      int index=0;
      char buf[9]={0};

      // clear the memory
      memset(data, 0, dk->length);

      for(index=0; index<dk->length; index+=8) {
        memcpy(buf, ((char *)dk->string)+index, 8);
        sscanf(buf, "%x", (data+index/8));
      }

      for(index=0; index < dk->length / 8; index+=2) {
        rsa_decypher((data+index), (data+index+1));
      }

      data[index] = 0;

      clog(log, CDEBUG, "Checking for architecture type");
      if(1!=*archtest) { // 1 == little endian
        clog(log, CDEBUG, "Big endian found");
        // bigendian swap
        for(index=0; index < dk->length / 8; index++) {
          data[index] = ((data[index] & 0xFF000000) >> 24) |
                        ((data[index] & 0x00FF0000) >>  8) |
                        ((data[index] & 0x0000FF00) <<  8) |
                        ((data[index] & 0X000000FF) << 24);
        }
      }
      else {
        clog(log, CDEBUG, "Little endian found");
      }

      _tmpa = (char*)data;

      // some basic checks for a 'good key'
      index = 0;
      while(NULL!=(_tmpa=strchr(_tmpa, '\n'))) { ++_tmpa; index++; }

      // if it's less than 10, it's corrupt.
      if(10>index) {
        clog(log, CFATAL, "Corrupt KEY!");
        error=EX_SOFTWARE;
      }
      if(0==error) {
        unsigned int c=0;
        char *expiration=NULL;
        char *tmpa=NULL;
        struct tm tim;

        xml_node = cxml_node_create(log, NULL, "key", 3);

        // reset the ptrs
        _tmpa=_tmpb=(char*)data;
        _tmpa=strchr(_tmpb, '\n');

        // type
        *_tmpa=0;
        node = cxml_node_create(log, xml_node, "type", 4);
        cxml_node_adddata(log, node, _tmpb, strlen(_tmpb));
        *_tmpa='\n';
        _tmpb=++_tmpa;

        // company
        _tmpa=strchr(_tmpb, '\n');
        _tmpb=++_tmpa;

        // email address
        _tmpa=strchr(_tmpb, '\n');
        _tmpb=++_tmpa;

        // issue date
        _tmpa=strchr(_tmpb, '\n');
        _tmpb=++_tmpa;

        // expiration date
        _tmpa=strchr(_tmpb, '\n');
        *_tmpa=0;
        node = cxml_node_create(log, xml_node, "expiration", 10);
        cxml_node_adddata(log, node, _tmpb, strlen(_tmpb));
        expiration = strdup(_tmpb);
        *_tmpa='\n';
        _tmpb=++_tmpa;

        if(NULL!=expiration) {

          memset(&tim, 0, sizeof(tim));
          // validate expiration date:
          // YEAR
          tmpa = expiration;
          c = *(tmpa+4);
          *(tmpa+4) = 0;
          tim.tm_year = strtol(tmpa, NULL, 10) - 1900;
          *(tmpa+4) = c;

          // MONTH
          tmpa = expiration+4;
          c = *(tmpa+2);
          *(tmpa+2) = 0;
          tim.tm_mon = strtol(tmpa, NULL, 10) - 1;
          *(tmpa+2) = c;

          // DAY
          tmpa = expiration+6;
          tim.tm_mday = strtol(tmpa, NULL, 10);

          if( (tim.tm_year<200) && (mktime(&tim)>=time(NULL)) ) {
            char *begin;
            char *end;

            // domains
            _tmpa=strchr(_tmpb, '\n');
            *_tmpa=0;
            begin=end=_tmpb;
            while (0!=*begin) {
              begin=strchr(begin, '"'); // find starting quote
              if(NULL==begin) {
                break;
              }
              else {
                ++begin; // go past a quote
                end = strchr(begin, '"'); // find end quote
                if(NULL==end) {
                  break;
                }
                else {
                  node = cxml_node_create(log, xml_node, "domain", 6);
                  cxml_node_adddata(log, node, begin, end-begin);
                  begin = end+1;
                }
              }
              if(','==*begin) ++begin;
            }

            *_tmpa='\n';
            _tmpb=++_tmpa;

            // queue count
            _tmpa=strchr(_tmpb, '\n');
            _tmpb=++_tmpa;


            // key serial
            _tmpa=strchr(_tmpb, '\n');
            *_tmpa=0;
            node = cxml_node_create(log, xml_node, "serial", 6);
            cxml_node_adddata(log, node, _tmpb, strlen(_tmpb));
            *_tmpa='\n';
            _tmpb=++_tmpa;
            
            // extra number
            _tmpa=strchr(_tmpb, '\n');
            _tmpb=++_tmpa;

						// tag line
      			_tmpa=strchr(_tmpb, '\n');
      			*_tmpa=0;
						node = cxml_node_create(log, xml_node, "tagline", 7);
	        	cxml_node_adddata(log, node, _tmpb, strlen(_tmpb));
            *_tmpa='\n';
            _tmpb=++_tmpa;
          }
          else {
            clog(log, CFATAL, "KEY EXPIRED!");
            error=EX_SOFTWARE;
          }
          free(expiration);
          expiration=NULL;
        }
        else {
          clog(log, CFATAL, "Memory allocation error");
        }
        free(data);
        data=NULL;
      }
    }
  }

  if(error) {
    if(NULL!=xml_node) cxml_node_free(log, &xml_node);
    xml_node = NULL;
  }

  return xml_node;
}



