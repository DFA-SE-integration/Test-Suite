/*
 * Struct with array of structs and pointers.
 * Author: Sen Ye
 * Date: 28/04/2014
 */
#include "aliascheck.h"

struct InnerArrayStruct {
  int* in1[10];
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
  int* out4;
};

int main() {
  struct ArrayStruct* p;
  struct ArrayStruct s = {0};   // важно: все указатели в агрегате -> NULL
  int x, y;

  s.out4 = &x;
  p = &s;

  p->out3.mid2[2].in1[2] = s.out4; // == &x
  p->out3.mid2[3].in1[3] = &y;     // == &y

  // Индекс [1][1] не трогали -> NULL, значит точно не алиасит &x
  NOALIAS(p->out3.mid2[1].in1[1], &x);

  // Ровно этот элемент мы присвоили &y -> точный MUSTALIAS
  MUSTALIAS(s.out3.mid2[3].in1[3], &y);

  return 0;
}