#include "aliascheck.h"

int *p;

void foo(int *p){
    *p = 100;
}

int main(int argc, char **argv){
    int *q, s, c, d;
    int cond = (argc > 1);   /* стабильное условие без UB */

    q = &s;
    p = q;

    /* Phase 1: effects */
    if (cond) {
        p = &c;
    } else {
        p = &d;
    }

    *p = 100;
    foo(p);

    /* ---- merge point ---- */

    /* Phase 2: path-specific checks using the same predicate */
    if (cond) {
        MUSTALIAS(p, &c);
        NOALIAS(p, &d);

        MUSTALIAS(q, &s);
        NOALIAS(q, p);
    } else {
        MUSTALIAS(p, &d);
        NOALIAS(p, &c);

        MUSTALIAS(q, &s);
        NOALIAS(q, p);
    }

    return 0;
}