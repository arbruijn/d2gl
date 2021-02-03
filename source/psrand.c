#include "psrand.h"
#include <stdio.h>

unsigned int d_rand_seed = 1;
extern int GameTime;

int psrand(void) {
	printf("rand GameTime %08x seed %08x\n", GameTime, d_rand_seed);
	return ((d_rand_seed = d_rand_seed * 0x41c64e6d + 0x3039) >> 16) & 0x7fff;
}

void pssrand(unsigned int seed) {
	d_rand_seed = seed;
}
