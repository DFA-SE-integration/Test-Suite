#include "aliascheck.h"

int main(){
    char *a = "hello";

    *a = "fdf";

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(a, a);
    /* END_AUTOGEN_ALIASCHECK */
}

