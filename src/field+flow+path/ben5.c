#include "aliascheck.h"

int main(){

	int **p, *q,*a,b;
	//p = q;
	p = &a;
	if(q)
	q = &b;
	if(p)
	*p = q;
	MAYALIAS(*p,q);
	//p = q;
	printf("%d,%d %d %d",*p,*q,a,b);

	/* AUTOGEN_ALIASCHECK */
	MAYALIAS(p, &a);
	MAYALIAS(q, &b);
	NOALIAS(p, q);
	/* END_AUTOGEN_ALIASCHECK */
}

