#include "rgb_color.h"

void rgb_color__set(rgb_color *color, int r, int g, int b){
    color->r = r;
    color->g = g;
    color->b = b;
}

void rgb_color__set_to_white(rgb_color *color){
    rgb_color__set(color, 255,255,255);
}

void rgb_color__set_to_black(rgb_color *color){
    rgb_color__set(color, 0, 0, 0);
}

void rgb_color__multiply_by(rgb_color *dest, rgb_color *color, float value){
    dest->r = color->r * value;
    dest->g = color->g * value;
    dest->b = color->b * value;
}

void rgb_color__inverse(rgb_color *dest, rgb_color *color){
    dest->r = 255 - color->r;
    dest->g = 255 - color->g;
    dest->b = 255 - color->b;
}
