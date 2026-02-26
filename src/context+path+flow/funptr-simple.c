#include "aliascheck.h"

/*
 * Function pointer (context + path + flow), MUST/NO version
 * Author: Sen Ye
 * Date: 10/10/2013
 */

__attribute__((noinline))
void f(int *p, int *q) {
    /* nothing else needed: alias relation comes from caller context */
    MUSTALIAS(p, q);
}

void (*fptr)(int*, int*);

int main(int argc, char **argv) {
    (void)argv;

    int x, y;
    int *m = 0, *n = 0;

    int take = (argc > 1);   /* defined path condition */

    /* effects (path + flow) */
    if (take) {
        m = &x;
        n = &x;
        fptr = f;
        fptr(m, n);          /* CS1: via function pointer */
    } else {
        m = &x;
        n = &y;
        /* direct call, but keep MUST/NO by checking in caller */
        /* (we do not call f here because f asserts MUSTALIAS) */
    }

    /* merge */
    int *mm = m;
    int *nm = n;

    /* checks (path) */
    if (take) {
        MUSTALIAS(mm, nm);   /* &x vs &x */
    } else {
        NOALIAS(mm, nm);     /* &x vs &y */
    }

    return 0;
}