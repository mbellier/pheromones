#include "display.h"
#include "pheromone.h"
#include "msg.h"
#include "rgb_color.h"
#include "events.h"
#include "ant.h"
#include "map.h"

#include <SDL.h>
#include <stdlib.h>

/* Originlly supposed to be multiple displays,
 * but SDL provides a single windows.
 * It was more relevant to use a global declaration of the display.
 */
struct display {
    int loop;
    point2d screen_size;
    point2d draw_resolution;
    point2d draw_offset;
    point2d draw_zone_position;
    point2d draw_zone_size;
    rgb_color screen_background_color;
    rgb_color draw_zone_background_color;
    SDL_Surface *screen;
    SDL_Surface *drawing_surface;
} disp;


int display__init(int screen_size_x, int screen_size_y) {
    DEBUG_MSG(DISPLAY, "Starting...")

    disp.screen_size.x = screen_size_x;
    disp.screen_size.y = screen_size_y;

    disp.draw_zone_position.x = 0;
    disp.draw_zone_position.y = 0;
    disp.draw_offset.x = 0;
    disp.draw_offset.y = 0;
    disp.draw_zone_size.x = screen_size_x;
    disp.draw_zone_size.y = screen_size_y;

    rgb_color__set(&disp.screen_background_color, DISPLAY__SCREEN_DEFAULT_COLOR);


    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        ERROR_MSG(DISPLAY, "Unable to init SDL : %s", SDL_GetError() )
        return 1;
    }

    //atexit(SDL_Quit); // DO NOT USE

    SDL_EnableKeyRepeat(DISPLAY__KEYBOARD_REPETITION_DELAY, DISPLAY__KEYBOARD_REPETITION_INTERVAL);

    disp.screen = SDL_SetVideoMode(screen_size_x, screen_size_y, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (NULL == disp.screen) {
        ERROR_MSG(DISPLAY,"Unable to set %dx%d video: %s", screen_size_x, screen_size_y, SDL_GetError())
        return 2;
    }

    disp.drawing_surface = NULL;

    disp.loop = 1;

    return 0;
}

int display__quit(void) {

    if (disp.drawing_surface ==NULL) {
        ERROR_MSG(DISPLAY, "attempting to free an unitialized surface (drawing_surface).")
    } else {
        SDL_FreeSurface(disp.drawing_surface);
        disp.drawing_surface=NULL;
    }

    SDL_Quit();

    DEBUG_MSG(DISPLAY, "Terminated.")
    return 0;
}

int display__handle_errors(void) {
    if (NULL == disp.screen) {
        ERROR_MSG(DISPLAY, "screen surface not initialized.")
        return 1;
    }
    if (NULL == disp.drawing_surface) {
        ERROR_MSG(DISPLAY, "drawing surface not initialized.")
        return 2;
    }
    return 0;
}

int display__drawing_surface(int pos_x, int pos_y, rgb_color *color) {
    int err;
    if ((err = display__handle_errors())) return err;

    SDL_Rect pos;
    pos.x = pos_x;
    pos.y = pos_y;
    SDL_FillRect(disp.drawing_surface, 0, SDL_MapRGB(disp.screen->format, color->r, color->g, color->b));
    SDL_BlitSurface(disp.drawing_surface, 0, disp.screen, &pos);
    return 0;
}

void display__draw_boundaries(point2d *dest, int size_x, int size_y) {
    dest->x = MIN((disp.draw_zone_size.x / disp.draw_resolution.x + disp.draw_offset.x + 1) , size_x);
    dest->y = MIN((disp.draw_zone_size.y / disp.draw_resolution.y + disp.draw_offset.y + 1) , size_y);
}

void display__apply_offset(point2d *dest, int i, int j) {
    dest->x = (i - disp.draw_offset.x) * disp.draw_resolution.x + disp.draw_zone_position.x;
    dest->y = (j - disp.draw_offset.y) * disp.draw_resolution.y + disp.draw_zone_position.y;
}

void display__unapply_offset(point2d *dest, int x, int y) {
    dest->x = (x - disp.draw_zone_position.x) / disp.draw_resolution.x + disp.draw_offset.x;
    dest->y = (y - disp.draw_zone_position.y) / disp.draw_resolution.y + disp.draw_offset.y;
}

int display__draw_pheromone(pheromone *p, int is_map) {
    int err;
    if ((err = display__handle_errors())) return err;

    rgb_color color;
    pheromone__get_color(&color, p);
    rgb_color tmp_color;

    point2d boundaries;
    display__draw_boundaries(&boundaries, pheromone__size_x(p), pheromone__size_y(p));

    point2d pos;
    int i,j;
    float value, transparency = pheromone__get_transparency(p);
    for (i = disp.draw_offset.x; i < boundaries.x ; i++) {
        for (j = disp.draw_offset.y; j< boundaries.y; j++) {
            display__apply_offset(&pos, i, j);
            value = pheromone__get(p,i,j);
            if (value >= 0 && value != transparency) {
                if (is_map) {
                    switch ((int) (value)) {
                    case map__food:
                        rgb_color__multiply_by(&tmp_color, &color, 1);
                        break;
                    case map__wall:
                        rgb_color__inverse(&tmp_color, &disp.screen_background_color);
                        break;
                    default:
                        value = transparency;
                    }
                } else {
                    rgb_color__multiply_by(&tmp_color, &color, value);
                }
                if (value != transparency) { // second check in case of map
                    if ((err = display__drawing_surface(pos.x, pos.y, &tmp_color))) {
                        ERROR_MSG(DISPLAY,"pheromone drawing aborted. (error code : %d)", err)
                        return err;
                    }
                }
            }
        }
    }
    return 0;
}

int display__draw_ant(ant *a, rgb_color *color) {
    int err;
    if ((err = display__handle_errors())) return err;

    point2d pos;
    ant__get_pos(&pos, a);
    display__apply_offset(&pos, pos.x, pos.y);

    if ((err = display__drawing_surface(pos.x, pos.y, color))) {
        ERROR_MSG(DISPLAY,"drawing ant (error code : %d)", err)
        return err;
    }

    return 0;
}

int display__begin_drawing(void) {
    if (disp.screen == NULL) {
        ERROR_MSG(DISPLAY, "drawing_surface not initialized. Use display__set_draw_resolution.")
        return 1;
    }
    SDL_FillRect(disp.screen, 0, SDL_MapRGB(disp.screen->format, disp.screen_background_color.r, disp.screen_background_color.g, disp.screen_background_color.b));
    return 0;
}

int display__end_drawing(void) {
    if (disp.screen == NULL) {
        ERROR_MSG(DISPLAY, "drawing_surface not initialized. Use display__set_draw_resolution.")
        return 1;
    }
    SDL_Flip(disp.screen);
    return 0;
}

int display__loop(events__handling_function f) {
    if (display__handle_errors()) {
        ERROR_MSG(DISPLAY, "loop aborted.")
        disp.loop = 0;
    } else {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                DEBUG_MSG(DISPLAY,"Window close button.")
                disp.loop = 0;
                break;
            default:
                disp.loop = f(&event);
            }
        }
    }
    return disp.loop;
}

/* ------ Accessors/Mutators ------*/
int display__set_draw_resolution(int draw_resolution_x, int draw_resolution_y) {
    if (draw_resolution_x < 1 || draw_resolution_y < 1 || draw_resolution_x > disp.screen_size.x || draw_resolution_y > disp.screen_size.y) {
        ERROR_MSG(DISPLAY, "attempted to set an invalid resolution of %d x %d.", draw_resolution_x, draw_resolution_y)
        return 1;
    }
    disp.draw_resolution.x = draw_resolution_x;
    disp.draw_resolution.y = draw_resolution_y;
    if (disp.drawing_surface != NULL) {
        SDL_FreeSurface(disp.drawing_surface);
    }
    disp.drawing_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, draw_resolution_x, draw_resolution_y, 32, 0, 0, 0, 0);
    DEBUG_MSG(DISPLAY,"resolution change to %d x %d.", draw_resolution_x, draw_resolution_y)
    return 0;
}

void display__get_draw_resolution(point2d *dest) {
    *dest = disp.draw_resolution;
}

void display__get_draw_offset(point2d *dest) {
    *dest = disp.draw_offset;
}

void display__set_draw_offset(int offset_x, int offset_y) {
    disp.draw_offset.x = offset_x;
    disp.draw_offset.y = offset_y;
}

void display__get_draw_zone_position(point2d *dest) {
    *dest = disp.draw_zone_position;
}

void display__set_draw_zone_position(int x, int y) {
    disp.draw_zone_position.x = x;
    disp.draw_zone_position.y = y;
}

void display__get_draw_zone_size(point2d *dest) {
    *dest = disp.draw_zone_size;
}

void display__set_draw_zone_size(int x, int y) {
    disp.draw_zone_size.x = x;
    disp.draw_zone_size.y = y;
}

void display__set_screen_background_color_color(int r, int g, int b) {
    disp.screen_background_color.r = r;
    disp.screen_background_color.g = g;
    disp.screen_background_color.b = b;
}

void display__get_screen_background_color_color(rgb_color *dest) {
    *dest = disp.screen_background_color;
}
