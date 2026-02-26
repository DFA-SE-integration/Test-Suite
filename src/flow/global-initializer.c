#include "aliascheck.h"

int x;
int *p, *q;
int **pp = &p;
int **qq = &q;

void foo(void) { p = &x; }
void bar(void) { q = &x; }

int main(void) {
    foo();
    bar();
    MUSTALIAS(*pp, *qq);   // теперь точно оба == &x
    return 0;
}