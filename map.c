#include "map.h"
#include <stdlib.h>
#include "msg.h"
#include "fifo.h"
#include "point2d.h"
#include "maths.h"

#define MAP__MAX_SUCCESSORS 8

struct map__successors{
    int nb;
    point2d *successors;
};

map__successors *map__successors_new(void){
    map__successors *s = malloc(sizeof(*s));
    if(NULL==s) ERROR_MSG(MAP, "new successor struct (malloc struct)")
    s->successors = malloc(sizeof(*(s->successors))* MAP__MAX_SUCCESSORS);
    if(NULL==s) ERROR_MSG(MAP, "new successor struct (malloc array")

    s->nb = 0;
    int i;
    for(i=0; i<MAP__MAX_SUCCESSORS; i++){
        s->successors[i].x = 0;
        s->successors[i].y = 0;
    }

    return s;
}

void map__successors_free(map__successors *s){
    free(s->successors);
    free(s);
}

void map__successor(point2d *dest, map__successors *successors, int i){
    if (i>=0 && i<successors->nb){
        *dest = successors->successors[i];
    }
}

int map__successors_nb(map__successors *successors){
    return successors->nb;
}

int map__get_successors(map__successors *dest, pheromone *map, int x, int y){
    point2d boundaries, pos;
    boundaries.x = pheromone__size_x(map);
    boundaries.y = pheromone__size_y(map);

    dest->nb = 0;
    int dir;
    for (dir = 0; dir < 8; dir ++) {
        pos.x = x;
        pos.y = y;
        point2d__pos_by_dir(&pos, &pos, &boundaries, dir, 0);
        if (pos.x != x || pos.y != y) {
            if (!map__is_wall(map, pos.x, pos.y)) {
                dest->successors[(dest->nb)++] = pos;
            }
        }
    }

    return 0;
}

pheromone *map__new(int size_x, int size_y) {
    return pheromone__new(size_x, size_y, (float) map__empty);
}

int map__is_wall(pheromone *map, int x, int y) {
    return map__wall == (int)pheromone__get(map, x, y);
}

int map__is_food(pheromone *map, int x, int y) {
    return map__food == (int)pheromone__get(map, x, y);
}



/*
pheromone *map__compute_nest_map(pheromone *map, point2d *nest) {
    DEBUG_MSG(MAP, "computing nest map...")
    int size_x = pheromone__size_x(map);
    int size_y = pheromone__size_y(map);

    DEBUG_MSG(MAP, "%d x %d", size_x, size_y)

    int nest_x = nest->x, nest_y = nest->y;

    if (map__is_wall(map, nest_x, nest_y)){
        pheromone__set(map,nest_x, nest_y, (float) map__empty);
    }

    pheromone *nest_map = map__new(size_x, size_y);
    if (NULL == nest_map) {
        perror("malloc");
        return NULL;
    }
    pheromone *buffer = map__new(size_x, size_y);
    if (NULL == nest_map) {
        perror("malloc");
        pheromone__free(nest_map);
        return NULL;
    }
    pheromone__set(nest_map, nest_x, nest_y, 1);

    int i,j;
    int nb_unvisited_cells = 0, nb_unvisited_cells_prev = 1;
    unsigned long max_value = 0, iterations = 0;
    point2d found_pos;

    while (nb_unvisited_cells != nb_unvisited_cells_prev) {
        max_value++;
        nb_unvisited_cells_prev=nb_unvisited_cells;
        nb_unvisited_cells = 0;
        pheromone__copy_values(buffer, nest_map);

        for (i = 0; i<size_x; i++) {
            for (j=0; j<size_y; j++) {
                iterations ++;
                if (!map__is_wall(map, i, j)) {
                    pheromone__find_around(&found_pos, nest_map, i, j);
                    if (found_pos.x >= 0) {
                        pheromone__set(buffer, i, j, pheromone__get(nest_map, i, j) + 1);
                    } else {
                        nb_unvisited_cells++;
                    }
                }
            }
        }
        pheromone__copy_values(nest_map, buffer);
    }
    pheromone__free(buffer);
    DEBUG_MSG(MAP, "nest_map computed in %ld iterations.", iterations)

    pheromone__set(nest_map, nest_x, nest_y, ++max_value);

    for (i = 0; i<size_x; i++) {
        for (j=0; j<size_y; j++) {
            pheromone__set(nest_map, i, j, pheromone__get(nest_map, i, j)/max_value);
        }
    }

    DEBUG_MSG(MAP, "done.")
    return nest_map;
}
//*/

//*
pheromone *map__compute_nest_map(pheromone *map, point2d *nest) {
    DEBUG_MSG(MAP, "computing nest map...")
    int size_x = pheromone__size_x(map);
    int size_y = pheromone__size_y(map);

    DEBUG_MSG(MAP, "%d x %d", size_x, size_y)

    int nest_x = nest->x, nest_y = nest->y;

    if (map__is_wall(map, nest_x, nest_y)){
        pheromone__set(map,nest_x, nest_y, (float) map__empty);
    }

    pheromone *nest_map = map__new(size_x, size_y);
    if (NULL == nest_map) {
        perror("malloc");
        return NULL;
    }
    pheromone__init(nest_map, -1);

    unsigned long current_value = 0, max_value = 0, iterations = 0;

    fifo *q = point2d__queue_new();

    point2d pos, suc;
    pos.x = nest_x; pos.y = nest_y;
    pheromone__set(nest_map, pos.x, pos.y, current_value);
    point2d__queue_push(q, &pos);

    map__successors *successors = map__successors_new();
    int i, k, tab[8];
    while(!point2d__queue_is_empty(q)){
        point2d__queue_pop(q, &pos);
        current_value = pheromone__get(nest_map, pos.x, pos.y);
        max_value = MAX(current_value, max_value);
        map__get_successors(successors, map, pos.x, pos.y);

        for (i=0; i<successors->nb; i++){tab[i] = i;}
        maths__permutation(tab, successors->nb);

        for (k = 0; k<map__successors_nb(successors); k++){
            i = tab[k];
            iterations++;
            map__successor(&suc, successors, i);
            if (!map__is_wall(map, suc.x, suc.y)){
                float value = pheromone__get(nest_map, suc.x, suc.y);
                if (value <= 0){
                    point2d__queue_push(q, &suc);
                    pheromone__set(nest_map, suc.x, suc.y, current_value + 1);
                }else{
                    pheromone__set(nest_map, suc.x, suc.y, MIN(value, current_value + 1));
                }
            }
        }

    }

    DEBUG_MSG(MAP, "nest_map computed in %ld iterations.", iterations)

    point2d__queue_free(q);
    map__successors_free(successors);

    pheromone__set(nest_map, nest_x, nest_y, ++max_value);
    int j;
    for (i = 0; i<size_x; i++) {
        for (j = 0; j<size_y; j++) {
            float value = pheromone__get(nest_map, i, j);
            if (value >= 0){
                pheromone__set(nest_map, i, j, (max_value - value) / max_value);
            }else{ // innaccessible zones
                pheromone__set(nest_map, i, j, -1);
            }
        }
    }
    pheromone__set(nest_map, nest_x, nest_y, 1);

    DEBUG_MSG(MAP, "done.")
    return nest_map;
}

//*/
