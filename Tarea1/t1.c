#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t1.h"

uint borrar_bits(uint x, uint pat, int len){
	int i;
	uint mask,mask2,resto;
    mask = ((1<<len)-1);
	for(i=0; i<=32-len ;i++){
        mask2 = mask<<(32-len-i);
        resto = (x & mask2)>>(32-len-i);
        if (resto == pat){
            x = x & ~mask2;
        }
    }
    return x;
}

char *reemplazo(char *s, char c, char *pal){
    int len_s,len_pal,count_c,len_malloc;
    len_s = 0;
    len_pal = 0;
    count_c = 0;
    len_malloc = 0;
    while(*s){
        if (c == *s){
            count_c++;
        }
        len_s++;
        s++;
    }
    while(*pal){
        len_pal++;
        pal++;
    }

    len_malloc = len_s + count_c * (len_pal-1);
    char *res = (char*)malloc(len_malloc);
    s -= len_s;
    pal -= len_pal;
    while(*s){
        if (c == *s){
            while(*pal){
                *res = *pal;
                res++;
                pal++;
            }
            pal -= len_pal;
        }
        else{
            *res = *s;
            res++;
        }
        s++;
    }
    res -= len_malloc;
    return res;
}

void reemplazar(char *s, char c, char *pal){
    int len_s,len_pal,count_c,len_malloc;
    char *puntero;
    len_s = 0;
    len_pal = 0;
    count_c = 0;
    printf("%s\n",s);
    printf("%s\n",puntero );
    while(*s){
        if (c == *s){
            count_c++;
        }
        len_s++;
        s++;
    }
    while(*pal){
        len_pal++;
        pal++;
    }
    if (len_pal>1){
        int i,j;
        puntero = s + count_c*(len_pal-1);
        for(i = 0;i<len_s;i++){
            if (c == *s){
                for(j = 0; j<len_pal; j++){
                  	pal--;
                    *puntero = *pal;
                    puntero--;
                }
                pal += len_pal;
                s--;         
            }
            else{
                *puntero = *s;
                s--;
                puntero--;
            }
            
        }
        *s = *puntero;
        printf("%s\n", s);
    }
    else if(len_pal == 1){
    	for(i = 0;i<len_s;i++){
    		if (c == *s){
            	for(j = 0; j<len_pal; j++){
                	pal--;
                	*s = *pal;
                	s--;
            	}
            	pal += len_pal;
      			s--;
        	}
        	else{
        		s--;
        	}
    	}
    }
    //
}