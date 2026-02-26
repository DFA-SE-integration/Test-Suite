#include "aliascheck.h"
#include <stdio.h>

int main(int argc, char **argv){
    int a = 0, b = 0, c = 0;
    int *p = 0, *q = 0;
    int i = argc;          /* инициализированный флаг */

    /* PATH: p выбирается по пути */
    if (i)
        p = &a;
    else
        p = &b;

    q = p;                 /* сохраняем path-значение */

    *p = 10;
    c = *p;

    /* FLOW: strong update убивает прошлый выбор пути для p */
    p = &a;
    *p = 20;

    /* проверяем path-эффект через q и отношение p/q */
    if (i) {
        MUSTALIAS(q, &a);
        MUSTALIAS(p, q);   /* оба &a */
    } else {
        MUSTALIAS(q, &b);
        NOALIAS(p, q);     /* p=&a, q=&b */
    }

    /* и отдельный flow-чек */
    MUSTALIAS(p, &a);

    printf("%d,%d,%d,%d\n", *p, a, b, c);
    return 0;
}