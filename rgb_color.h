#ifndef RGB_COLOR_H_
#define RGB_COLOR_H_

typedef struct rgb_color{
    int r;
    int g;
    int b;
} rgb_color;

void rgb_color__set          (rgb_color *color, int r, int g, int b);
void rgb_color__set_to_white (rgb_color *color);
void rgb_color__set_to_black (rgb_color *color);
void rgb_color__multiply_by  (rgb_color *dest, rgb_color *color, float value);
void rgb_color__inverse(rgb_color *dest, rgb_color *color);

#endif
