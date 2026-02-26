#include "aliascheck.h"

int *x, y, z;

__attribute__((noinline))
void f(void) {
    /* path+flow with bounded recursion via z as a depth counter */
    if (z > 0) {
        /* effect: consume the single recursion “budget” */
        z--;

        /* context 1: call with x == &y */
        x = &y;
        f();

        /* context 2: call with x == &z */
        x = &z;
        f();

        /* after the second assignment+call, x is deterministically &z */
        MUSTALIAS(x, &z);
        NOALIAS(x, &y);
    }
}

int main(void) {
    /* make everything defined even if someone changes z later */
    x = &y;

    /* enable exactly one path iteration (finite recursion) */
    z = 1;

    f();

    /* final state is deterministic */
    MUSTALIAS(x, &z);
    NOALIAS(x, &y);

    *x = 100;
    return 0;
}