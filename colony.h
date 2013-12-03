#ifndef COLONY_H_
#define COLONY_H_

#include "pheromone.h"
#include "msg.h"

#define COLONY__DEBUG_MSG 1
#define COLONY__ERROR_MSG 1

#define COLONY__DEFAULT_ANT_COLOR   0, 0, 255
#define COLONY__DEFAULT_SCENT_COLOR 255, 0, 0

struct colony;
typedef struct colony colony;

int        colony__resize_ant_array (colony *c, int needed_ant_nb);
int        colony__new_ant          (colony *c);
int        colony__del_ant          (colony *c, int id);
colony    *colony__new              (int nest_x, int nest_y, pheromone *map, int sight, float evaporation_rate);
void       colony__free             (colony *c);
pheromone *colony__nest_map         (colony *c);
pheromone *colony__scent_pheromone  (colony *c);
void       colony__compute_nest_map (colony *c);
int        colony__load_nest_map    (colony *c, char *file_path);
void       colony__draw_ants        (colony *c);
void       colony__ant_ai           (colony *c);
void       colony__count_ants       (int *ants_nb, int *ants_with_food_nb, colony *c, int x, int y);

#endif
