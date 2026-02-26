#include "aliascheck.h"

int obj, t, s;

__attribute__((noinline))
void foo(int **p, int **q) {
    (void)p;
    *q = &obj;
}

int main(int argc, char **argv) {
    (void)argv;

    int **x, **y;
    int *a, *b, *c, *d, *e;

    int A, B, D, E;

    /* defined init */
    a = &A;
    b = &B;
    d = &D;
    e = &E;
    c = &s;

    /* -------- Context 1: foo(x=&a, y=&b) writes into b -------- */
    x = &a;
    y = &b;

    foo(x, y);

    MUSTALIAS(b, &obj);
    NOALIAS(a, &obj);

    /* flow side-effect through b (actually writes obj) */
    *b = 5;

    /* -------- Context 2: same callee, but different caller/path decides target -------- */
    t = (argc > 1); /* path selector (defined) */

    /* effects (path) */
    if (t) {
        x = &c;     /* irrelevant for foo, keeps "context" different */
        y = &e;     /* foo will write into e */
    } else {
        x = &d;
        y = &d;     /* foo will write into d */
    }

    /* merge */
    int **xm = x;
    int **ym = y;

    foo(xm, ym);

    /* checks (path) */
    if (t) {
        MUSTALIAS(e, &obj);
        NOALIAS(d, &obj);
        NOALIAS(e, d);
    } else {
        MUSTALIAS(d, &obj);
        NOALIAS(e, &obj);
        NOALIAS(e, d);
    }

    /* -------- flow after join: write through whichever one got &obj -------- */
    int *target;
    if (t)
        target = e;
    else
        target = d;

    /* merge */
    int *targetm = target;

    MUSTALIAS(targetm, &obj);
    *targetm = 10;

    return 0;
}