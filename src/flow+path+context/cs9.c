#include "aliascheck.h"

int obj, t, s;
int *k = &s;

__attribute__((noinline))
void foo(int **p, int **q) {
    /* flow through pointer-to-pointer */
    *p = *q;
    *q = &obj;
}

/* разные контексты вызова одного и того же callee */
__attribute__((noinline)) void caller1(int **x, int **y) { foo(x, y); }
__attribute__((noinline)) void caller2(int **x, int **y) { foo(x, y); }

int main(int argc, char **argv) {
    (void)argv;

    int **x, **y;
    int *a, *b, *c, *d, *e;

    int A;
    a = &t;          /* как в оригинале: a сначала на глобальный t */
    b = &s;          /* важно инициализировать до первого foo */
    c = &s;
    d = &t;          /* важно инициализировать до второго foo */
    e = &t;          /* важно инициализировать до второго foo */
    (void)A;

    /* -------- Context 1: caller1(x=&a, y=&b) -------- */
    x = &a;
    y = &b;

    caller1(x, y);

    /* после foo: a = (старый b) == &s, b = &obj */
    NOALIAS(a, b);
    MUSTALIAS(b, &obj);

    *b = 5;

    /* flow: связываем a и c */
    c = &s;
    a = c;
    MUSTALIAS(a, c);

    /* -------- Context 2: caller2, выбор аргументов по пути -------- */
    t = (argc > 1);          /* defined path selector */

    /* effects (path) */
    if (t) {
        c = &obj;
        x = &c;
        y = &e;              /* foo пишет в e */
    } else {
        x = &d;
        y = &d;              /* foo пишет в d */
    }

    /* merge */
    int **xm = x;
    int **ym = y;

    /* flow */
    caller2(xm, ym);

    /* общие свойства (держатся на всех путях) */
    NOALIAS(a, d);

    /* checks (path) */
    if (t) {
        /* до вызова: e=&t, c=&obj
           foo: c = e(old) -> &t ; e = &obj
         */
        MUSTALIAS(e, &obj);
        NOALIAS(d, &obj);

        MUSTALIAS(c, &t);
        MUSTALIAS(c, d);         /* d был &t и не менялся */
        NOALIAS(a, c);           /* a=&s, c=&t */
    } else {
        /* до вызова: d=&t
           foo: d = d(old) -> &t ; d = &obj  => d=&obj
         */
        MUSTALIAS(d, &obj);

        NOALIAS(c, d);           /* c=&s, d=&obj */
        MUSTALIAS(a, c);         /* a=&s, c=&s */
    }

    /* flow после join: e всегда указывает на int-объект */
    *e = 10;

    return 0;
}