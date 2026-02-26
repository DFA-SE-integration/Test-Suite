#include "aliascheck.h"

/* observable side-effect of callee, to make context matter */
int *LASTP, *LASTQ;

__attribute__((noinline))
void foo(int *p, int *q){
    /* expose actual arguments */
    LASTP = p;
    LASTQ = q;

    /* value-flow (не влияет на alias, но даёт path внутри callee) */
    *p = 100;
    if (*q)
        *p = 200;
}

/* Caller #1: path updates x (flow), then calls foo(x,y) */
__attribute__((noinline))
void callsiteA(int *x, int *y, int flag){
    /* effects (path) */
    if (flag) {
        x = y;              /* flow update */
    } else {
        /* x stays */
    }

    /* merge */
    int *xm = x;

    /* checks (path) */
    if (flag) {
        MUSTALIAS(xm, y);
    } else {
        NOALIAS(xm, y);
    }

    /* context: call #1 */
    foo(xm, y);

    /* merge */
    int *lp = LASTP;
    int *lq = LASTQ;

    /* checks: should hold precisely under context+path+flow */
    MUSTALIAS(lq, y);
    MUSTALIAS(lp, xm);

    if (flag) {
        MUSTALIAS(lp, y);
    } else {
        NOALIAS(lp, y);
    }
}

/* Caller #2: different calling context for foo (arguments swapped) */
__attribute__((noinline))
void callsiteB(int *x, int *y, int flag){
    /* effects (path): update y this time */
    if (flag) {
        y = x;              /* flow update */
    } else {
        /* y stays */
    }

    /* merge */
    int *ym = y;

    /* checks (path) */
    if (flag) {
        MUSTALIAS(ym, x);
    } else {
        NOALIAS(ym, x);
    }

    /* context: call #2 (different callsite + swapped args) */
    foo(ym, x);

    /* merge */
    int *lp = LASTP;
    int *lq = LASTQ;

    /* checks */
    MUSTALIAS(lq, x);
    MUSTALIAS(lp, ym);

    if (flag) {
        MUSTALIAS(lp, x);
    } else {
        NOALIAS(lp, x);
    }
}

int main(int argc, char **argv){
    (void)argv;

    int a = 0, b = 1, c = 0;
    int *x, *y;

    x = &a;
    y = &b;

    /* Context 1: caller A, flag=1 => x aliases y */
    callsiteA(x, y, 1);

    /* Context 2: caller A, flag=0 => x and y stay distinct */
    callsiteA(&a, &b, 0);

    /* Another context for the same callee (different caller + swapped args) */
    callsiteB(&a, &c, 1);
    callsiteB(&a, &c, 0);

    /* Pure path+flow check (no MAY) */
    x = &a;
    y = &b;

    int sel = (argc > 1);          /* defined path condition */
    if (sel)
        x = &a;
    else
        x = &b;

    /* merge */
    int *xm = x;

    /* checks (path) */
    if (sel) {
        MUSTALIAS(xm, &a);
        NOALIAS(xm, &b);
    } else {
        MUSTALIAS(xm, &b);
        NOALIAS(xm, &a);
    }

    x = &c;
    MUSTALIAS(x, &c);
    NOALIAS(x, &a);
    NOALIAS(x, &b);

    return 0;
}