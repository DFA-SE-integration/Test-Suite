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

  struct MyStruct s[3] = {0};
  int a, b, c, d;

  s[0].f1 = &a;
  s[0].f2 = &c;

  s[1].f1 = &b;
  s[1].f2 = &d;

  /* effects (flow) */
  if (argc > 1) {
    s[1].f1 = &c;      /* extra write (field+flow) */
  }

  /* merge */
  int *t = s[1].f2;    /* always &d */

  /* checks (path) */
  NOALIAS(s[0].f1, t); /* &a vs &d, всегда no-alias */

  return 0;
}