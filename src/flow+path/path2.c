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

    p = &a;
    q = &b;

    /* effects (path) */
    int take = (argc > 1);
    if (take) {
        p = &b;
        q = &a;
    }

    /* merge */
    int **pm = p;
    int **qm = q;

    /* effects (flow) */
    *pm = m;
    n = *qm;

    /* checks (path) */
    if (take) {
        MUSTALIAS(p, &b);
        NOALIAS(p, &a);

        MUSTALIAS(n, &a1);
        NOALIAS(n, &b1);
    } else {
        MUSTALIAS(p, &a);
        NOALIAS(p, &b);

        MUSTALIAS(n, &b1);
        NOALIAS(n, &a1);
    }

    /* path-independent */
    NOALIAS(p, q);       /* &a vs &b (as int** locations) */
    NOALIAS(n, &m1);     /* n is either &a1 or &b1 */

    return 0;
}