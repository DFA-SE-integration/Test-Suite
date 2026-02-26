#include "aliascheck.h"

int a, b, c;

/*
 * foo(x, n):
 * - при n > 0 рекурсивно спускается (конечная рекурсия)
 * - при n == 0 выбирает результат по пути (x / !x)
 * - переменная y сначала получает &c, затем перезаписывается (нужна flow-sensitivity)
 */
int *foo(int *x, int n) {
    int *y = &c;   // временное значение (должно "убиваться" позже)

    if (n > 0) {
        y = foo(x, n - 1);   // перезапись y -> проверка flow-sensitivity
    } else {
        if (x) {
            y = x;           // path 1
            MUSTALIAS(y, x);
            NOALIAS(y, &b);
        } else {
            y = &b;          // path 2
            MUSTALIAS(y, &b);
        }
    }

    return y;
}

int main() {
    int *p = &a;
    int *r1, *r2;

    r1 = foo(p, 1);   // контекст 1: x = &a
    r2 = foo(0, 1);   // контекст 2: x = NULL

    MUSTALIAS(r1, &a);
    MUSTALIAS(r2, &b);

    NOALIAS(r1, r2);
    NOALIAS(r1, &c);
    NOALIAS(r2, &c);

    return 0;
}