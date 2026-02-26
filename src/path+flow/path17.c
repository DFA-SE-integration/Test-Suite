#include "aliascheck.h"

int main(int argc, char **argv) {
    (void)argv;

    int **p, *q;
    int *a, *b;
    int c, d;

    /* init (defined) */
    a = 0;
    b = 0;
    q = 0;
    c = 1;
    d = 2;

    /* effects (path + flow) */
    if (argc & 1) {
        p = &a;
        if (argc & 2)
            q = &c;
        else
            q = &d;
    } else {
        p = (int**)0;
        if (argc & 2)
            p = &b;
        q = &d;
    }

    /* merge */
    int **pm = p;
    int *qm  = q;

    /* effect */
    if (pm)
        *pm = qm;

    /* checks (path) */
    if (argc & 1) {
        /* p == &a, so a gets q */
        if (argc & 2) {
            MUSTALIAS(a, &c);
            NOALIAS(a, &d);
        } else {
            MUSTALIAS(a, &d);
            NOALIAS(a, &c);
        }
        /* b never written on this path */
        NOALIAS(b, &c);
        NOALIAS(b, &d);
    } else {
        /* p was NULL, unless (argc&2) then p==&b and b gets &d */
        if (argc & 2) {
            MUSTALIAS(b, &d);
            NOALIAS(b, &c);
        } else {
            /* neither a nor b written */
            NOALIAS(a, &c);
            NOALIAS(a, &d);
            NOALIAS(b, &c);
            NOALIAS(b, &d);
        }
        /* a is never written on the (argc&1)==0 paths */
        NOALIAS(a, &c);
        NOALIAS(a, &d);
    }

    return 0;
}