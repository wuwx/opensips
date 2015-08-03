#ifndef API_H
#define	API_H

#include "../../str.h"

#define STATUS_PERMANENT_DOWN 0
#define STATUS_UP 1
#define STATUS_TEMPORARY_DOWN 2

typedef struct clusterer_node_ clusterer_node_t;

struct clusterer_node_ {
    /* machine_id */
    int machine_id;
    /* machine state */
    int state;
    /* description */
    str description;
    /* protocol */
    int proto;
    /* sock address */
    union sockaddr_union addr;
    /* linker in list */
    clusterer_node_t *next;
};

typedef clusterer_node_t * (*get_nodes_f) (int, int);
typedef int (*set_state_f) (int, int, int, int);
typedef void (*free_nodes_f) (clusterer_node_t *);
typedef int (*check_connection_f) (int, union sockaddr_union*, int, int);
typedef int (*get_my_id_f) (void);

struct clusterer_binds {
    get_nodes_f get_nodes;
    free_nodes_f free_nodes;
    set_state_f set_state;
    check_connection_f check;
    get_my_id_f get_my_id;
};




typedef int(*load_clusterer_f)(struct clusterer_binds *binds);

int load_clusterer(struct clusterer_binds *binds);

static inline int load_clusterer_api(struct clusterer_binds *binds) {
    load_clusterer_f load_clusterer;

    /* import the DLG auto-loading function */
    if (!(load_clusterer = (load_clusterer_f) find_export("load_clusterer", 0, 0)))
        return -1;
 
    /* let the auto-loading function load all DLG stuff */
    if (load_clusterer(binds) == -1)
        return -1;

    return 0;
}

#endif	/* API_H */