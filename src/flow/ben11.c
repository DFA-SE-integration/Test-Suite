#include "aliascheck.h"
int main(){



	int *p, *q, c,d;

	p  =&c;
	q = &d;

	p = q;

	*p = 100;
	*q = 100;

	/* AUTOGEN_ALIASCHECK */
	NOALIAS(p, &c);
	MUSTALIAS(q, &d);
	MUSTALIAS(p, q);
	MUSTALIAS(p, &d);
	NOALIAS(q, &c);
	/* END_AUTOGEN_ALIASCHECK */
}

