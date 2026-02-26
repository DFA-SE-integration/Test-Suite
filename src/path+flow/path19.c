#include "aliascheck.h"

/* globals */
int y, c, d;
int *q, *r, *f, *e;

__attribute__((noinline))
void foo(int **p) {
    /* keep original actual parameter for flow checks */
    int **p_orig = p;

    /* effects (path) */
    if (y) {
        p = &e;     /* redirect the store target */
        f = &y;     /* redirect the value being stored */
    }

    /* merge */
    int **p_m = p;
    int *f_m  = f;

    /* effect (flow) */
    *p_m = f_m;

    /* checks (path) */
    if (y) {
        /* on this path we MUST store &y into e */
        MUSTALIAS(e, &y);

        /* original q/r were not the store target on this path */
        NOALIAS(*p_orig, &y); /* whatever q/r points to, it's not forced to &y here */
    } else {
        /* on this path we store f into the caller-provided *p */
        MUSTALIAS(*p_orig, f_m);

        /* and e stays what it was before (we keep it NULL in main for this call) */
        NOALIAS(e, &y);
    }
}

int main(void) {
    /* init all globals to avoid UB / uninitialized reads */
    y = 0;
    q = 0;
    r = 0;
    e = 0;
    f = 0;

    /* ---- Call 1: y == 0, so store goes through parameter (&q) ---- */
    f = &d;
    foo(&q);

    MUSTALIAS(q, &d);
    NOALIAS(q, &c);
    NOALIAS(q, &y);
    NOALIAS(e, &d);
    NOALIAS(e, &c);

    /* ---- Call 2: y == 1, so store target is redirected to e, value to &y ---- */
    y = 1;
    r = &c;      /* make r deterministic and stable across foo() */
    f = &d;      /* will be overwritten to &y inside foo() on this path */
    foo(&r);

    /* r unchanged (store did not go through &r), e updated */
    MUSTALIAS(r, &c);
    MUSTALIAS(e, &y);

    /* cross checks */
    NOALIAS(r, &d);
    NOALIAS(q, &c);
    NOALIAS(e, &c);
    NOALIAS(e, &d);

    return 0;
}