/*
 * field + context + flow  (rewritten from 254.gap-style idea)
 * - deterministic (no UB, no uninitialized)
 * - context: same callee chain NewBag/StoreFunc/LoadFunc called from two callers
 * - flow: strong updates of FreeHandle + stored function pointer
 * - field: distinct fields inside a heap-allocated State struct
 */
#include <stdlib.h>
#include "aliascheck.h"

__attribute__((noinline))
char *SyGetmem(long size) {
    return (char*)calloc(1, (size_t)size);
}

typedef struct TypHeader {
    struct TypHeader **ptr; /* “bag payload” slot (we store a function pointer there) */
} *TypHandle;

TypHandle HdFree;
TypHandle FreeHandle;
TypHandle *FirstBag; /* unused here, kept to resemble original globals */

__attribute__((noinline)) void IntComm(void) {}
__attribute__((noinline)) void OtherComm(void) {}

__attribute__((noinline))
TypHandle NewBag(void) {
    TypHandle h = (TypHandle)SyGetmem((long)sizeof(*h));

    /* allocate one slot to store a function pointer */
    void (**slot)(void) = (void (**)(void))SyGetmem((long)sizeof(void (*)(void)));

    /* store slot address into field (type-punned but used consistently) */
    h->ptr = (TypHandle*)slot;

    /* flow: strong update of global */
    FreeHandle = h;
    return h;
}

__attribute__((noinline))
void StoreFunc(TypHandle h, void (*func)(void)) {
    *(void (**)(void))(h->ptr) = func;
}

__attribute__((noinline))
void (*LoadFunc(TypHandle h))(void) {
    return *(void (**)(void))(h->ptr);
}

/* two different calling contexts */
__attribute__((noinline))
TypHandle CallA(void) {
    TypHandle h = NewBag();
    StoreFunc(h, IntComm);
    return h;
}

__attribute__((noinline))
TypHandle CallB(void) {
    TypHandle h = NewBag();
    StoreFunc(h, OtherComm);
    return h;
}

/* field-sensitivity carrier on heap */
struct State {
    TypHandle A;
    TypHandle B;
};

int main(void) {
    /* init globals deterministically */
    HdFree = (TypHandle)SyGetmem((long)sizeof(*HdFree));
    FreeHandle = HdFree;
    FirstBag = 0;

    /* context #1 */
    TypHandle h1 = CallA();
    MUSTALIAS(FreeHandle, h1);

    void (*f1)(void) = LoadFunc(h1);
    MUSTALIAS(f1, IntComm);
    NOALIAS(f1, OtherComm);

    /* context #2 */
    TypHandle h2 = CallB();
    MUSTALIAS(FreeHandle, h2);

    void (*f2)(void) = LoadFunc(h2);
    MUSTALIAS(f2, OtherComm);
    NOALIAS(f2, IntComm);

    /* heap objects must be distinct */
    NOALIAS(h1, h2);
    NOALIAS(h1->ptr, h2->ptr);

    /* field checks on a heap object containing two fields */
    struct State *st = (struct State*)SyGetmem((long)sizeof(*st));
    st->A = h1;
    st->B = h2;

    NOALIAS(&st->A, &st->B);
    MUSTALIAS(st->A, h1);
    MUSTALIAS(st->B, h2);
    NOALIAS(st->A, st->B);

    return 0;
}