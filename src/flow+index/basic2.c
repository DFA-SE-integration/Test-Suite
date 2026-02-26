// basic2.c
#include "aliascheck.h"

int main() {
  int *s[2];
  int x, y;
  int i = 0, j = 1;   // фиксируем разные индексы (без UB)

  s[i] = &x;
  s[j] = &y;

  NOALIAS(s[i], s[j]);

  return 0;
}
