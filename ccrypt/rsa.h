#define MAXKEYBYTES 56          /* 448 bits */

unsigned int R(unsigned int x);
void rsa_encypher(unsigned int *xl, unsigned int *xr);
void rsa_decypher(unsigned int *xl, unsigned int *xr);
short rsa_initialize(void);
