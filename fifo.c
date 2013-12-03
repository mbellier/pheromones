#include "fifo.h"
#include "msg.h"

#include <stdlib.h>

struct fifo__element{
    struct fifo__element *next;
    void *data;
};

struct fifo{
    struct fifo__element *head;
    struct fifo__element *tail;
    int size;
};

fifo *fifo__new(){
    fifo *q = malloc(sizeof(*q));
    if (NULL == q){
        ERROR_MSG(FIFO, "malloc fifo")
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

void fifo__free(fifo *queue){
    if (NULL == queue){
        ERROR_MSG(FIFO, "nothing to free")
        return;
    }
    struct fifo__element *e = queue->head;
    struct fifo__element *tmp;

    while (e != NULL){
        tmp = e->next;
        free(e);
        e = tmp;
    }
    free(queue);

}

int fifo__is_empty(fifo *queue){
    return queue->size == 0;
}

void *fifo__pop(fifo *queue){
    if (queue->size <= 0){
        DEBUG_MSG(FIFO, "fifo is empty")
        return NULL;
    }

    struct fifo__element *e = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    void *data = e->data;
    free(e);

    return data;
}

int fifo__push(fifo *queue, void *data){
    struct fifo__element *new_tail = malloc(sizeof(*new_tail));
    new_tail->next = NULL;
    new_tail->data = data;

    if (queue->tail != NULL){
        queue->tail->next = new_tail;
    }
    queue->tail = new_tail;

    if (queue->size == 0){
        queue->head = queue->tail;
    }

    queue->size++;

    return 0;
}


