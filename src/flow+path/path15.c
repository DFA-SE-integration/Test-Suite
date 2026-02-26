#include "aliascheck.h"

int main(int argc, char **argv) {
    (void)argv;

    int **p, *q, **w, *v;
    int *a;
    int a1, q1;

    /* init */
    a = &a1;
    p = &a;
    q = &q1;

    /* effects (path + flow) */
    int take = (argc > 1);
    if (take) {
        *p = q;          /* a = &q1 (through p) */
    } else {
        /* keep a = &a1 */
    }

    /* merge */
    int *a_mid = a;

    /* checks (path) */
    if (take) {
        MUSTALIAS(a_mid, &q1);
        NOALIAS(a_mid, &a1);
    } else {
        MUSTALIAS(a_mid, &a1);
        NOALIAS(a_mid, &q1);
    }

    /* flow: overwrite back to &a1 so final v is deterministic */
    a = &a1;

    /* make deref defined on all paths */
    w = &a;
    v = *w;

    /* final checks (no MAY) */
    MUSTALIAS(v, &a1);
    NOALIAS(v, &q1);

    return 0;
}