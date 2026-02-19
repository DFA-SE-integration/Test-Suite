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
    MAYALIAS(m, &y);
    MAYALIAS(n, &z);
    MAYALIAS(z, &z1);
    MAYALIAS(y, &y1);
    NOALIAS(m, n);
    /* END_AUTOGEN_ALIASCHECK */
}

