#include "aliascheck.h"
int obj, t, s;
int *k = &s;

void foo(int **p, int **q);

int main(void){
    int **x, **y;
    int *a, *b, *c, *d, *e;

    a = &t;
    b = &s;          /* fix UB for first foo */
    x = &a; 
    y = &b; 
    foo(x, y);

    NOALIAS(a, b);        /* a=&s, b=&obj */
    MUSTALIAS(b, &obj);

    *b = 5;               /* obj = 5 */

    c = &t;
    c = &s;
    a = c;
    MUSTALIAS(a, c);

    d = &t;               /* fix UB for second foo */

    if (t) {              /* t is still 0 here, so else in real execution */
        c = &obj; 
        x = &c; 
        y = &e;
    } else { 
        x = &d; 
        y = &d;
    }

    e = &t;
    foo(x, y);

    NOALIAS(c, d);
    MUSTALIAS(d, &obj);
    NOALIAS(a, d);

    *e = 10;

    return 0;
}

void foo(int **p, int **q){
    *p = *q;
    *q = &obj;
}