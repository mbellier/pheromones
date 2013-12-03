#include "random.h"
#include <time.h>
#include <stdlib.h>

void random__init  (void){
    srand(time(NULL));
}

int  random__int   (void){
    return rand();
}

int  random__range (int a, int b){
    return rand() % (b - a) + a;
}

int  random__proba (int one_out_of){
    return rand() % one_out_of == 0;
}
