#include "aliascheck.h" 

int **x, *y;
int z;

void f(int **p);

void main(){
	x = &y;
	f(x);
}

void f(int **p){
	int k;
	y = &k;
	if (z){
		*p = &z;
		MUSTALIAS(y,&z);
		NOALIAS(y,&k);

		f(p);
	}
	// 	Why this is true (if execution paths are taken into account)
	
	// The key point is that z is never modified anywhere in f.
	
	// If z == 0, the if (z) branch is not taken, and after the if we still have y = &k → MUSTALIAS(y, &k).
	
	// If z != 0, the branch is taken, f(p) is called, and then it keeps calling itself recursively forever (because z still does not change).
	// That means control never reaches the program point after the if.
	
	// Therefore, at the program point after the if, there is only one actually reachable scenario: z == 0, and in that case y == &k.
	NOALIAS(y,&z);
	MUSTALIAS(y,&k);
}

