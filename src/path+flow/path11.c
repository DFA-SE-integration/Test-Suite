#include "aliascheck.h"

__attribute__((noinline))
void foo(int **x, int *y) {
    *x = y;
}

int main(int argc, char **argv) {
    (void)argv;

    int **p = 0, *q = 0;
    int *a = 0, *b = 0;
    int c, d;

    /* make the path condition defined */
    int take = (argc > 1);

    /* effects (path) */
    if (take) {
        p = &a;
        q = &c;
        foo(p, q);   /* a = &c */
    } else {
        p = &b;
        q = &d;
        foo(p, q);   /* b = &d */
    }

    /* merge */
    int **pm = p;
    int *qm  = q;

    /* effect (flow) */
    *pm = qm;

    /* checks (path) */
    if (take) {
        MUSTALIAS(a, &c);
        NOALIAS(a, &d);

        NOALIAS(b, &c);
        NOALIAS(b, &d);
    } else {
        MUSTALIAS(b, &d);
        NOALIAS(b, &c);

        NOALIAS(a, &c);
        NOALIAS(a, &d);
    }

    /* cross checks (as in original intent) */
    NOALIAS(a, &d);
    NOALIAS(b, &c);

    return 0;
}