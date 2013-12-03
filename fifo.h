#ifndef FIFO_H_
#define FIFO_H_

#define FIFO__DEBUG_MSG 1
#define FIFO__ERROR_MSG 1

typedef struct fifo fifo;

fifo *fifo__new      ();
void  fifo__free     (fifo *queue);
int   fifo__is_empty (fifo *queue);
void *fifo__pop      (fifo *queue);
int   fifo__push     (fifo *queue, void *data);

#endif
