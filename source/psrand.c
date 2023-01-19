//#define VERBOSE
//#define RAND_RET
#ifndef _FIX_H
#include "psrand.h"
#endif
#include <stdio.h>

unsigned int d_rand_seed = 1;
#ifndef _FIX_H
typedef int fix;
#endif
extern fix GameTime;

int psrand(void) {
	#ifdef VERBOSE
	#ifdef RAND_RET
	//volatile int v = 0;
	void *addr = __builtin_extract_return_addr (__builtin_return_address (0));
	#endif
	printf("rand GameTime %08x seed %08x"
	        #ifdef RAND_RET
	        " ret %08x"
	        #endif
		"\n", GameTime, d_rand_seed
	        #ifdef RAND_RET
	        , addr //(&v)[5]
	        #endif
		);
	#endif
	return ((d_rand_seed = d_rand_seed * 0x41c64e6d + 0x3039) >> 16) & 0x7fff;
}

void pssrand(unsigned int seed) {
	d_rand_seed = seed;
}

int pal_rand_seed = 1;
int psrand_pal(void) {
	return ((pal_rand_seed = pal_rand_seed * 0x41c64e6d + 0x3039) >> 16) & 0x7fff;
}
