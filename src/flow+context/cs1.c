#include "aliascheck.h"
void foo(int **x, int **y){

	*x = *y;
}

int main(){

	int *a, *b;
	int a1, b1;
	a = &a1;
	b = &b1;
	NOALIAS(a, b);
	foo(a,b);
	MUSTALIAS(a, b);
}
