#include "aliascheck.h"
#include <stdio.h>

typedef struct arc {
    struct arc *nextout, *nextin;
    long ident;
} arc_t;

typedef struct network {
    arc_t *arcs, *stop_arcs;
    arc_t *dummy_arcs, *stop_dummy;
} network_t;

int main(int argc, char **argv){
    arc_t arcs[4];
    network_t net;
    arc_t *arc;
    void *stop;
    int condition = argc;

    net.arcs = &arcs[0];
    net.stop_arcs = &arcs[4];     /* one-past-end sentinel */
    stop = (void *)net.stop_arcs;

    for (arc = net.arcs; arc != (arc_t *)stop; arc++) {
        if (arc->ident) {
            if (condition)
                printf("hello world\n");
        }
    }

    /* AUTOGEN_ALIASCHECK */
    MUSTALIAS(stop, (void*)net.stop_arcs);
    NOALIAS(stop, (void*)net.arcs);
    /* END_AUTOGEN_ALIASCHECK */

    return 0;
}