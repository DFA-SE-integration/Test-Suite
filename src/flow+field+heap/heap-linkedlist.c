#include <stdlib.h>
#include "aliascheck.h"

struct Node {
    int *data;
    struct Node *next;
};

int main(void) {
    struct Node *head, *p, *q;
    int i;

    /* heap object #1 (Node) */
    head = (struct Node *)malloc(sizeof(struct Node));
    head->data = (int *)malloc(sizeof(int));   /* heap object #A (int) */
    head->next = NULL;

    p = head;

    /* Build a chain of 2 more nodes (total = 3 nodes) */
    for (i = 0; i < 2; ++i) {
        p->next = (struct Node *)malloc(sizeof(struct Node));   /* heap object #2/#3 (Node) */
        p = p->next;
        p->data = (int *)malloc(sizeof(int));                   /* heap object #B/#C (int) */
        p->next = NULL;
    }

    /* q aliases the 2nd node */
    q = head->next;

    /* -------- field + heap checks -------- */
    MUSTALIAS(q, head->next);
    MUSTALIAS(q->data, head->next->data);

    NOALIAS(head, head->next);
    NOALIAS(head->next, head->next->next);

    NOALIAS(head->data, head->next);              /* int-heap vs Node-heap */
    NOALIAS(head->data, head->next->data);        /* different int heap objects */
    NOALIAS(head->next->data, head->next->next);  /* int-heap vs Node-heap */
    NOALIAS(head->next->data, head->next->next->data);

    /* -------- flow check (strong update on a field) -------- */
    head->next->data = head->data;
    MUSTALIAS(head->next->data, head->data);

    head->next->data = (int *)malloc(sizeof(int));   /* overwrite previous alias */
    NOALIAS(head->next->data, head->data);

    return 0;
}