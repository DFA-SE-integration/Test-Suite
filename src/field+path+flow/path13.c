#include "aliascheck.h"

struct agg { int **i; };

int main(int argc, char **argv){
    int *b = 0, *c = 0, *d = 0;
    int f = argc;      /* path condition without UB */
    int w = 0;

    struct agg ag1;
    struct agg *a = &ag1;

    if (f) {
        a->i = &c;
        b = &f;
    } else {
        a->i = &d;
        b = &w;
    }

    *(a->i) = b;

    /* AUTOGEN_ALIASCHECK */
	if (f) {
		MUSTALIAS(c, &f);   /* only on the 'then' path */
	} else {
		MUSTALIAS(d, &w);   /* only on the 'else' path */
	}
    NOALIAS(c, &w);    /* always */
    NOALIAS(d, &f);    /* always */
    /* END_AUTOGEN_ALIASCHECK */

    return 0;
}