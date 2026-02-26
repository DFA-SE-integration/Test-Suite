#include "aliascheck.h"

int main(int argc, char **argv) {
    int **p, **q;
    int *x, *y;
    int cond = (argc > 1);

    /* Phase 1: effects */
    if (cond) {
        p = &x;
        q = &y;
    } else {
        p = &y;
        q = &x;
    }

    /* ---- merge point ---- */

    /* Phase 2: checks */
    if (cond) {
        MUSTALIAS(p, &x);
        MUSTALIAS(q, &y);
        NOALIAS(p, q);
    } else {
        MUSTALIAS(p, &y);
        MUSTALIAS(q, &x);
        NOALIAS(p, q);
    }

    return 0;
}