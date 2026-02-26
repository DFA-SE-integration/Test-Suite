#include "aliascheck.h"
#include <stdlib.h>

int z;

/*
 * foo updates the pointee of a, but recursion is guarded by z.
 * bar allocates a fresh heap cell and passes it to foo, so we exercise heap handling.
 */

void foo(int **a);
void bar(int **q) {
    int **h = (int **)malloc(sizeof(*h));  /* heap cell storing an int* */
    *h = *q;                               /* copy incoming value into heap cell */
    foo(h);                                /* recurse on heap-backed indirection */
    /* q is intentionally not updated from h: caller's pointee should stay unchanged here */
}

void foo(int **a) {
    if (z > 5) return;   /* path-sensitive guard */
    z++;                 /* flow-sensitive state update */
    *a = &z;             /* strong update through a */
    bar(a);
}

int main(void) {
    int **a, *b, *c, b1, c1;

    /* First context: starts with b -> &b1, then foo must overwrite b to &z */
    b = &b1;
    a = &b;
    foo(a);
    MUSTALIAS(b, &z);
    NOALIAS(b, &b1);

    /* Second context: z is already > 5, so foo returns immediately (different path) */
    c = &c1;
    a = &c;
    foo(a);
    NOALIAS(c, &z);
    MUSTALIAS(c, &c1);

    return 0;
}