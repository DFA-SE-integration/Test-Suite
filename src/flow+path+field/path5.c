#include "aliascheck.h"

struct agg {
    int *pi;
    int k;
};

int main(int argc, char **argv){
    int *s = 0;
    int k1 = 0, k2 = 0;

    struct agg obj1, obj2;
    struct agg *a = &obj1, *b = &obj2;

    obj1.pi = 0;
    obj2.pi = 0;

    if (argc) {
        a = &obj1;
        s = &k1;

        a->pi = s;

        MUSTALIAS(obj1.pi, &k1);
        NOALIAS(obj1.pi, &k2);

        /* на этом пути obj2.pi не трогали */
        NOALIAS(obj2.pi, &k1);
        NOALIAS(obj2.pi, &k2);
    } else {
        b = &obj2;
        s = &k2;

        b->pi = s;

        MUSTALIAS(obj2.pi, &k2);
        NOALIAS(obj2.pi, &k1);

        /* на этом пути obj1.pi не трогали */
        NOALIAS(obj1.pi, &k1);
        NOALIAS(obj1.pi, &k2);
    }

    return 0;
}