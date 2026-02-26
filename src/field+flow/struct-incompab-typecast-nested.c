#include "aliascheck.h"

struct InnerStruct { char in1; int* in2; };

struct SrcStruct {
    int* f1[10];
    char f2[10];
    struct InnerStruct f3[5];
    char f4;
};

struct DstStruct {
    int* f1[10];
    char f2[20];
    struct InnerStruct f3[5];
};

int main(void) {
    struct DstStruct* pdst;
    struct SrcStruct* psrc;
    struct SrcStruct s = {0};   /* важно: всё NULL/0 */
    int x, y, z;

    psrc = &s;
    psrc->f1[3] = &x;
    psrc->f3[2].in2 = &y;

    pdst = (struct DstStruct*)psrc;  /* явный cast (всё ещё type punning) */

    /* f1[9] не задавали => NULL */
    NOALIAS(pdst->f1[9], &x);

    /* из-за разных layout это поле не обязано совпасть с psrc->f3[2].in2,
       а у нас оно изначально NULL => точно не &y */
    NOALIAS(pdst->f3[3].in2, &y);

    /* тоже NULL */
    NOALIAS(psrc->f1[2], &z);

    pdst->f3[1].in2 = &z;

    /* psrc->f3[1].in2 мы не трогали => всё ещё NULL */
    NOALIAS(psrc->f3[1].in2, &z);

    return 0;
}