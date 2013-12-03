#include "point2d.h"
#include <stdlib.h>

/*
3 2 1
4 x 0
5 6 7
*/

/**
 * This function returns the coordinates of a 2D point, given an other point and a trigonometric direction (from 0 to 7).
 * /param "point2d *dest" Pointer to the output 2D point, modified only at the end of the function.
 * /param "point2d *pos" Pointer to the input 2D point.
 * /param "point2d *boundaries" Pointer to a 2D point containing the boundaries, i.e. output point cannot exceed them (min is (0; 0)).
 * /param "int dir" The trigonometric direction, from 0 (right) to 7 (right and below).
 * /param "int cyclic" Boolean. If the output exceeds to boundaries, it moves to the opposite (if cyclic) or remains before the boundaries (if !cyclic).
 */
void point2d__pos_by_dir(point2d *dest, point2d *pos, point2d *boundaries, int dir, int cyclic) {
    point2d r;
    r.x = pos->x;
    r.y = pos->y;

    switch (dir) {
    case 0:
        r.x ++;
        break;
    case 1:
        r.x++;
        r.y++;
        break;
    case 2:
        r.y++;
        break;
    case 3:
        r.x--;
        r.y++;
        break;
    case 4:
        r.x--;
        break;
    case 5:
        r.x--;
        r.y--;
        break;
    case 6:
        r.y--;
        break;
    case 7:
        r.y--;
        r.x++;
        break;
    }

    if (r.x >= boundaries->x) {
        r.x = cyclic ? 0 : boundaries->x - 1;
    }
    if (r.y >= boundaries->y) {
        r.y = cyclic ? 0 : boundaries->y - 1;
    }
    if (r.x < 0) {
        r.x = cyclic ? boundaries->x - 1 : 0;
    }
    if (r.y < 0) {
        r.y = cyclic ? boundaries->y - 1 : 0;
    }

    *dest = r;
}

int point2d__dir_by_pos(point2d *from, point2d *to){
    int x = to->x - from->x;
    int y = to->y - from->y;

    if (y == 0){
        if (x >= 0) return 0;
        else return 4;
    }else if (y>0){
        if (x==0) return 2;
        else if (x>0) return 1;
        else return 3;
    }else{
        if (x==0) return 6;
        else if (x>0) return 7;
        else return 5;
    }
}


int point2d__equal(point2d *a, point2d *b){
    DEBUG_MSG(POINT2D, "%d %d == %d %d", a->x,a->y , b->x, b->y)
    return ((a->x == b->x) && (a->y == b->y));
}

fifo *point2d__queue_new(){
    return fifo__new();
}

void point2d__queue_free(fifo *q){
    point2d *p;
    while (!fifo__is_empty(q)){
        p = (point2d *) fifo__pop(q);
        free(p);
    }
    fifo__free(q);
}

void point2d__queue_push(fifo *q, point2d *p){
    point2d *ptr = malloc(sizeof(*ptr));
    *ptr = *p;
    fifo__push(q, ptr);
}

void point2d__queue_pop(fifo *q, point2d *dest){
    point2d *p = (point2d *) fifo__pop(q);
    *dest = *p;
    free(p);
}

int point2d__queue_is_empty(fifo *q){
    return fifo__is_empty(q);
}
