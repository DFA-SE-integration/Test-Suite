#include "aliascheck.h"
#include <stdio.h>

int main(int argc, char **argv) {
    (void)argv;

    int **p = 0, *q = 0;
    int *a = 0, *b = 0;
    int c, d;

    /* effects (path) */
    if (argc > 2) {
        p = &a;
        q = &c;
    } else if (argc > 1) {
        p = &b;
        q = &d;
    } else {
        printf("dummy branch\n");
        /* безопасная ветка: не пишем через p */
        p = 0;
        q = 0;
    }

    /* merge */
    int **pm = p;
    int *qm  = q;

    /* effect (flow) */
    if (pm)
        *pm = qm;

    /* checks (path) */
    if (argc > 2) {
        MUSTALIAS(a, &c);
        NOALIAS(a, &d);
        NOALIAS(b, &c);
        NOALIAS(b, &d);
    } else if (argc > 1) {
        MUSTALIAS(b, &d);
        NOALIAS(b, &c);
        NOALIAS(a, &c);
        NOALIAS(a, &d);
    } else {
        /* ничего не записали */
        NOALIAS(a, &c);
        NOALIAS(a, &d);
        NOALIAS(b, &c);
        NOALIAS(b, &d);
    }

    return 0;
}