#include "aliascheck.h"
int obj, t, s;

void foo(int **p, int **q){
    (void)p;
    *q = &obj;
}

int main(void){
    int **x, **y;
    int *a = 0, *b = 0, *c = 0, *d = 0, *e = 0;

    x = &a; y = &b;
    foo(x, y);
    MUSTALIAS(b, &obj);

    *b = 5;      /* obj = 5 */
    c = &s;

    if (t) {     /* в реальном исполнении t == 0, но пусть будет */
        x = &c; y = &e;
    } else {
        x = &d; y = &d;
    }

    foo(x, y);

    /* Для реального выполнения (t == 0): */
    MUSTALIAS(d, &obj);
    NOALIAS(c, d);

    return 0;
}