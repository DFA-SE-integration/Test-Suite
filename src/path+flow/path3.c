#include "aliascheck.h"

int main(int argc, char **argv) {
    (void)argv;

    int **p, **q;
    int *a, *b;
    int *m, *n;
    int a1, b1, m1;

    /* init */
    a = &a1;
    b = &b1;
    m = &m1;

    p = q = &a;

    /* effects (path + flow) */
    int take = (argc > 1);
    if (take) {
        p = q = &b;
    } else {
        /* keep p=q=&a */
    }

    /* merge */
    int **pm = p;
    int **qm = q;

    /* effects */
    *pm = m;
    n = *qm;

    /* checks (path) */
    if (take) {
        MUSTALIAS(p, &b);
        NOALIAS(p, &a);
    } else {
        MUSTALIAS(p, &a);
        NOALIAS(p, &b);
    }

    /* path-independent */
    NOALIAS(n, &a1);
    NOALIAS(n, &b1);
    MUSTALIAS(n, m);

    return 0;
}