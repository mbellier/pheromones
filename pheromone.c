#include "pheromone.h"
#include "matrix.h"
#include "point2d.h"
#include "msg.h"
#include <stdlib.h>
#include <stdio.h>

#define PHEROMONE__DEFAULT_COLOR_RGB 255, 255, 255

#define PHEROMONE__DEBUG_MSG 1
#define PHEROMONE__ERROR_MSG 1

struct pheromone {
    float_matrix *matrix;
    float evaporation_rate;
    rgb_color color;
    float transparency;
};

int pheromone__save(pheromone *p, char *file_path){
    return float_matrix__save(p->matrix, file_path);
}

int pheromone__load(pheromone *p, char *file_path){
    p->matrix = float_matrix__open(file_path);
    if (NULL == p->matrix){
        return 1;
    }
    return 0;
}

pheromone *pheromone__new(int x, int y, float evaporation_rate) {
    pheromone *p = (pheromone *)malloc(sizeof (*p));
    if (p==NULL) {
        perror("malloc");
        return NULL;
    }
    p->matrix = float_matrix__new(x, y);
    float_matrix__init(p->matrix, 0);
    p->evaporation_rate = evaporation_rate;
    rgb_color__set(&(p->color), PHEROMONE__DEFAULT_COLOR_RGB);
    p->transparency = -1;
    return p;
}

void pheromone__init(pheromone *p, float value){
    float_matrix__init(p->matrix, value);
}

void pheromone__free(pheromone *p) {
    float_matrix__free(p->matrix);
    free(p);
}

void pheromone__evaporate(pheromone *p) {
    if (p->evaporation_rate == 0) return;
    int i, j;
    for (i=0; i < float_matrix__size_x(p->matrix); i++) {
        for (j=0; j < float_matrix__size_y(p->matrix); j++) {
            float value = float_matrix__get(p->matrix, i, j);
            if (value > 0) {
                float_matrix__set(p->matrix, i, j, value - p->evaporation_rate);
            }
        }
    }
}

int pheromone__size_x(pheromone *p) {
    return float_matrix__size_x(p->matrix);
}
int pheromone__size_y(pheromone *p) {
    return float_matrix__size_y(p->matrix);
}

int pheromone__copy_values(pheromone *d, pheromone *s) {
    return float_matrix__copy(d->matrix, s->matrix);
}

float pheromone__get(pheromone *p, int x, int y) {
    return float_matrix__get(p->matrix, x, y);
}
int pheromone__set(pheromone *p, int x, int y, float value) {
    return float_matrix__set(p->matrix, x, y, value);
}

/**
 * Returns the position of the first (by trigonometric order) cell with a strictly positive value.
 */
void pheromone__find_around(point2d *dest, pheromone *p, int x, int y) {
    point2d boundaries, pos;
    pheromone__get_size(&boundaries, p);

    int dir;
    for (dir = 0; dir < 8; dir ++) {
        pos.x = x;
        pos.y = y;
        point2d__pos_by_dir(&pos, &pos, &boundaries, dir, 0);
        if (pos.x != x || pos.y != y) {
            if (pheromone__get(p, pos.x, pos.y) > 0) {
                *dest = pos;
                return;
            }
        }
    }
    pos.x=-1;
    pos.y=-1;

    *dest = pos;
}


void pheromone__debug_print(pheromone *p) {
    float_matrix__debug_print(p->matrix);
}

void pheromone__set_color(pheromone *p, int r, int g, int b){
    p->color.r = r;
    p->color.g = g;
    p->color.b = b;
}

void pheromone__get_color(rgb_color *dest, pheromone *p){
    *dest = p->color;
}



void pheromone__set_transparency(pheromone *p, float value){
    p->transparency = value;
}

float pheromone__get_transparency(pheromone *p){
    return p->transparency;
}

void pheromone__get_size(point2d *dest, pheromone *p){
    dest->x = pheromone__size_x(p);
    dest->y = pheromone__size_y(p);
}
