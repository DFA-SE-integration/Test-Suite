#include "aliascheck.h"
#include <stdlib.h>

struct node{
    int* number;
} node;

int main(void){
    struct node node1, node2;
    struct node **_node;
    struct node node_arr[10];
    int k, *a;

    (void)a; /* unused */

    _node = malloc(sizeof(*_node) * 2);   /* array of 2 pointers to struct node */
    _node[0] = &node1;
    _node[1] = &node2;

    _node[0]->number = &k;
    *(_node[0]->number) = 1;

    node_arr[0].number = &k;
    *(node_arr[0].number) = 100;

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(_node[0]->number, &k);
    MUSTALIAS(node1.number, &k);
    MUSTALIAS(node_arr[0].number, &k);
    MUSTALIAS(_node[0]->number, node_arr[0].number);
    /* END_AUTOGEN_ALIASCHECK */

    free(_node);
    return 0;
}