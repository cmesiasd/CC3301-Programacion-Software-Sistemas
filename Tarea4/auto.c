#include "jsocket.h"
#include "util.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


int getPort(char* env){
    char* aux=env;
    while((*aux)!=':'){
        aux++;
    }
    aux++;
    int res;
    int count = 0;
    char* aux2;
    while(*aux){
        *aux2 = *aux;
        aux++;
        aux2++;
        count++;
    }
    aux2 -= count;
    res = atoi(aux2)/10;
    return res;
}

int largo_host(char* env){
    char* aux=env;
    int i=0;
    while((*aux)!=':'){
        i++;
        aux++;
    }
    return i;
}

void copiar_host(char** s1,char* s2){
    char* ps1=*s1;
    char* aux=s2;
    while((*aux)!=':'){
        *ps1=*aux;
        aux++;
        ps1++;
    }
    *ps1=0;
}


int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr,"Introduzca bien los datos!");
        exit(1);
    }
    int s = j_socket();
    char* ambiente = getenv("PARKING");
    int lhost = largo_host(ambiente);
    char* host = malloc(lhost+1);
    copiar_host(&host,ambiente);
    int port = getPort(ambiente);

    if(j_connect(s,host,port) < 0){
        fprintf(stderr,"Falló conexión");
        exit(1);
    }
    int pos;
    int num;
    char* nombre=argv[1];
    char* op=argv[2];
    sendstr(s,nombre);
    sendstr(s,op);
    if(argc==4){
        num = atoi(argv[3]);
        write(s,&num,sizeof(num));
        leer(s,&pos,sizeof(pos));
        printf("%i\n",pos);
    }
    close(s);
    free(host);
    return 0;
}