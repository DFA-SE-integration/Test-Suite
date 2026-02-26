#include "aliascheck.h"

int main(int argc, char **argv) {
    int *p, *q;
    int x, y;
    int cond = (argc > 1);   /* инициализированный предикат, оба пути возможны */

    /* Phase 1: effects */
    if (cond)
        p = &x;
    else
        p = &y;

    q = &y;

    /* ---- merge point ---- */

    /* Phase 2: checks */
    if (cond) {
        NOALIAS(p, q);       /* p=&x, q=&y */
        MUSTALIAS(p, &x);
    } else {
        MUSTALIAS(p, q);     /* p=&y, q=&y */
        MUSTALIAS(p, &y);
    }

    return 0;
}