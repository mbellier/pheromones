#include "ant_ai.h"
#include "ant.h"
#include "pheromone.h"
#include "point2d.h"
#include "maths.h"
#include "map.h"
#include "maths.h"
#include "random.h"


/**
 * Side effects.
 */
void ant_ai__turn_to(ant *a, point2d *to) {
    int dir = ant_ai__dir_to(a, to);
    int angle;
    if (ant_ai__distance_to(a, to) >= 2) {
        if (dir <= 4) angle = 1;
        else angle = -1;
        ant__turn(a, angle);
    } else {
        ant__set_dir(a, dir);
    }
}

/**
 * No side effect.
 */
int ant_ai__dir_to(ant *a, point2d *to) {
    point2d pos;
    ant__get_pos(&pos, a);
    int dir = (point2d__dir_by_pos(&pos, to) - ant__get_dir(a)) % 8;
    if (dir < 0) dir += 8;
    return dir;
}

/**
 * No side effect.
 */
int ant_ai__distance_to(ant *a, point2d *to) {
    point2d pos;
    ant__get_pos(&pos, a);
    return maths__norm(pos.x, pos.y, to->x, to->y);
}


int ant_ai__handle_collision(ant *a, pheromone *map) {
    point2d pos, to, boundaries;
    ant__get_pos(&pos, a);
    pheromone__get_size(&boundaries, map);

    int rotation = (random__int() % 2 == 0) ? 1 : -1;

    int dir = ant__get_dir(a);
    do {
        point2d__pos_by_dir(&to, &pos, &boundaries, ant__get_dir(a), 0);
        if (map__is_wall(map, to.x, to.y)) ant__turn(a, rotation);
        else return 0;


    } while(dir != ant__get_dir(a));

    DEBUG_MSG(ANT_AI, "Ant %d collision", ant__get_tag(a))
    return 1;
}

int ant_ai__collision_move(ant *a, pheromone *map) {
    point2d boundaries;
    pheromone__get_size(&boundaries, map);
    int collision = ant_ai__handle_collision(a, map);
    if (!collision) {
        ant__move(a, &boundaries);
    }
    return collision;
}

int ant_ai__random_move(ant *a, pheromone *map) {
    ant__turn(a, (random__int() % 2 == 0) ? 1 : -1);
    return ant_ai__collision_move(a, map);
}

int ant_ai__follow_nest(ant *a, point2d *nest, pheromone *map, pheromone *nest_map) {
    point2d boundaries, pos, to;
    pheromone__get_size(&boundaries, map);
    ant__get_pos(&pos, a);

    if (point2d__equal(nest, &pos)) return 1;

    int dir, target_dir = -1;

    float current_value;
    float target_value = 0;

    for (dir = 0; dir < 8; dir ++) {
        point2d__pos_by_dir(&to, &pos, &boundaries, dir, 0);
        current_value = pheromone__get(nest_map, to.x, to.y);
        if (current_value >= 0) {
            if (current_value > target_value || (current_value == target_value && random__proba(3))) {
                target_value = current_value;
                target_dir = dir;
            }
        }
    }
    if (target_dir >= 0) {
        ant__set_dir(a, target_dir);
        if (maths__norm(pos.x, pos.y, nest->x, nest->y) > 1 && random__proba(2)) {
            ant__turn(a, random__proba(2) ? -1 : 1);
        }
        ant_ai__collision_move(a, map);
        return 0;
    }

    return 1;
}

int ant_ai__follow_pheromone(ant *a, pheromone *map, pheromone *p) {
    point2d boundaries, pos, to;
    pheromone__get_size(&boundaries, map);
    ant__get_pos(&pos, a);

    int dir, target_dir = -1;

    float value = pheromone__get(p, pos.x, pos.y);
    float current_value;
    float target_value = 0;

    for (dir = 0; dir < 8; dir ++) {
        point2d__pos_by_dir(&to, &pos, &boundaries, dir, 0);
        current_value = pheromone__get(p, to.x, to.y);
        if (current_value > target_value && current_value < value && current_value >= 0) {
            target_value = current_value;
            target_dir = dir;
        }
    }

    if (target_dir >= 0) {
        ant__set_dir(a, target_dir);
        //if (random__int() % 8 == 0) ant__turn(a, 1);
        ant_ai__collision_move(a, map);
        return 1;
    }

    return 0;
}

int ant_ai__load_food(ant *a, pheromone *map) {
    point2d pos;
    ant__get_pos(&pos, a);
    if (map__is_food(map, pos.x, pos.y)) {
        pheromone__set(map, pos.x, pos.y, (float) map__empty);
        ant__set_carrying_food(a, 1);
        return 1;
    }
    return 0;
}

int ant_ai__unload_food(ant *a, point2d *nest) {
    point2d pos;
    ant__get_pos(&pos, a);
    if (point2d__equal(&pos, nest)) {
        ant__set_carrying_food(a, 0);
        return 1;
    }
    return 0;
}

void ant_ai__algorithm(ant *a, point2d *nest, pheromone *map, pheromone *nest_map, pheromone *scent) {
    point2d pos;
    ant__get_pos(&pos, a);
    if (ant__get_carrying_food(a)) {
        if (!ant_ai__unload_food(a, nest)) {
            ant_ai__follow_nest(a, nest, map, nest_map);
            pheromone__set(scent, pos.x, pos.y, 1);
        } else {
            ant__turn_back(a);
            DEBUG_MSG(ANT_AI, "Ant %d brought some food to the nest.", ant__get_tag(a))
        }
    } else {
        if (!ant_ai__load_food(a, map)) {
            if (!ant_ai__follow_pheromone(a, map, scent)) {
                ant_ai__random_move(a, map);
            }
        } else {
            DEBUG_MSG(ANT_AI, "Ant %d picked up some food.", ant__get_tag(a))
        }
    }

}

