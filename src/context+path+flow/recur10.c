#include "aliascheck.h"

/* globals (static storage => zero-initialized) */
int **p, *x;
int y, z;

__attribute__((noinline))
void f(void) {
  p = &x;

  /* path + flow, with a bounded recursion using z as a depth counter */
  if (z) {
    /* effect */
    z--;

    /* effect */
    *p = &y;
    /* check */
    MUSTALIAS(x, &y);

    /* context: recursive call #1 */
    f();

    /* effect */
    *p = &z;
    /* checks */
    MUSTALIAS(x, &z);
    NOALIAS(x, &y);

    /* context: recursive call #2 */
    f();
  }
}

int main(void) {
  z = 1;   /* enable the path once, but keep recursion finite */
  f();
  return 0;
}