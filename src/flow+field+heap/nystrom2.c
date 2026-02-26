#include <stdlib.h>
#include "aliascheck.h"

int a,b;

struct foo{
    int x;
    int y;
};
typedef struct foo FOO;

void allocation(FOO **s){
    *s = (FOO*)malloc(sizeof(FOO));
}

FOO *ex(void){
    int *q;
    FOO *p;

    allocation(&p);

    q = &a;

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(q, &a);
    /* END_AUTOGEN_ALIASCHECK */

    return p;
}

int main(void){
    FOO *r = ex();
    free(r);
    return 0;
}