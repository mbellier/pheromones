#include "ant.h"
#include "point2d.h"
#include <stdlib.h>
#include <stdio.h>
#include "msg.h"

#define ANT__DEBUG_MSG 1
#define ANT__ERROR_MSG 1

/*
3 2 1
4 x 0
5 6 7
*/

struct ant {
    int tag;
    point2d pos;
    int dir;
    int carrying_food;
    int sight;
};


ant *ant__new(point2d *pos, int sigth){
    ant *a = (ant *)malloc(sizeof(*a));
    if (a==NULL) {
        perror("malloc");
        return NULL;
    }
    a->tag = 0;
    a->pos = *pos;
    a->dir = 0;
    a->carrying_food = 0;
    a->sight = sigth;
    return a;
}

void ant__free(ant *a) {
    free(a);
}

void ant__set_tag(ant *a, int tag) {
    a->tag = tag;
}

int ant__get_tag(ant *a) {
    return a->tag;
}

void ant__get_pos(point2d *dest, ant *a){
    *dest = a->pos;
}

void ant__set_pos(ant *a, point2d *pos){
    a->pos = *pos;
}

int ant__get_dir(ant *a){
    return a->dir;
}

void ant__set_dir(ant *a, int dir){
    a->dir = dir;
}

int ant__get_carrying_food(ant *a){
    return a->carrying_food;
}

void ant__set_carrying_food(ant *a, int carrying_food){
    a->carrying_food = carrying_food;
}

void ant__turn(ant *a, int angle){
    a->dir += angle;
    a->dir %= 8;
    if (a->dir < 0) a->dir += 8;
}

void ant__turn_back(ant *a){
    a->dir += 4;
    a->dir %= 8;
}

void ant__move(ant *a, point2d *boundaries){
    point2d__pos_by_dir(&a->pos, &a->pos, boundaries, a->dir, 0);
}


