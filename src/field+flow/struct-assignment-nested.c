#include "aliascheck.h"

struct InnerArrayStruct {
    int* in1[10];
    int* in2[20];
    char in3;
};

struct MidArrayStruct {
    char mid1[10];
    struct InnerArrayStruct mid2[5];
};

struct ArrayStruct {
    char out2;
    struct MidArrayStruct out3;
    int* out4;
};

int main(void) {
    struct ArrayStruct *p;
    struct ArrayStruct s1 = {0}, s2 = {0};   /* важно: всё в 0/NULL */
    int x, y;

    s1.out4 = &x;
    p = &s1;
    p->out3.mid2[3].in1[3] = &y;

    s2 = s1;  /* flow: копирование всего объекта */

    MUSTALIAS(s2.out4, &x);

    /* это единственное место, куда писали &y */
    MUSTALIAS(s2.out3.mid2[3].in1[3], &y);

    /* прочие элементы были NULL и такими же скопировались */
    NOALIAS(s2.out3.mid2[1].in1[1], &y);
    NOALIAS(s2.out3.mid2[3].in1[4], &y);

    return 0;
}