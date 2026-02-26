/*
 * array-constIdx.c
 * Author: Sen Ye
 * Date: 06/09/2013
 */
#include "aliascheck.h"

struct MyStruct {
  int *f1;
  int *f2;
};

int main() {
  struct MyStruct s;
  int a, b;

  s.f1 = &a;
  s.f2 = &b;

  // Только полевая чувствительность:
  // field-sensitive -> NOALIAS
  NOALIAS(s.f1, s.f2);

  return 0;
}
