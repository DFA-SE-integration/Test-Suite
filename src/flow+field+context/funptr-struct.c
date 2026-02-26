#include <stdio.h>
#include "aliascheck.h"

int g;

__attribute__((noinline))
static int my_sn_write(int *p) {
    printf("Executing my_sn_write\n");
    MUSTALIAS(&g, p);
    return 0;
}

struct MYFILE {
    int (*pt)(int *p);
};

__attribute__((noinline))
void my_vfprintf(struct MYFILE *pts) {
    printf("Executing bar\n");
    int *p = &g;

    /* field + flow: load function pointer from struct field */
    int (*fp)(int*) = pts->pt;
    MUSTALIAS(fp, my_sn_write);

    fp(p);

    MUSTALIAS(p, &g);
}

__attribute__((noinline))
int my_vsnprintf(void) {
    struct MYFILE pts = { .pt = my_sn_write };

    /* context: pass address of stack object */
    my_vfprintf(&pts);

    return 0;
}

int main(void) {
    my_vsnprintf();
    return 0;
}