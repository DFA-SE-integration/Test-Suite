// arraycopy1.c
#include "aliascheck.h"

int main() {
    int a, b;
    int* source[2] = {&a, &b};
    int* x = source[1];

    NOALIAS(&a, x);
    return 0;
}
