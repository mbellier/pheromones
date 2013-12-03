#ifndef MISC_H_
#define MISC_H_

#include "fifo.h"
#include "msg.h"

#define POINT2D__DEBUG_MSG 0
#define POINT2D__ERROR_MSG 1

#define MIN(a, b) (a < b) ? a : b
#define MAX(a, b) (a > b) ? a : b



struct point2d {
    int x;
    int y;
};

typedef struct point2d point2d;

int point2d__equal(point2d *a, point2d *b);
//void point2d__substract(point2d *dest, point2d *a, point2d *b);

void point2d__pos_by_dir(point2d *dest, point2d *pos, point2d *boundaries, int dir, int cyclic);
int point2d__dir_by_pos(point2d *from, point2d *to);

fifo *point2d__queue_new();
void point2d__queue_free(fifo *q);
void point2d__queue_push(fifo *q, point2d *p);
void point2d__queue_pop(fifo *q, point2d *dest);
int point2d__queue_is_empty(fifo *q);


#endif
