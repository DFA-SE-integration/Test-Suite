#include "aliascheck.h"

__attribute__((noinline))
void foo(int **p, int *q) {
    *p = q;
}

int main(int argc, char **argv) {
    (void)argv;

    int **x = 0, *y = 0;
    int *c = 0, *d = 0;
    int e, f;

    /* make condition defined */
    int take = (argc > 1);

    /* effects (path) */
    if (take) {
        x = &c;
        y = &e;
    } else {
        x = &d;
        y = &f;
    }

    /* merge */
    int **xm = x;
    int *ym  = y;

    /* effect (flow) */
    foo(xm, ym);

    /* checks (path) */
    if (take) {
        MUSTALIAS(c, &e);
        NOALIAS(c, &f);
        NOALIAS(d, &e);
        NOALIAS(d, &f);
    } else {
        MUSTALIAS(d, &f);
        NOALIAS(d, &e);
        NOALIAS(c, &f);
        NOALIAS(c, &e);
    }

    /* cross checks */
    NOALIAS(c, &f);
    NOALIAS(d, &e);

    return 0;
}