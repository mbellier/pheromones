#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "ant.h"
#include "pheromone.h"
#include "colony.h"
#include "map.h"
#include "display.h"
#include "events.h"
#include "random.h"
#include "fifo.h"
#include "maths.h"



void __map__(pheromone *map, int size_x, int size_y){
    int k;
    for (k=0; k<size_x / 2; k++) {
        pheromone__set(map, k+size_x / 4,size_x / 4,1);
    }

    for (k=0; k<size_y / 2; k++) {
        pheromone__set(map, size_x / 4,size_x / 4+k,1);
    }
    for (k=0; k<size_y / 4; k++) {
        pheromone__set(map, size_x / 4 * 3,size_x / 4 +k,1);
    }

    for (k=0; k<size_x * 4 / 12 ; k++) {
        pheromone__set(map, k+(size_x * 7 / 12),size_x / 2,1);
    }

    for (k=0; k<size_x / 2 - 1; k++) {
        pheromone__set(map, k,size_y - k,1);
        pheromone__set(map, k,size_y - k - 1,1);
    }
}

void food_spot(pheromone *map, int x, int y, int r){
    int i,j;
    for (i=0; i<r; i++){
        for (j=0; j<r; j++){
            pheromone__set(map, x+i-y/2, y+j-y/2, map__food);
        }
    }
}


int main (int argc, char **argv) {
    random__init();
/*
    printf("%d\n", random__range(10,20));

    int test[8]={0,1,2,3,4,5,6,7};
    int t;
    maths__permutation(test, 7);
    for (t=0; t<8; t++){printf("%d ", test[t]);}
*/


    int size_x = 400;
    int size_y = 400;
    int width  = 2;
    int height = 2;
    int screen_x = size_x * width;
    int screen_y = size_y * height;
    int nest_x = size_x/2;
    int nest_y = size_y/2;

    pheromone *map = map__new(size_x,size_y);
    pheromone__set_color(map, 0,150,0);
    int k, x, y;
    for (k=0; k<20; k++) {
        x = random__range(0,size_x); y = random__range(0,size_y);
        food_spot(map, x, y, 20);
    }
    __map__(map, size_x, size_y);

    colony *c = colony__new(nest_x, nest_y,map,1,0.001);
    colony__compute_nest_map(c);

    //colony__load_nest_map(c, "saves/nest_map_500x500.dat");
    pheromone *nest_map = colony__nest_map(c);
    //pheromone__save(nest_map, "saves/nest_map.dat");
    pheromone__set_transparency(nest_map, 0);

    pheromone *scent = colony__scent_pheromone(c);
    pheromone__set_transparency(scent, 0);


    int i;
    for (i=0; i<10000; i++) {
        colony__new_ant(c);
    }


    display__init(screen_x,screen_y);
    display__set_draw_resolution(width, height);
    display__set_draw_offset(-(screen_x/2 - (size_x/2 * width))/width, -(screen_y/2 - (size_y/2 * height))/height );

    events__draw_nest = 1;
    int mx, my, ants, food;
    while (display__loop(events__handle)) {
        display__begin_drawing();
        if (events__draw_nest) display__draw_pheromone(nest_map, DISPLAY__PHEROMONE_MODE);
        display__draw_pheromone(map, DISPLAY__MAP_MODE);
        display__draw_pheromone(scent, DISPLAY__PHEROMONE_MODE);
        colony__draw_ants(c);
        display__end_drawing();

        if (!events__pause) colony__ant_ai(c);

        if (events__mouse){
            mx = events__mouse_pos.x; my = events__mouse_pos.y;
            printf("Informations about cell (%3d; %3d) : ", mx, my);
            if (mx == nest_x && my == nest_y) printf("[NEST] ");
            if (map__is_wall(map, mx, my)) printf("[WALL] ");
            if (map__is_food(map, mx, my)) printf("[FOOD] ");
            colony__count_ants(&ants, &food, c, mx, my);
            if (ants) {
                printf("[ANTS (%d)", ants);
                if (food) printf(" (food : %d)", food);
                printf("]");
            }
            printf("\n");
            events__mouse = 0;
        }
    }

    display__quit();

    colony__free(c);
    pheromone__free(map);

    return 0;
}

