/*
 * Alias with array (field + flow + path), MUST/NO version
 * Author: Sen Ye
 * Date: 06/09/2013
 */
#include "aliascheck.h"

struct MyStruct {
  int *f1;
  int *f2;
};

int main(int argc, char **argv) {
  (void)argv;

  struct MyStruct s[2] = {0};
  int a, b, c, d;
  (void)d;

  /* field */
  s[0].f1 = &a;
  s[0].f2 = &c;

  /* effects (flow) */
  if (argc > 1) {
    s[1].f2 = s[0].f1;  /* -> &a */
  } else {
    s[1].f2 = &b;       /* -> &b */
  }

  /* merge */
  int *t = s[1].f2;

  /* checks (path), отдельно от эффектов */
  if (argc > 1) {
    MUSTALIAS(s[0].f1, t);
  } else {
    NOALIAS(s[0].f1, t);
  }

  return 0;
}