#include "aliascheck.h"
typedef struct{
    int f1, f2;
}FOO;

int *z;


int main(){
    FOO a;
    int *b;
    b = &a.f1;
    *b = 100;

    /* AUTOGEN_ALIASCHECK */
    MAYALIAS(z, z);
    /* END_AUTOGEN_ALIASCHECK */
}

