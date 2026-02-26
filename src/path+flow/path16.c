#include "aliascheck.h"

int main(int argc, char **argv) {
    (void)argv;

    int **p = 0, *q = 0;
    int *a = 0, *b = 0;
    int c, d;

    /* make conditions defined */
    int take1 = (argc > 1); /* for (p) branch in original */
    int take2 = (argc > 2); /* for (c) branch in original */
    int take3 = (argc > 3); /* for (d) guard before store */

    /* effects (path) */
    p = &a;
    if (take1) {
        if (take2) {
            q = &c;
        } else {
            q = &d;      /* define q on this path */
        }
    } else {
        p = &b;
        q = &d;
    }

    /* merge */
    int **pm = p;
    int *qm  = q;

    /* effect (flow) */
    if (take3) {
        *pm = qm;
    }

    /* checks (path) */
    if (take3) {
        if (take1 && take2) {
            MUSTALIAS(a, &c);
            NOALIAS(a, &d);
            NOALIAS(b, &c);
            NOALIAS(b, &d);
        } else if (!take1) {
            MUSTALIAS(b, &d);
            NOALIAS(b, &c);
            NOALIAS(a, &c);
            NOALIAS(a, &d);
        } else {
            /* take1==1 && take2==0 -> store a = &d */
            MUSTALIAS(a, &d);
            NOALIAS(a, &c);
            NOALIAS(b, &c);
            NOALIAS(b, &d);
        }
    } else {
        /* no store happened: a and b unchanged (NULL) */
        NOALIAS(a, &c);
        NOALIAS(a, &d);
        NOALIAS(b, &c);
        NOALIAS(b, &d);
    }

    /* keep the two original “cross” expectations in MUST/NO form per path */
    if (take3 && take1 && take2) {
        /* a==&c implies NOALIAS(a,&d) */
        NOALIAS(a, &d);
    }
    if (take3 && !take1) {
        /* b==&d implies NOALIAS(b,&c) */
        NOALIAS(b, &c);
    }

    return 0;
}