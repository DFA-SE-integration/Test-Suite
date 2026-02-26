#include "aliascheck.h"

struct Ops {
    void (*set)(int **slot, int *v);
};

struct Node {
    int tag;
    struct Ops *ops;
    int *payload;
};

void set_impl(int **slot, int *v) {
    *slot = v;
}

struct Ops G = { .set = set_impl };

void update(struct Node *n, int *v) {
    /* field-sensitive access: n->ops->set and n->payload */
    n->ops->set(&n->payload, v);
}

int main(void) {
    int a, b;
    struct Node n1, n2;

    n1.ops = &G;
    n2.ops = &G;
    n1.payload = 0;
    n2.payload = 0;

    /* same callee, different calling contexts */
    update(&n1, &a);
    update(&n2, &b);

    /* field + context sensitive facts */
    MUSTALIAS(n1.payload, &a);
    MUSTALIAS(n2.payload, &b);
    NOALIAS(n1.payload, n2.payload);

    return 0;
}