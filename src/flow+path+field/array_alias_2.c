/*
 * Alias with array (field + flow + path), MUST/NO version
 * Author: Sen Ye
 * Date: 06/09/2013
 */
#include "aliascheck.h"

struct MyStruct {
  int *f1;
  int *f2;
  float *f3;
};

int main(int argc, char **argv) {
  (void)argv;

  struct MyStruct s[3] = {0};
  int *p[2] = {0};

  int a, b, c, d;
  float x, y;

  /* init (field) */
  s[0].f1 = &a;  s[0].f2 = &c;  s[0].f3 = &x;
  s[1].f1 = &b;  s[1].f2 = &d;  s[1].f3 = &y;
  p[0] = &c;     p[1] = &d;

  /* --- fixed MUST/NO properties (no MAY) --- */

  /* same-field across elements: make it deterministically MUST */
  s[1].f1 = s[0].f1;              /* both -> &a */
  MUSTALIAS(s[0].f1, s[1].f1);

  /* p[0] aliases s[0].f2 by construction -> and we can also force s[1].f2 to same */
  s[1].f2 = p[0];                 /* both -> &c */
  MUSTALIAS(p[0], s[1].f2);

  /* float field: s[1].f3 is exactly &y */
  MUSTALIAS(s[1].f3, &y);

  /* different fields: keep them deterministically NO */
  NOALIAS(s[0].f1, s[1].f2);      /* &a vs &c */
  NOALIAS(p[1], s[1].f1);         /* &d vs &a */

  /* --- flow + path part --- */
  if (argc > 1) {
    s[1].f2 = &c;                 /* q -> &c */
  } else {
    s[1].f2 = &d;                 /* q -> &d */
  }

  int *q = s[1].f2;               /* merge */

  if (argc > 1) {
    MUSTALIAS(q, &c);
  } else {
    MUSTALIAS(q, &d);
  }

  /* and relate it to a fixed pointer: p[0] == &c */
  if (argc > 1) {
    MUSTALIAS(p[0], q);           /* &c vs &c */
  } else {
    NOALIAS(p[0], q);             /* &c vs &d */
  }

  return 0;
}