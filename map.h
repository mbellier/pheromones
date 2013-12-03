#ifndef MAP_H_
#define MAP_H_

#include "point2d.h"
#include "pheromone.h"

#define MAP__DEBUG_MSG 1
#define MAP__ERROR_MSG 1

typedef struct map__successors map__successors;

//enum map__value {map__empty = 0, map__wall, map__food};

#define map__empty 0
#define map__wall  1
#define map__food  2

pheromone       *map__new              (int size_x, int size_y);
pheromone       *map__compute_nest_map (pheromone *map, point2d *nest);
int              map__is_wall          (pheromone *map, int x, int y);
int              map__is_food          (pheromone *map, int x, int y);
map__successors *map__successors_new   (void);
void             map__successors_free  (map__successors *s);
int              map__get_successors   (map__successors *dest, pheromone *map, int x, int y);
void             map__successor        (point2d *dest, map__successors *successors, int i);
int              map__successors_nb    (map__successors *successors);

#endif
