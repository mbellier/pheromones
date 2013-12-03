#ifndef RANDOM_H_
#define RANDOM_H_
#include <time.h>
#include <stdlib.h>
void random__init  (void);
int  random__int   (void);
int  random__range (int a, int b);
int  random__proba (int one_out_of);

#endif
