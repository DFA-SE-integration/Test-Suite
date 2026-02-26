#include "aliascheck.h"
#include <stdio.h>

int main(int argc, char **argv) {
  (void)argv;

  int **p;
  int *q, *a;
  int b, c;

  /* init */
  p = &a;

  /* effects (flow) */
  if (argc > 1) {
    q = &b;
  } else {
    q = &c;
  }

  /* check (path) — отдельно от эффектов */
  if (argc > 1) {
    MUSTALIAS(q, &b);
    NOALIAS(q, &c);
  } else {
    MUSTALIAS(q, &c);
    NOALIAS(q, &b);
  }

  /* effect */
  if (p)
    *p = q;

  /* always true after assignment */
  MUSTALIAS(*p, q);

  /* AUTOGEN_ALIASCHECK */
  MUSTALIAS(p, &a);
  if (argc > 1) {
    MUSTALIAS(q, &b);
  } else {
    NOALIAS(q, &b);
  }
  NOALIAS(p, q);
  /* END_AUTOGEN_ALIASCHECK */

  /* safe debug */
  printf("%p %p %p\n", (void*)p, (void*)q, (void*)*p);
  return 0;
}