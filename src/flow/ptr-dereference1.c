#include "aliascheck.h"

int main(void)
{
    int a, b, *c, *d;
    c = &a;
    d = &a;
    MUSTALIAS(c, d);

    c = &b;

    MUSTALIAS(c, &b);
    MUSTALIAS(d, &a);
    NOALIAS(c, d);
    NOALIAS(&b, d);

    return 0;
}