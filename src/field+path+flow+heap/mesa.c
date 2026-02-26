#include "aliascheck.h"
/*
 * Testcases from 177.mesa.
 * Author: Sen Ye
 * Date: 12/05/2014
 *
 * field + path + flow + heap (MUST/NO version)
 */
#include <stdio.h>
#include <stdlib.h>

__attribute__((noinline))
void gl_Accum(int* x, unsigned y, float z) {
  (void)x; (void)y; (void)z;
}

struct gl_api_table {
  void (*Accum)(int*, unsigned, float);
};

__attribute__((noinline))
void check_pointers(struct gl_api_table* table)
{
  void **entry;
  int numentries = (int)(sizeof(struct gl_api_table) / sizeof(void*));
  int i;

  entry = (void **)table;

  for (i = 0; i < numentries; i++) {
    if (!entry[i])
      printf("found uninitialized function pointer at %d\n", i);
  }
}

__attribute__((noinline))
void init_exec_pointers(struct gl_api_table *table)
{
  table->Accum = gl_Accum;
}

int main(int argc, char **argv)
{
  (void)argv;

  /* heap */
  struct gl_api_table* table =
      (struct gl_api_table*)calloc(1, sizeof(struct gl_api_table));

  /* flow: initialise function pointer field */
  init_exec_pointers(table);

  /* extra use */
  check_pointers(table);

  /* -------- AUTOGEN_ALIASCHECK (field + path + flow + heap) -------- */
  {
    /* field: address of heap object equals itself */
    MUSTALIAS(table, table);

    /* field: taking address of a field is within the same heap object */
    /* (and two different fields would be NOALIAS, but here only one field exists) */
    MUSTALIAS(&table->Accum, &((table)->Accum));

    /* flow: copy out the field, and relate it back */
    void (*fp1)(int*, unsigned, float) = table->Accum;
    void (*fp2)(int*, unsigned, float) = gl_Accum;

    MUSTALIAS(fp1, fp2);

    /* path: branch on the field value; both sides have defined checks */
    if (table->Accum) {
      MUSTALIAS(table->Accum, gl_Accum);
    } else {
      /* unreachable given init_exec_pointers, but still a defined NO check */
      NOALIAS(table->Accum, gl_Accum);
    }

    /* heap: a second heap object must not alias the first */
    struct gl_api_table* table2 =
        (struct gl_api_table*)calloc(1, sizeof(struct gl_api_table));
    init_exec_pointers(table2);

    NOALIAS(table, table2);

    /* field+heap: addresses of fields from different heap objects do not alias */
    NOALIAS(&table->Accum, &table2->Accum);

    free(table2);
  }
  /* -------- END_AUTOGEN_ALIASCHECK -------- */

  free(table);
  return 0;
}