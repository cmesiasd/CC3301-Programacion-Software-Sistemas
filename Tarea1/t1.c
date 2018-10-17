#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t1.h"

uint borrar_bits(uint x, uint pat, int len){
	int i;
	for(i=0; i<=32-len ;i++){
        uint mask;
        uint mask2;
        uint resto;
        mask = ((1<<len)-1);
        mask2 = mask<<(32-len-i);
        resto = (x & mask2)>>(32-len-i);
        if (resto == pat){
            x = x & ~mask2;
        }
        else{
            mask >>= 1;
        }
    }
    return x;
}