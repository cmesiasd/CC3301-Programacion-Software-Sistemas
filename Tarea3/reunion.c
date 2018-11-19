#include <pthread.h>
#include <stdlib.h>
#include "reunion.h"

struct reunion {
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

Reunion *nuevaReunion(){
    Reunion *reu = (Reunion*)malloc(sizeof(Reunion));
    reu->in = 0;
    reu->out = 0;
    pthread_mutex_init(&reu->mutex, NULL);
    pthread_cond_init(&reu->cond, NULL);
    return reu;
}

void entrar(Reunion *r){
    pthread_mutex_lock(&r->mutex);
    r->in++;
    pthread_mutex_unlock(&r->mutex);
}

void concluir(Reunion *r){
    pthread_mutex_lock(&r->mutex);
    r->out++;
    while (r->in > r->out)
      pthread_cond_wait(&r->cond, &r->mutex);
    pthread_cond_broadcast(&r->cond);
    pthread_mutex_unlock(&r->mutex);
}