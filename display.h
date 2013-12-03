#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "pheromone.h"
#include "events.h"
#include "point2d.h"
#include "ant.h"

/* Display messages (booleans) : */
#define DISPLAY__DEBUG_MSG 1
#define DISPLAY__ERROR_MSG 1

/* Macros */
#define DISPLAY__MAP_MODE       1
#define DISPLAY__PHEROMONE_MODE 0
#define DISPLAY__KEYBOARD_REPETITION_DELAY    100
#define DISPLAY__KEYBOARD_REPETITION_INTERVAL 100
#define DISPLAY__SCREEN_DEFAULT_COLOR         200, 200, 200

/* Prototypes */
int  display__init(int screen_size_x, int screen_size_y);
int  display__quit(void);
int  display__begin_drawing(void);
int  display__draw_pheromone(pheromone *p, int is_map);
int  display__draw_ant(ant *a, rgb_color *color);
int  display__end_drawing(void);
int  display__drawing_surface(int pos_x, int pos_y, rgb_color *color);
void display__apply_offset(point2d *dest, int i, int j);
void display__unapply_offset(point2d *dest, int x, int y);
int  display__loop(events__handling_function f);
int  display__handle_errors(void);
void display__get_draw_resolution(point2d *dest);
int  display__set_draw_resolution(int draw_resolution_x, int draw_resolution_y);
void display__get_draw_offset(point2d *dest);
void display__set_draw_offset(int offset_x, int offset_y);
void display__get_draw_zone_position(point2d *dest);
void display__set_draw_zone_position(int offset_x, int offset_y);
void display__get_draw_zone_size(point2d *dest);
void display__set_draw_zone_size(int offset_x, int offset_y);
void display__get_screen_background_color_color(rgb_color *dest);
void display__set_screen_background_color_color(int r, int g, int b);

#endif
