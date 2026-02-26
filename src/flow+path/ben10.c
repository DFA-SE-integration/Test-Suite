#include "aliascheck.h"

int main(int argc, char **argv){
    int a, b, *p, c;
    int cond = (argc > 1);

    /* Phase 1: effects */
    if (cond)
        p = &a;
    else
        p = &b;

    /* ---- merge point ---- */

    /* Phase 2: checks (path) */
    if (cond) {
        MUSTALIAS(p, &a);
        NOALIAS(p, &b);
    } else {
        MUSTALIAS(p, &b);
        NOALIAS(p, &a);
    }

    /* Flow kill */
    p = &a;

    /* Flow check */
    MUSTALIAS(p, &a);

    /* остальная логика, если нужна */
    *p = 10;
    c = *p;

    return 0;
}