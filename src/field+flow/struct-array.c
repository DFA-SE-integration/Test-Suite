/*
 * Struct with array.
 * Author: Sen Ye
 * Date: 28/04/2014
 */
#include "aliascheck.h"

struct ArrayStruct {
  int f1;
  char f2;
  int f3[100];
  int f4;
};

int main() {
  struct ArrayStruct* p;
  struct ArrayStruct s;
  int* q;

  p = &s;

  /* делаем указатели заведомо на один и тот же элемент массива */
  q = &s.f3[10];
  MUSTALIAS(&p->f3[10], q);

  /* разные элементы массива не могут пересекаться */
  NOALIAS(&p->f3[20], &p->f3[30]);
  NOALIAS(&s.f3[0], &s.f3[99]);

  /* поле массива и соседнее скалярное поле — разные области */
  NOALIAS(&p->f3[0], &s.f4);

  return 0;
}