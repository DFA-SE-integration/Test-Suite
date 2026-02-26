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

struct MyStruct {
    const struct MYFILE *myfile;
};

const struct MYFILE pts = { .pt = my_sn_write };
const struct MyStruct ms = { .myfile = &pts };

__attribute__((noinline))
void my_vfprintf(const struct MyStruct *ms_in) {
    printf("Executing bar\n");
    int *p = &g;

    /* field + flow: load function pointer from nested field and call it */
    int (*fp)(int*) = ms_in->myfile->pt;
    MUSTALIAS(fp, my_sn_write);

    fp(p);

    /* still deterministic */
    MUSTALIAS(p, &g);
}

int main(void) {
    my_vfprintf(&ms);
    return 0;
}