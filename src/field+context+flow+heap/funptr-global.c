#include "aliascheck.h"
#include <stdlib.h>

/*
 * functionpointer1.c
 * Test function pointer with field initialization of globals
 *
 * MUST/NO version (field + context + flow + heap)
 */

typedef int PRSize;
typedef unsigned int PRUint32;

struct PLHashAllocOps {
  void *(*allocTable)(void *pool, PRSize size);
};
typedef struct PLHashAllocOps PLHashAllocOps;

__attribute__((noinline))
static void *DefaultAllocTable(void *pool, PRSize size) {
  (void)pool;
  return malloc((size_t)size);
}

PLHashAllocOps defaultHashAllocOps = { &DefaultAllocTable };

__attribute__((noinline))
void PL_NewHashTable(PRUint32 n, void *allocPriv) {
  (void)n;

  PLHashAllocOps const *allocOps =
      (PLHashAllocOps const *)(&defaultHashAllocOps);

  void *tmp0 = (*(allocOps->allocTable))(allocPriv, (int)sizeof(int));
  void *tmp1 = (*(allocOps->allocTable))(allocPriv, (int)sizeof(int));

  /* heap: two separate mallocs must not alias */
  NOALIAS(tmp0, tmp1);

  /* field: the function pointer loaded from the global struct is exact */
  MUSTALIAS(allocOps->allocTable, DefaultAllocTable);

  free(tmp0);
  free(tmp1);
}

int main(void) {
  PL_NewHashTable(1, 0);
  return 0;
}