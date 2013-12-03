#ifndef ANT_H_
#define ANT_H_

#include "point2d.h"

struct ant;
typedef struct ant ant;

ant  *ant__new               (point2d *pos, int sigth);
void  ant__free              (ant *a);
void  ant__set_tag           (ant *a, int tag);
int   ant__get_tag           (ant *a);
void  ant__get_pos           (point2d *dest, ant *a);
void  ant__set_pos           (ant *a, point2d *pos);
int   ant__get_dir           (ant *a);
void  ant__set_dir           (ant *a, int dir);
int   ant__get_carrying_food (ant *a);
void  ant__set_carrying_food (ant *a, int carrying_food);
void  ant__turn              (ant *a, int angle);
void  ant__move              (ant *a, point2d *boundaries);
void  ant__turn_back         (ant *a);

#endif
