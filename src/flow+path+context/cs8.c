#include "aliascheck.h"

int obj1, obj2;

void foo(int **p, int **q){
    *p = &obj1;
    *q = &obj2;
}

int main(void){
    int **a, **b;
    int *x = 0, *y = 0, *z = 0;

    /* Path 1: a=&x, b=&y */
    a = &x;
    b = &y;
    foo(a, b);

    MUSTALIAS(x, &obj1);
    MUSTALIAS(y, &obj2);
    NOALIAS(x, &obj2);
    NOALIAS(y, &obj1);
    NOALIAS(x, y);

    /* Path 2: a=&z, b=&z */
    a = &z;
    b = &z;
    foo(a, b);

    /*
     * Внутри foo:
     *   *p = &obj1;   => z = &obj1
     *   *q = &obj2;   => z = &obj2   (перезапись того же z)
     * Итог: z == &obj2
     */
    MUSTALIAS(z, &obj2);
    NOALIAS(z, &obj1);

    return 0;
}