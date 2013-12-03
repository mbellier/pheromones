#ifndef ANT_AI_H_
#define ANT_AI_H_

#include "ant.h"
#include "point2d.h"
#include "pheromone.h"
#include "msg.h"

#define ANT_AI__DEBUG_MSG 0
#define ANT_AI__ERROR_MSG 1

void ant_ai__turn_to(ant *a, point2d *to);
int  ant_ai__dir_to(ant *a, point2d *to);
int  ant_ai__distance_to(ant *a, point2d *to);
int  ant_ai__collision_move(ant *a, pheromone *map);
int  ant_ai__random_move(ant *a, pheromone *map);
int  ant_ai__follow_pheromone(ant *a, pheromone *map, pheromone *p);
int  ant_ai__follow_nest(ant *a, point2d *nest, pheromone *map, pheromone *nest_map);
int  ant_ai__load_food(ant *a, pheromone *map);
int  ant_ai__unload_food(ant *a, point2d *nest);
void ant_ai__algorithm(ant *a, point2d *nest, pheromone *map, pheromone *nest_map, pheromone *scent);

#endif
