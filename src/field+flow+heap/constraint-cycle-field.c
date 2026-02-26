#include <stdlib.h>
#include "aliascheck.h"

struct MyStruct {
    int *f1;
    struct MyStruct *next;
};

int main(void) {
    struct MyStruct *head = malloc(sizeof(struct MyStruct));
    struct MyStruct *p = head;
    int num = 2;

    head->next = NULL;
    head->f1 = NULL;

    while (num--) {
        p->next = malloc(sizeof(struct MyStruct));
        p->next->f1 = malloc(sizeof(int));
        p->next->next = NULL;
        p = p->next;
    }

    struct MyStruct *q = p;

    MUSTALIAS(q, p);
    MUSTALIAS(q->f1, p->f1);

    return 0;
}