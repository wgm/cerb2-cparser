/***************************************************************************
                          cmime_parse_subject.c  -  description
                             -------------------
    begin                : Tue May 13 2003
    copyright            : (C) 2003 by Benjamin Halsted
    email                : ben@webgroupmedia.com
 ***************************************************************************/

#include <string.h>
#include <unicode/ucnv.h>

#include "cmime.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

#define DEFAULT 1
#define BEGINNING 2
#define PARSE_QP 3
#define PARSE_B64 4
#define ENDING 5

// returns length of decoded subject
CSTRING* cmime_parse_subject(CLOG_INFO* log, CSTRING* subject)
{
  char *posa = NULL;
  char *posb = NULL;
  char *posc = NULL;
  char encoding[64] = {0};
  int state = DEFAULT;
  CSTRING* encoded = NULL;
  CSTRING* final = NULL;
  
  if(NULL==subject) return 0;

  final = cstring_new(subject->length);

  posa = posb = subject->string;

  while(NULL!=posa && 0 != *posa) {

    switch(state) {
    	case BEGINNING: {
    		posa+=2; // advance to after the =? bit
    		posc = posa; // record position for the type
			posa = (char*)strstr(posa, "?"); // advance to the next ? after the =?iso-...
			
			// clear and create the encoded buffer
			
			if(NULL!=encoded) {
				cstring_free(&encoded);
			}
			encoded = cstring_new(0);
			
			// clear the buffer
			memset(encoding, 0, 64);
			
			// copy max 63 bytes over
			memcpy(encoding, posc, (64>(posa-posc)?posa-posc:63));
			
			// check to see if it's a Q or a q for quoted printable
            ++posa;
            if('q'==*posa || 'Q'==*posa) {
				state = PARSE_QP;
            } else if('b'==*posa || 'B'==*posa) {
				state = PARSE_B64;
			}
        
    		// go past the q or b
            posa++;
            posa++; // we're sitting on Q?subject_goes_here now advance past ?
    		break;
    	}
    	case PARSE_QP: {
            while(0 != memcmp(posa, "?=", 2)) {
                // underscores become spaces
                if('_' == *posa) {
                    // *posb = ' ';
                    cstring_strcat_imp(encoded, " ", 1);
                    ++posa; ++posb;
                }
                else if('=' == *posa && (
                		(47<*(posa+1) &&  58>*(posa+1)) || 
                		(64<*(posa+1) &&  91>*(posa+1)) || 
                		(96<*(posa+1) && 123>*(posa+1))
                		)) {
                	char tmp[2]={0};
                    char c = 0;
                    ++posa; // get past =
                    // go to end of escaped character
                    ++posa;
                    ++posa;
                    // store this character and replace with a zero
                    c=*posa;
                    *posa=0;
                    posa-=2; // go back to one past the =
                    //*posb = strtol(posa, NULL, 16);
                    tmp[0] = strtol(posa, NULL, 16);
                    cstring_strcat_imp(encoded, tmp, 1);
                    ++posb;
                    // go back to where we mared a zero
                    posa+=2;
                    // restore the character
                    *posa=c;
                }
                else if('\t' == *posa) {
    	            posa++;
                }
                else {
            	    // *posb = *posa;
            	    cstring_strcat_imp(encoded, posa, 1);
               		++posa; ++posb;
                }
            }
            
            // found the ? now to the ending
            state = ENDING;
            
    		break;
    	}
    	case PARSE_B64: {
    		char *begin = NULL;
            char *end = NULL;
            int newlength = 0;
            begin = posa;
            end = (char*)strstr(posa, "?=");

            // convert the base64 area to text
            newlength = cmime_parse_b64_string(begin, end - begin);

            // copy over converted chars
            // memmove(posb, begin, newlength);
            cstring_strcat_imp(encoded, begin, newlength);

            posb+=newlength;
            *posb = 0; // cap the end..
            posa=end;
    		
    		state = ENDING;
    		
    		break;
    	}
    	case ENDING: {
    		
    		if(0 == memcmp(posa, "?=", 2)) {
    			// *posb=0;
    			// above line not converted to cstring cause it's already null term'd
    			++posa; ++posa;
    		}
    		
/*
    		if(0<strlen(encoding)) {
    			// convert encoded string here into UTF-8
    			UErrorCode err = U_ZERO_ERROR;
				int convertedSize = 0;
    			CSTRING* transcoded = NULL;

    			transcoded = cstring_new(encoded->length*2);
    			
    			convertedSize = ucnv_convert("utf-8", encoding, transcoded->string, transcoded->length, encoded->string, encoded->length, &err);
    		
    			if(err == U_BUFFER_OVERFLOW_ERROR) {
    				cstring_free(&transcoded);
    				transcoded = cstring_new(convertedSize);

					err=U_ZERO_ERROR;

	    			convertedSize = ucnv_convert("utf-8", encoding, transcoded->string, transcoded->memsize-1, encoded->string, encoded->length, &err);
    			}
    		
			    if(U_FAILURE(err)) {
		    	    cstring_strcat(final, encoded);
			    }
			    else {
			    	transcoded->length=convertedSize;
			        cstring_strcat(final, transcoded);
			    }
			    
			    cstring_free(&transcoded);
			    cstring_free(&encoded);
    		
    		} else {
*/
	    		cstring_strcat(final, encoded);
//    		}
    		
    		state = DEFAULT;
    		break;
    	}
		default: {
			if(0 == memcmp(posa, "=?", 2) && NULL!=(char*)strstr(posa, "?=")) {
				state = BEGINNING;
			}
			else { // copy non-encoded chars over.
				if('\t' != *posa && '\n' != *posa && '\r' != *posa) {
       	 			// *posb = *posa;
       	 			cstring_strcat_imp(final, posa, 1);
        			++posb;
      			}
      			++posa;
    		}
			break;
		}
	}
  }

//  fprintf(stderr, "SL: %s\n", subject);
  // *posb = 0; // cap the end

  // subject->length = posb - subject->string;

  return final;
}

#ifndef NOTEST
void Test__cmime_parse_subject(CuTest* tc)
{
}
#endif // NOTEST

