#include "aliascheck.h"
int main(){
    int *p, *s,*r,*w,*q,*x;
    int ***m,**n,*z,*y,z1,y1;

    m=&n;
    n=&z;
    *m=&y;
    z=&z1;
    y=&y1;
    ***m=10;
    z=**m;

    /* AUTOGEN_ALIASCHECK */
    NOALIAS(m, &y);
    NOALIAS(n, &z);
    NOALIAS(z, &z1);
    MUSTALIAS(y, &y1);
    NOALIAS(m, n);
    
    /* можно добавить для усиления flow-sensitivity */
    MUSTALIAS(n, &y);
    MUSTALIAS(z, &y1);
    /* END_AUTOGEN_ALIASCHECK */
}

