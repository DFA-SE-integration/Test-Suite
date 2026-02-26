#include "aliascheck.h"
#include <stdio.h>

int main(int argc, char **argv) {
    (void)argv;

    int **p, *q;
    int *b, *c;
    int d, e;

    /* init (defined) */
    b = 0;
    c = &e;      /* keep c deterministic */
    p = &c;
    q = 0;

c1:
    /* effects (path) */
    if (argc > 2) {
        p = &b;
        q = &d;
    } else if (argc > 1) {
        q = &e;          /* p stays &c */
    } else {
        printf("dummy branch\n");
        q = &e;          /* still define q */
        /* no back-edge taken, but we keep the label to match original shape */
    }

    /* merge */
    int **pm = p;
    int *qm  = q;

    /* effect (flow) */
    *pm = qm;

    /* checks (path) */
    if (argc > 2) {
        MUSTALIAS(p, &b);
        NOALIAS(p, &c);

        MUSTALIAS(b, &d);
        MUSTALIAS(c, &e);

        NOALIAS(c, &d);
        NOALIAS(b, &e);
    } else {
        MUSTALIAS(p, &c);
        NOALIAS(p, &b);

        MUSTALIAS(c, &e);

        /* b not written on this path (stays NULL) */
        NOALIAS(b, &d);
        NOALIAS(b, &e);

        NOALIAS(c, &d);
    }

    return 0;
}