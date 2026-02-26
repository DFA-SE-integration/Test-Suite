#include "aliascheck.h"
#include <stdlib.h>
#include <stdio.h>

void foo(int*, int*);

typedef struct gg{
    int f3;
} gg;

typedef struct ff{
    int *f1;
    gg f2;
} ff;

void ss(){}

int main(void){
    int *a, *b, obj, t, k;
    ff f, g;
    gg g1, g2;

    (void)g;
    (void)g2;
    (void)k;

    g1.f3 = 0;        /* fix UB */
    f.f2 = g1;
    f.f1 = &t;

    a = &obj;
    b = &t;
    foo(a, b);

    a = malloc(1);
    a = malloc(2);

    printf("%d", f.f2.f3);

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(f.f1, &t);
    NOALIAS(a, &obj);
    MUSTALIAS(b, &t);
    NOALIAS(f.f1, a);
    /* END_AUTOGEN_ALIASCHECK */

    free(a);
    return 0;
}

void foo(int* x, int *y){
    *x = *y;
}