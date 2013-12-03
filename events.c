#include "events.h"
#include "msg.h"
#include "point2d.h"
#include "display.h"
#include <stdio.h>

#define EVENTS__STOP_LOOP     0
#define EVENTS__CONTINUE_LOOP 1



int events__handle(SDL_Event *event) {
    events__mouse = 0;
    int mx, my;
    point2d pos;
    switch (event->type) {
    case SDL_KEYDOWN:
        DEBUG_MSG(EVENTS,"Key pressed : %d", event->key.keysym.sym)
        point2d offset;
        display__get_draw_offset(&offset);
        point2d resolution;
        display__get_draw_resolution(&resolution);

        switch (event->key.keysym.sym) {
        case (SDLK_ESCAPE):
            return EVENTS__STOP_LOOP;
            break;
        case (SDLK_UP) :
            display__set_draw_offset(offset.x, offset.y - 5);
            break;
        case (SDLK_DOWN) :
            display__set_draw_offset(offset.x, offset.y + 5);
            break;
        case (SDLK_LEFT) :
            display__set_draw_offset(offset.x - 5, offset.y);
            break;
        case (SDLK_RIGHT) :
            display__set_draw_offset( offset.x + 5, offset.y);
            break;
        case (SDLK_HOME) :
            display__set_draw_offset( 0, 0);
            break;
        case (SDLK_KP_PLUS ) :
            display__set_draw_resolution( resolution.x + 1, resolution.y + 1);
            break;
        case (SDLK_KP_MINUS) :
            display__set_draw_resolution( resolution.x - 1, resolution.y - 1);
            break;
        case (SDLK_SPACE):
            events__pause = !events__pause;
            break;
        case (SDLK_p):
            events__draw_nest = !events__draw_nest;
            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        mx = event->button.x;
        my = event->button.y;
        display__unapply_offset(&pos, mx, my);

        switch (event->button.button) {
        case 1: // left button
            DEBUG_MSG(EVENTS, "Mouse at (%d; %d)", mx, my)
            events__mouse_pos.x = pos.x;
            events__mouse_pos.y = pos.y;
            events__mouse = 1;
            break;
        }
        break;
    }
    return EVENTS__CONTINUE_LOOP;
}

