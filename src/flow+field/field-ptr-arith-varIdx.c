#include "aliascheck.h"

struct s {
    int *f[2];   /* field + valid pointer arithmetic */
};

int main(void)
{
    int **q, **p;
    int a, b;
    struct s s1;

    s1.f[0] = &a;

    /* flow: overwrite the same field */
    s1.f[1] = &a;   /* temporary value */
    s1.f[1] = &b;   /* strong update kills previous */

    q = &s1.f[0];
    p = q + 1;      /* points to &s1.f[1] */

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(p, &s1.f[1]);
    MUSTALIAS(*p, &b);
    NOALIAS(*p, &a);
    /* END_AUTOGEN_ALIASCHECK */

    return 0;
}