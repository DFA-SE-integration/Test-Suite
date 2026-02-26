/*
 * Pure context-SENSITIVE test:
 * context-sensitive -> PASS (NOALIAS holds per call context)
 * context-insensitive -> FAIL (merges contexts, may-alias via &b)
 */
#include "aliascheck.h"

void foo(int *m, int *n)
{
    NOALIAS(m, n);
    // Можно оставить “полезную” нагрузку, но она не обязательна:
    int x = *n;
    int y = *m;
    *m = x;
    *n = y;
}

int main()
{
    int a, b, c;
    int *p, *q;

    p = &a; q = &b;
    foo(p, q);   // context 1: (m=&a, n=&b)

    p = &b; q = &c;
    foo(p, q);   // context 2: (m=&b, n=&c)

    return 0;
}
