/*
 * Pure PATH-SENSITIVE test (derived from the original).
 * path-sensitive -> PASS (NOALIAS holds on each path)
 * path-insensitive -> FAIL (merges paths, may-alias)
 */
#include "aliascheck.h"

int main()
{
    int *p, *q;
    int a, b;

    if (a) {
        p = &a;
        q = &b;
    } else {
        p = &b;
        q = &a;
    }

    NOALIAS(p, q);
    return 0;
}
