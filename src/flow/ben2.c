#include "aliascheck.h"

void foo(int **s, int **c){
    /* FLOW: update pointee */
    *s = *c;
}

int main(void){
    int *a, *b;
    int **c;

    int x = 0, y;

    int *w;
    int **s;
    int u;

    a = &x;
    b = &y;

    c = &a;     /* c -> a -> x */
    s = &w;     /* s -> w */
    w = &u;
    u = 9;

    /* FLOW change: w was &u, becomes &x */
    foo(s, c);

    /* now c points to w (not to a anymore) */
    c = s;

    if (x == 5){
        **c = 10;   /* writes to x (since *c == w == &x) */
    } else {
        *b = 100;   /* writes to y */
    }

    while (u > 10){
        *w = 100;   /* not executed (u=9), avoids infinite loop */
    }

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(a, &x);
    MUSTALIAS(b, &y);

    MUSTALIAS(s, &w);
    MUSTALIAS(c, &w);

    MUSTALIAS(w, &x);     /* key flow fact (due to foo) */
    MUSTALIAS(*s, &x);    /* same as w == &x */

    NOALIAS(a, b);
    NOALIAS(c, &a);       /* since c == &w, not &a */
    /* END_AUTOGEN_ALIASCHECK */

    return 0;
}