#include "aliascheck.h"
int main(){
    char *a = "hello";

    *a = "fdf";

    /* AUTOGEN_ALIASCHECK */
    MAYALIAS(a, a);
    /* END_AUTOGEN_ALIASCHECK */
}

