/*
 * Cycle (with an optional branch that can redirect y1)
 * Author: Sen Ye
 * Date: 11/10/2013
 *
 * Path+flow MUST/NO version:
 *  - y1 is either &y2 or &y2_ depending on path
 *  - indirect writes through *y1 make the final aliasing differ by path
 */
#include "aliascheck.h"

int main(int argc, char **argv) {
    (void)argv;

    int **x1, **y1, **z1;          // int**
    int *x2, *y2, *z2, *y2_;       // int*
    int x3, y3, z3, y3_;           // pointees

    /* init: distinct pointees */
    x2 = &x3;
    y2 = &y3;
    z2 = &z3;
    y2_ = &y3_;                    /* always initialized */

    /* x1,y1,z1 point to pointer variables */
    x1 = &x2;
    y1 = &y2;
    z1 = &z2;

    /* effects (path) */
    int take = (argc > 1);
    if (take) {
        y1 = &y2_;
    }

    /* merge */
    int **y1m = y1;

    /* flow through pointers-to-pointers */
    *x1  = *y1m;   /* x2 = (y2 or y2_) */
    *z1  = *x1;    /* z2 = x2 */
    *y1m = *x1;    /* (y2 or y2_) = x2 */

    /* checks (path) */
    if (take) {
        /* y1 redirected to y2_: x2 == y2_ and y2 stays distinct */
        MUSTALIAS(x2, y2_);
        NOALIAS(x2, y2);
        MUSTALIAS(z2, x2);
    } else {
        /* y1 was &y2: x2 == y2 */
        MUSTALIAS(x2, y2);
        MUSTALIAS(z2, x2);
        NOALIAS(x2, y2_); /* y2_ still points to y3_ */
    }

    /* path-independent sanity */
    NOALIAS(y2, y2_);

    return 0;
}