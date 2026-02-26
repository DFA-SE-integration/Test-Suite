#include "aliascheck.h"

void foo(int **s);
void bar(int **s);

int k;

int main(int argc, char **argv) {
    (void)argv;

    int **p = 0, *q = 0;
    int *b = 0, *c = 0;
    int e;

    /* make path condition defined */
    int take = (argc > 1);

    /* effects (path) */
    if (take) {
        p = &b;
        foo(&q);      /* bar writes q = &k */
    } else {
        p = &c;
        q = &e;       /* q = &e */
    }

    /* merge */
    int **pm = p;
    int *qm  = q;

    /* effect (flow) */
    *pm = qm;

    /* checks (path) */
    if (take) {
        MUSTALIAS(b, &k);
        NOALIAS(b, &e);
        NOALIAS(c, &k);
        NOALIAS(c, &e);
    } else {
        MUSTALIAS(c, &e);
        NOALIAS(c, &k);
        NOALIAS(b, &k);
        NOALIAS(b, &e);
    }

    return 0;
}

void foo(int **x) {
    bar(x);
}

void bar(int **s) {
    *s = &k;
}