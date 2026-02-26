#include "aliascheck.h"

int main(int argc, char **argv) {
  (void)argv;

  int **p, *q;
  int *b = 0, *c = 0;
  int d = 1, e = 2;

  /* init */
  p = &c;
  q = &e;

  /* effects (path + flow) */
  int take = (argc > 1);
  if (take) {
    p = &b;      /* store goes to b */
    /* q stays &e */
  } else {
    /* p stays &c */
    q = &d;      /* value becomes &d */
  }

  /* merge */
  int **pm = p;
  int  *qm = q;

  /* effect */
  *pm = qm;

  /* checks (path) */
  if (take) {
    MUSTALIAS(b, &e);
    NOALIAS(b, &d);

    NOALIAS(c, &e);
    NOALIAS(c, &d);
  } else {
    MUSTALIAS(c, &d);
    NOALIAS(c, &e);

    NOALIAS(b, &d);
    NOALIAS(b, &e);
  }

  /* path-independent from above */
  NOALIAS(c, &e);
  NOALIAS(b, &d);

  return 0;
}