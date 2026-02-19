#include "aliascheck.h"
int main(){



	int *p, *q, c,d;

	p  =&c;
	q = &d;

	p = q;

	*p = 100;
	*q = 100;

	/* AUTOGEN_ALIASCHECK */
	MAYALIAS(p, &c);
	MAYALIAS(q, &d);
	NOALIAS(p, q);
	/* END_AUTOGEN_ALIASCHECK */
}

