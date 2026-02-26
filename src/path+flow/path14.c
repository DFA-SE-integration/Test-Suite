#include "aliascheck.h"

int main() {
    int **p, *q;
    int *a, *b;
    int c, d, e;

    /* init (defined) */
    c = 1; d = 2; e = 3;
    a = 0;
    b = 0;

    /* flow only: no path-dependent control flow */
    p = &a;
    q = &d;
    *p = q;          /* a = &d */

    b = &e;          /* b = &e */

    /* checks: only MUST/NO */
    MUSTALIAS(a, &d);
    NOALIAS(a, &c);
    NOALIAS(a, &e);

    MUSTALIAS(b, &e);
    NOALIAS(b, &c);
    NOALIAS(b, &d);

    return 0;
}