#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include "jsocket.h"
#include "util.h"


pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

char* nombre_ocupante[]={"*","*","*","*","*"};
int est[5] = {0, 0, 0, 0, 0};

void morir(){
    fprintf(stderr,"Llego SIGPIPE");
}

typedef void *(*Thread_fun)(void *);

int ubicar(int k){
    if (k == 1){
        int i = 0;
        for (; i < 5; i++){
            if (est[i] == 0){
                return i;
            }
        } 
    }
    else if (k == 2){
        int i = 0;
        for(; i < 5; i++){
            if (i == 4){
                return -1;
            }
            if (est[i] == 0 && est[i+1] == 0){
                return i;
            }
        }
    }
    else if (k == 3) {
        int i = 0;
        for(; i < 5; i++){
            if (i == 3){
                return -1;
            }
            if (est[i] == 0 && est[i+1] == 0 && est[i+2] == 0){
                return i;
            }
        }   
    }
    else if (k == 5){
        int i = 0;
        for(; i < 5; i++){
            if (i == 1){
                return -1
            }
            if (est[i] != 0){
                return -1
            }
        }
    }
    return -1;
}

int reservar(char *nom, int k) {
    int len = strlen(nom);
    char *nombre = malloc(sizeof(char)*len+1);
    strcpy(nombre,nom);
    pthread_mutex_lock(&m);
    int val = ubicar(k);
    while (val == -1) {
        pthread_cond_wait(&c, &m);
    }
    for(int i=val;i<val+k;i++){
        nombre_ocupante[i] = nombre;
        est[i] = 1;
    }
    pthread_mutex_unlock(&m);
    return val;
}

void liberar(char* nom){
    pthread_mutex_lock(&m);
    for(int i=0;i<5;i++){
        if(strcmp(nom,nombre_ocupante[i])==0){
            nombre_ocupante[i] = "*";
            est[i] = 0;
        }
    }
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}


void * serv(void* s){
    int pos;
    char num;
    char* nombre = getstr(s);
    char* accion = getstr(s);
    if(strcmp(accion,"r")){
        leer(s,&num,1);
        pos = reservar(nombre,atoi(num));
        write(s,&pos,sizeof(int));
        imprimir();
    }
    else{
        liberar(nombre);
        imprimir();
    }
    close(s);
    return NULL;
}

void imprimir(){
    for(int i = 0; i < 5; i++){
        if(est[i] == 0){
            printf("%d",i);
        }
        else{
            printf("*");
        }
    }
    printf("\n");
}

int main(int argc,char** argv){
    int s,s2;
    pthread_t pid;
    signal(SIGPIPE,morir);
    s = j_socket();
    int port = atoi(argv[1]);

    if(j_bind(s,port) < 0){
        fprintf(stderr,"Fallo bind \n");
        exit(1);
    }

    for(;;){
        pthread_attr_t attr;
        s2 = j_accept(s);
        pthread_attr_init(&attr);

        if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED) != 0){
            fprintf(stderr,"Fallo Detachable");
            exit(1);
        }

        if(pthread_create(&pid, &attr, (Thread_fun) serv,(void*) s2) != 0){
            fprintf(stderr,"No pude crear thread para nuevo cliente %d \n", s2);
            exit(1);
        }
        pthread_attr_destroy(&attr);
    }
    return 0;
}