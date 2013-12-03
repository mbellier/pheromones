#ifndef EVENTS_H_
#define EVENTS_H_

#include "SDL_events.h"
#include "point2d.h"

#define EVENTS__DEBUG_MSG 1
#define EVENTS__ERROR_MSG 1

/*------ global variables ------*/
int events__pause;
int events__draw_nest;
int events__mouse;
point2d events__mouse_pos;

typedef int(*events__handling_function)(SDL_Event *);

int events__handle(SDL_Event *event);

#endif
