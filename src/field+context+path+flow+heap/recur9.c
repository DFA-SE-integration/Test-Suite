#include "aliascheck.h"
#include <stdlib.h>

int z;

__attribute__((noinline)) void foo(int **a);

__attribute__((noinline))
void bar(int **q) {
    (void)q;
    /* heap object for next recursion step */
    int **h = (int**)malloc(sizeof(int*));
    foo(h);
    free(h);
}

__attribute__((noinline))
void foo(int **a) {
    if (z > 5) return;
    z++;

    /* strong update: *a now точно указывает на глобальный z */
    *a = &z;

    bar(a);
}

int main() {
    int **a, *b, *c;
    int b1, c1;

    /* 1) через a=&b: b перезаписывается на &z */
    z = 0;
    b = &b1;
    a = &b;
    foo(a);

    MUSTALIAS(b, &z);
    NOALIAS(b, &b1);

    /* 2) сбрасываем z, чтобы foo снова сделал присваивание */
    z = 0;
    c = &c1;
    a = &c;
    foo(a);

    MUSTALIAS(c, &z);
    NOALIAS(c, &c1);

    return 0;
}