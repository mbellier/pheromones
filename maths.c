#include "maths.h"
#include "random.h"

int maths__power(int x, int p){
    if (--p<=0) return x;
    while(p--) {x *= x;}
    return x;
}

int maths__sqrt(int x){
    int r = 1;
    for (r = 1; r*r < x; r++);
    return r;
}

int maths__norm(int x1, int y1, int x2, int y2){
    return maths__sqrt(maths__power(x2 - x1, 2) + maths__power(y2 - y1, 2));
}

void maths__permutation(int *tab, int len){
    int i, j, tmp, n = 4*len;//random__range(2*len, 4*len);
    while (n--){
        i = random__range(0, len - 1);
        j = random__range(0, len - 1);//(i + random__range(1, len - 1)) % (len);
        tmp = tab[j];
        tab[j] = tab[i];
        tab[i] = tmp;
    }
}
