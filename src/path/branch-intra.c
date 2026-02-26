#include "aliascheck.h"

int main()
{
    int *p, *q;
    int a, b, c;
    c = 1;
    p = &a;

    if (c) {
        q = &a;
    } else {
        q = &b;
    }

    MUSTALIAS(p, q);
    return 0;
}
