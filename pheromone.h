#ifndef PHEROMONE_H_
#define PHEROMONE_H_

#include "point2d.h"
#include "rgb_color.h"

typedef struct pheromone pheromone;

pheromone *pheromone__new         (int x, int y, float evaporation_rate);
void       pheromone__free        (pheromone *p);
void       pheromone__init        (pheromone *p, float value);
int        pheromone__save        (pheromone *p, char *file_path);
int        pheromone__load        (pheromone *p, char *file_path);
void       pheromone__evaporate   (pheromone *p);
int        pheromone__size_x      (pheromone *p);
int        pheromone__size_y      (pheromone *p);
void       pheromone__get_size    (point2d *dest, pheromone *p);
int        pheromone__copy_values (pheromone *s, pheromone *d);
float      pheromone__get         (pheromone *p, int x, int y);
int        pheromone__set         (pheromone *p, int x, int y, float value);
void       pheromone__find_around (point2d *dest, pheromone *p, int x, int y);
void       pheromone__debug_print (pheromone *p);
void       pheromone__set_color   (pheromone *p, int r, int g, int b);
void       pheromone__get_color   (rgb_color *dest, pheromone *p);
void       pheromone__set_transparency(pheromone *p, float value);
float      pheromone__get_transparency(pheromone *p);

#endif
