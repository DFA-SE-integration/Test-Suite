/*
 * Return a struct instance from function.
 * Author: Sen Ye
 * Date: 07/05/2014
 */
#include "aliascheck.h"

struct MyStruct {
  int* f1;
  char f2;
};

int x, y;

__attribute__((noinline))
struct MyStruct foo() {
  struct MyStruct m;
  m.f1 = &x;
  m.f2 = 0;
  return m;
}

int main() {
  struct MyStruct m;
  m = foo();

  MUSTALIAS(m.f1, &x);
  NOALIAS(m.f1, &y);

  return 0;
}