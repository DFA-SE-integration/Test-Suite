/*
 * Array alias in flow-sensitive analysis (MUST/NO version).
 * Author: Sen Ye
 * Date: 08/11/2013
 */

 #include "aliascheck.h"

 struct MyStruct {
   int *f1;
   int *f2;
 };
 
 int main() {
   struct MyStruct s[2] = {0};
   int x, y;
 
   s[0].f1 = &x;
   s[0].f2 = &y;
 
   /* сделаем поведение определённым: зафиксируем связи между элементами массива */
   s[1].f1 = s[0].f1;  /* == &x */
   s[1].f2 = s[0].f2;  /* == &y */
 
   /* field-sensitivity */
   NOALIAS(s[0].f1, s[0].f2);
   NOALIAS(s[0].f1, s[1].f2);
 
   /* same-field relations across elements (deterministic) */
   MUSTALIAS(s[0].f1, s[1].f1);
   MUSTALIAS(s[0].f2, s[1].f2);
 
   /* flow-sensitivity */
   s[0].f1 = &y;
   MUSTALIAS(s[0].f1, s[1].f2);
   NOALIAS(s[0].f1, s[1].f1);
 
   return 0;
 }