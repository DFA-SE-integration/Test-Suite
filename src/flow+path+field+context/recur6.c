#include "aliascheck.h"

int *x;
int y, z;

void f(int depth, int flag) {
    /* termination (path 1) */
    if (depth == 0) {
        if (flag) {
            x = &y;
        } else {
            x = &z;
        }
        return;
    }

    /* path split + flow updates before recursive call */
    if (flag) {
        x = &y;              /* temporary value */
        f(depth - 1, 0);     /* different context/path */
        MUSTALIAS(x, &z);    /* recursive call overwrote x */
        NOALIAS(x, &y);
    } else {
        x = &z;              /* temporary value */
        f(depth - 1, 1);     /* different context/path */
        MUSTALIAS(x, &y);    /* recursive call overwrote x */
        NOALIAS(x, &z);
    }
}

int main(void) {
    f(1, 1);
    MUSTALIAS(x, &z);
    NOALIAS(x, &y);

    f(1, 0);
    MUSTALIAS(x, &y);
    NOALIAS(x, &z);

    return 0;
}