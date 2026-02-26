/*
 * Pointer arithmetic (defined version)
 * Author: Sen Ye
 * Date: 06/09/2013
 */
#include "aliascheck.h"

struct s {
  int* f[2];
};

int main() {
  int **q, **p;
  int a, b;
  struct s s1;

  s1.f[0] = &a;
  s1.f[1] = &b;

  q = &s1.f[0];
  p = q + 1;            // переход ко 2-му элементу массива (defined)

  MUSTALIAS(*p, &b);    // *p == s1.f[1] == &b

  return 0;
}