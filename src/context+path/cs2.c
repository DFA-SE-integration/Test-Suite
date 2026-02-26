#include "aliascheck.h"

int obj;

__attribute__((noinline))
void foo(int **p, int **q, int **r) {
    /* flow through two indirections */
    *r = *p;
    *q = &obj;
}

int main(int argc, char **argv) {
    (void)argv;

    int **a, **b, **c, **d, **e, **f;
    int *x, *y, *z, *w, *k;
    int x1, y1, z1, w1, k1;

    /* init */
    x = &x1;
    y = &y1;
    z = &z1;
    w = &w1;
    k = &k1;

    a = &x;
    b = &y;
    c = &z;

    /* 1) first context of foo: (a,b,c) */
    foo(a, b, c);

    /* after foo(a,b,c): z = x, y = &obj */
    MUSTALIAS(x, z);
    MUSTALIAS(y, &obj);

    /* 2) second context of foo depends on path (context + path) */
    int take = (argc > 1);

    /* effects (path) */
    if (take) {
        d = &w;
        e = &w;
        f = &k;
    } else {
        d = &k;
        e = &k;
        f = &w;
    }

    /* merge */
    int **dm = d, **em = e, **fm = f;

    /* second call, different calling context */
    foo(dm, em, fm);

    /* properties that hold on all paths */
    NOALIAS(w, k);
    NOALIAS(x, w);
    NOALIAS(z, k);

    /* path-specific postconditions */
    if (take) {
        /* d=&w,e=&w,f=&k  =>  k=w(old)=&w1 ; w=&obj */
        MUSTALIAS(w, &obj);
        MUSTALIAS(y, w);
    } else {
        /* d=&k,e=&k,f=&w  =>  w=k(old)=&k1 ; k=&obj */
        MUSTALIAS(k, &obj);
        MUSTALIAS(y, k);
        NOALIAS(y, w);
    }

    return 0;
}