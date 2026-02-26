#include "aliascheck.h"

int main(int argc, char **argv) {
    (void)argv;

    int **p, **q;
    int **x, **y;

    int *a, *b, *x1;
    int a1, b1;

    int *m, *n;
    int n1;

    /* init */
    n = &n1;
    a = &a1;
    b = &b1;
    x = y = &x1;

    p = q = &a;

    /* effects (flow) */
    if (argc > 1) {
        p = x;
        q = y;
    } else {
        /* keep p=q=&a */
    }

    /* merge */
    int **pm = p;
    int **qm = q;

    /* effects */
    *pm = n;
    m = *qm;

    /* checks (path) */
    if (argc > 1) {
        MUSTALIAS(p, &x1);
        NOALIAS(p, &a);
    } else {
        MUSTALIAS(p, &a);
        NOALIAS(p, &x1);
    }

    /* these are path-independent */
    NOALIAS(m, &a1);
    NOALIAS(m, &x1);
    MUSTALIAS(n, m);

    return 0;
}