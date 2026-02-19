#include "aliascheck.h"
struct agg{

	int* f;
	int *g;

}agg;

int main(){
	int a1, b1,c1;
	int *a, *b,*c;
	struct agg agg1[100];
	a = &a1;
	b = &b1;
	agg1[1].f = a;
	agg1[1].g = b;
	//agg1[0].f = &c;

	/* AUTOGEN_ALIASCHECK */
	MAYALIAS(a, &a1);
	MAYALIAS(b, &b1);
	MAYALIAS(f, &c);
	NOALIAS(a, b);
	/* END_AUTOGEN_ALIASCHECK */
}

