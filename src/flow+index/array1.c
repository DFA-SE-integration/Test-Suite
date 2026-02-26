#include "aliascheck.h"
int main(){
    int* a[100];
    int b,c,k;
    int e[20];

    e[0] = 100;

    k = e[0]; 
    a[15] = &b;

    a[18] = &c;
    
    *a[18] = 100;
    
    k = *a[18];

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(a, a);
    MUSTALIAS(a[15], &b);
    MUSTALIAS(a[18], &c);
    NOALIAS(a[15], a[18]);
    /* END_AUTOGEN_ALIASCHECK */
}

