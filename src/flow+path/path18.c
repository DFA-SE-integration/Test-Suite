#include "aliascheck.h"

/* globals */
int x, y, d;
int *q, *f, *e;

__attribute__((noinline))
void foo(int **p) {
    /* effects (flow) */
    f = &x;

    int take = (x != 0);
    if (take) {
        p = &e;      /* redirect store target */
        f = &y;      /* redirect stored value */
    }

    /* merge */
    int **pm = p;
    int *fm  = f;

    /* effect (flow) */
    *pm = fm;

    /* checks (path) */
    if (take) {
        /* q was NOT updated; only e gets &y */
        MUSTALIAS(e, &y);
        MUSTALIAS(q, &d);

        NOALIAS(q, &x);
        NOALIAS(q, &y);
        NOALIAS(e, &x);
    } else {
        /* q gets &x; e stays unchanged (NULL from main) */
        MUSTALIAS(q, &x);

        NOALIAS(q, &y);
        NOALIAS(q, &d);
        NOALIAS(e, &y);
        NOALIAS(e, &x);
    }
}

int main(int argc, char **argv) {
    (void)argv;

    /* init globals so everything is defined */
    y = 0;
    d = 0;
    e = 0;      /* NULL */
    q = &d;     /* make the “q unchanged” path deterministic */

    /* path selector */
    x = (argc > 1) ? 1 : 0;

    foo(&q);
    return 0;
}