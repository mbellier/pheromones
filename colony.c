#include "colony.h"
#include "pheromone.h"
#include "ant.h"
#include "ant_ai.h"
#include "map.h"
#include "display.h"
#include "point2d.h"
#include "random.h"
#include "maths.h"
#include <stdlib.h>
#include <stdio.h>


#define COLONY__INIT_ANT_NB_MAX 10


struct colony {
    point2d size;
    point2d nest;

    pheromone *map;
    pheromone *nest_map;
    pheromone *food;
    pheromone *scent;

    int sight;

    ant **ants;
    int ant_nb;
    int ant_nb_max;

    rgb_color ant_color;

    int tag;
};

int colony__resize_ant_array(colony *c, int needed_ant_nb) {
    if (needed_ant_nb >= c->ant_nb_max) {
        while(c->ant_nb_max < needed_ant_nb) {
            c->ant_nb_max *= 2;

        }
        c->ants = (ant **)realloc(c->ants, sizeof(ant*) * c->ant_nb_max);
        if (c->ants == NULL) {
            perror("malloc");
            return -1;
        }
    }
    return 0;
}

int colony__new_ant(colony *c) {
    if (colony__resize_ant_array(c, c->ant_nb + 1) != 0) return -1;

    c->ants[c->ant_nb] = ant__new(&(c->nest), c->sight);
    ant__set_tag(c->ants[c->ant_nb], (c->tag)++);
    c->ant_nb++;

/*
    point2d pos;
    pos.x = random__int() % c->size.x;
    pos.y = random__int() % c->size.y;
    ant__set_pos(c->ants[c->ant_nb-1], &pos);
*/

    return 0;
}

int colony__del_ant(colony *c, int id) {
    if (id<0 || id >= c->ant_nb) return -1;

    ant__free(c->ants[id]);
    if (id < c->ant_nb - 1) {
        c->ants[id] = c->ants[c->ant_nb-1];
    }
    c->ant_nb--;
    DEBUG_MSG(COLONY, "%d ants in the colony (-1)", c->ant_nb)
    return 0;
}

colony *colony__new(int nest_x, int nest_y, pheromone *map, int sight, float evaporation_rate) {
    DEBUG_MSG(COLONY,"creating new colony")
    colony *c = (colony *)malloc(sizeof(*c));
    if (c==NULL) {
        ERROR_MSG(COLONY, "malloc failure at creation of colony");
        return NULL;
    }

    int size_x = pheromone__size_x(map);
    int size_y = pheromone__size_y(map);

    pheromone *scent = pheromone__new(size_x, size_y, evaporation_rate);
    if (scent == NULL) {
        ERROR_MSG(COLONY,"new pheromone failure at creation of colony")
        free(c);
        return NULL;
    }

    c->size.x = size_x;
    c->size.y = size_y;
    c->nest.x = nest_x;
    c->nest.y = nest_y;

    c->map   = map;
    c->nest_map = NULL;

    //c->nest_map = map__compute_nest_map(map, nest_x, nest_y);

    c->scent = scent;
    c->sight = sight;

    c->ant_nb = 0;
    c->ant_nb_max = 1;
    c->ants=NULL;
    if (colony__resize_ant_array(c, COLONY__INIT_ANT_NB_MAX)){
        ERROR_MSG(COLONY, "resize failure at creation");
        c->ant_nb_max=0;
        colony__free(c);
        return NULL;
    };

    c->tag = 0;
    rgb_color__set(&(c->ant_color), COLONY__DEFAULT_ANT_COLOR);

    pheromone__set_color(c->scent, COLONY__DEFAULT_SCENT_COLOR);

    DEBUG_MSG(COLONY, "Success.")

    return c;
}

void colony__compute_nest_map(colony *c){
    c->nest_map = map__compute_nest_map(c->map, &c->nest);
}

int colony__load_nest_map(colony *c, char *file_path){
    c->nest_map = map__new(c->size.x, c->size.y);
    return pheromone__load(c->nest_map, file_path);
}

void colony__free(colony *c) {
    pheromone__free(c->scent);
    pheromone__free(c->nest_map);
    int i;
    for (i=0; i< c->ant_nb; i++) { // Even if the array size is ant_nb_max, only ant_nb are allocated.
        ant__free(c->ants[i]);
    }
    free(c->ants);
    free(c);
}

pheromone *colony__nest_map(colony *c){
    return c->nest_map;
}

pheromone *colony__scent_pheromone(colony *c){
    return c->scent;
}

void colony__draw_ants(colony *c){
    int i;
    for (i=0; i < c->ant_nb; i++){
        display__draw_ant(c->ants[i], &c->ant_color);
    }
}

void colony__ant_ai(colony *c){
    int i;
    for (i=0; i < c->ant_nb; i++){
        ant_ai__algorithm(c->ants[i], &c->nest, c->map, c->nest_map, c->scent);
    }
    pheromone__evaporate(c->scent);
}

void colony__count_ants(int *ants_nb, int *ants_with_food_nb, colony *c, int x, int y){
    int i, sum = 0, food = 0;
    point2d pos;
    for (i=0; i < c->ant_nb; i++){
        ant__get_pos(&pos, c->ants[i]);
        if (pos.x == x && pos.y == y){
            sum++;
            if (ant__get_carrying_food(c->ants[i])) food++;
        }
    }
    *ants_nb = sum;
    *ants_with_food_nb = food;
}
