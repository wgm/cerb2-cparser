#include "cerberus.h"

// ##########################################################################
// #################=- Check Domain in Headers -=############################
// ##########################################################################

int cer_check_domain(CLOG_INFO* log, CXMLNODE* xml, CSTRING* domain, int key_type)
{
  int retval=0;
  CXMLNODE* _xa=NULL;
  CXMLNODE* _xb=NULL;
  int (*validate) (CLOG_INFO* log, CSTRING* haystack, CSTRING* needle);

  if(NULL==xml) return 0;

  switch(key_type) {
    case 6:
      validate = cer_stremail;
      clog(log, CDEBUG, "Checking for destination of %s in email headers", domain->string);
      break;
    default:
      validate = cer_strdom;
      clog(log, CDEBUG, "Checking for destination of *@%s in email headers", domain->string);
      break;
  }

  _xa = cxml_node_get(log, xml, "email", "headers", NULL);


  // check the TO line
  cxml_node_iterate(log, _xa);
  while((0==retval || -1==retval) && NULL!=(_xb=cxml_node_next(log, _xa, "to"))) {
    retval = validate(log, _xb->data, domain);
  }

  if(0==retval || -1==retval) {
    // check the CC line
    cxml_node_iterate(log, _xa);
    while((0==retval || -1==retval) && NULL!=(_xb=cxml_node_next(log, _xa, "cc"))) {
      retval = validate(log, _xb->data, domain);
    }
  } // if(retval)

  if(0==retval || -1==retval) {
    // check the CC line
    cxml_node_iterate(log, _xa);
    while((0==retval || -1==retval) && NULL!=(_xb=cxml_node_next(log, _xa, "bcc"))) {
      retval = validate(log, _xb->data, domain);
    }
  } // if(retval)


/* - work-arounds the key envelope check
  if(0==retval || -1==retval) {
    // check the delivered to line
    cxml_node_iterate(log, _xa);
    while((0==retval || -1==retval) && NULL!=(_xb=cxml_node_next(log, _xa, "delivered-to"))) {
      retval = validate(log, _xb->data, domain);
    }
  } // if(retval)
*/

  if(0==retval || -1==retval) {
    // check the received line
    cxml_node_iterate(log, _xa);
    while((0==retval || -1==retval) && NULL!=(_xb=cxml_node_next(log, _xa, "received"))) {
      retval = validate(log, _xb->data, domain);
    }
  } // if(retval)

  return retval;
}

