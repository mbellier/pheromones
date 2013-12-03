#ifndef MATRIX_H_
#define MATRIX_H_

struct float_matrix;
typedef struct float_matrix float_matrix;

float_matrix *float_matrix__new         (int x, int y);
void          float_matrix__free        (float_matrix *m);
void          float_matrix__init        (float_matrix *m, float value);
float         float_matrix__get         (float_matrix *m, int x, int y);
int           float_matrix__set         (float_matrix *m, int x, int y, float value);
int           float_matrix__size_x      (float_matrix *m);
int           float_matrix__size_y      (float_matrix *m);
int           float_matrix__copy        (float_matrix *d, float_matrix *s);
void          float_matrix__debug_print (float_matrix *m);
int           float_matrix__save        (float_matrix *m, char *file_path);
float_matrix *float_matrix__open        (char *file_path);
#endif
