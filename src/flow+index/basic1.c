// basic1.c
#include "aliascheck.h"

int main() {
  int *s[2];
  int a, b;

  s[0] = &a;
  s[1] = &b;

  // Pure index sensitivity:
  // index-sensitive -> NOALIAS
  NOALIAS(s[0], s[1]);

  return 0;
}
