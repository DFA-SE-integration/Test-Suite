/*
 * Struct with array of structs.
 * Author: Sen Ye
 * Date: 28/04/2014
 */
#include "aliascheck.h"

struct InnerArrayStruct {
  int in1[10];
  char in2;
  double in3;
};

struct MidArrayStruct {
  char mid1;
  struct InnerArrayStruct mid2[5];
  double mid3[20];
};

struct ArrayStruct {
  int out1;
  char out2;
  struct MidArrayStruct out3;
  int out4;
};

int main() {
  struct ArrayStruct* p;
  struct ArrayStruct s;

  p = &s;

  MUSTALIAS(&p->out4, &s.out4);

  // Вместо OOB: корректная арифметика внутри in1[10]
  int *base = &p->out3.mid2[2].in1[0];
  int *elem = base + 7; // in-bounds: 0..9
  MUSTALIAS(elem, &s.out3.mid2[2].in1[7]);

  // Вместо OOB: разные элементы массива mid2[5] не алиасят
  NOALIAS(&p->out3.mid2[1].in1[0], &p->out3.mid2[2].in1[0]);

  NOALIAS(&p->out3.mid2[3].in3, &s.out3.mid3[2]);
  NOALIAS(&p->out3.mid2[0], &s.out4);

  return 0;
}