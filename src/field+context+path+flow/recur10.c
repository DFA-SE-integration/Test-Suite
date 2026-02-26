#include "aliascheck.h"

int **p, *x, y, z;

/*
 * f(depth, mode)
 * mode задаёт путь (path),
 * depth делает рекурсию конечной,
 * x переписывается до и после рекурсивного вызова (flow).
 */
void f(int depth, int mode) {
    p = &x;

    if (depth == 0) {
        if (mode) {
            *p = &y;
            MUSTALIAS(x, &y);
            NOALIAS(x, &z);
        } else {
            *p = &z;
            MUSTALIAS(x, &z);
            NOALIAS(x, &y);
        }
        return;
    }

    if (mode) {
        *p = &y;                  /* временное значение */
        MUSTALIAS(x, &y);

        f(depth - 1, 0);          /* другой контекст/путь */

        /* после возврата x перезаписан в рекурсивном вызове */
        MUSTALIAS(x, &z);
        NOALIAS(x, &y);
    } else {
        *p = &z;                  /* временное значение */
        MUSTALIAS(x, &z);

        f(depth - 1, 1);          /* другой контекст/путь */

        MUSTALIAS(x, &y);
        NOALIAS(x, &z);
    }
}

int main(void) {
    f(1, 1);
    MUSTALIAS(x, &z);
    NOALIAS(x, &y);

    f(1, 0);
    MUSTALIAS(x, &y);
    NOALIAS(x, &z);

    return 0;
}