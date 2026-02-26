#include "aliascheck.h"

typedef struct subagg1{
    int *d;
} subagg1;

typedef struct agg1{
    int *c;
    subagg1 sub;
} agg1;

int main(int argc, char **argv){
    int *a = 0, *b = 0, k1 = 0, k2 = 0;
    agg1 g1, g2;
    agg1 *g = &g1;

    /* init to avoid UB when reading in checks */
    g1.sub.d = 0;
    g2.sub.d = 0;

    if (argc) {
        g = &g2;
        a = &k1;
    } else {
        a = &k2;
    }

    g->sub.d = a;

    /* AUTOGEN_ALIASCHECK */
    if (argc) {
        MUSTALIAS(g1.sub.d, &k2);
    } else {
        MUSTALIAS(g2.sub.d, &k1);
    }
    NOALIAS(g1.sub.d, &k1);
    NOALIAS(g2.sub.d, &k2);
    /* END_AUTOGEN_ALIASCHECK */

    return 0;
}