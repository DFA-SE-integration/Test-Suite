#include "aliascheck.h"
#include <stdint.h>

struct MyStruct { int f1; int f2; };

int main(void) {
    struct MyStruct ms;
    int *p = &ms.f1;

    uintptr_t addr = (uintptr_t)p;
    int *q = (int*)(addr + sizeof(int)); /* пытаемся попасть в f2 */

    /* всё равно зависит от layout/выравнивания, но обычно p и q разные */
    NOALIAS(p, q);
    return 0;
}