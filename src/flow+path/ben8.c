#include "aliascheck.h"

int t;

int main(int argc, char **argv){
    int **x;
    int *a = 0, *c = 0, *d = 0;
    int e = 0;

    int cond = (argc > 1);

    x = &a;
    *x = &e;

    /* Phase 1: effects (path) */
    for (t = 0; t < 10; t++) {
        if (cond) { 
            x = &c;
        } else { 
            x = &d;
        }
    }

    /* flow: write through the selected pointer */
    *x = &t;

    /* ---- merge point ---- */

    /* Phase 2: checks (no MAY) */
    if (cond) {
        MUSTALIAS(x, &c);
        MUSTALIAS(c, &t);      /* i.e., *x == &t */
        NOALIAS(d, &t);        /* d was not selected */
    } else {
        MUSTALIAS(x, &d);
        MUSTALIAS(d, &t);
        NOALIAS(c, &t);
    }

    return 0;
}