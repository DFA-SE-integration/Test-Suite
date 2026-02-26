#include "aliascheck.h"

int *p = NULL;
int *q = NULL;
int c;

void foo(void) {
    /* общая функция, ничего не делает */
}

int main(void) {
    int a, b;

    p = &a;
    q = p;
    foo();
    MUSTALIAS(p, q);   // context 1

    p = &c;
    foo();
    NOALIAS(p, q);     // context 2 (flow update killed old alias)

    return 0;
}