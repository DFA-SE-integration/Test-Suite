#include "aliascheck.h"
#include <stdlib.h>

int z1, z2;

void foo(int **p);

void bar(int **a) {
	if (z1 > 5) return;
	z1++;

	int *c, b;

	/* strong update expected: after this, *a points only to &b */
	*a = &b;
	c = *a;

	MUSTALIAS(c, &b);
	NOALIAS(c, &z1);   /* no-alias when strong updates are enabled */
	NOALIAS(c, &z2);

	foo(a);
}

void foo(int **p) {
	/* heap */
	p = (int**)malloc(sizeof(int*));
	*p = &z2;

	bar(p);
}

int main() {
	int **x, *y;

	x = &y;
	y = &z1;

	foo(x);
	return 0;
}