#include "linear_sequence_assoc_hash.h"
#include <math.h>

typedef int _list_index;
const int LIST_COUNT = 2048;
const double FACTOR = 0.6180339887499; /* Кнут рекомендует золотое сечение (sqrt(5) - 1)/2 */
const unsigned int FACTOR_STEP = 10;

typedef enum {
    DEREFERENCABLE,
    PASTREAR,
} _iter_state;

typedef struct node_s {
    LSQ_KeyT key;
    LSQ_BaseTypeT value;
    struct node_s *next;
} _node;

typedef struct {
    LSQ_SizeT size;
    _node **table;
    LSQ_Callback_CloneFuncT *keyCloneFunc;
    LSQ_Callback_SizeFuncT *keySizeFunc;
    LSQ_Callback_CompareFuncT *keyCompFunc;
    LSQ_Callback_CloneFuncT *valCloneFunc;
} _container;

typedef struct {
    _iter_state state;
    _container *con;
    _node *node;
    _list_index index;
} _iterator;

static _list_index calc_hash(const _container *const c, const LSQ_KeyT k) {
    int size = c->keySizeFunc(k);
    unsigned int h = 0;
    int i;
    double ip;

    for (i = 0; i < size; i++) h += ((char *)k)[i] * (i + 1) * FACTOR_STEP;
    return (_list_index)(modf(h * FACTOR, &ip) * (LIST_COUNT - 1));
}

static _iterator *create_iter(_container *const c, const _list_index i, _node *const n, const _iter_state s) {
    _iterator * iter = malloc(sizeof(_iterator));
    iter->con = c;
    iter->index = i;
    iter->node = n;
    iter->state = s;
    return iter;
}

static _node *create_node(const LSQ_KeyT k, const LSQ_BaseTypeT v, _node *const n) {
    _node *node = malloc(sizeof(_node));
    node->key = k;
    node->value = v;
    node->next = n;
    return node;
}

static void free_node(_node *n) {
    free(n->key);
    free(n->value);
    free(n);
}

LSQ_HandleT LSQ_CreateSequence(LSQ_Callback_CloneFuncT keyCloneFunc, LSQ_Callback_SizeFuncT keySizeFunc,
                               LSQ_Callback_CompareFuncT keyCompFunc, LSQ_Callback_CloneFuncT valCloneFunc) {
    _container *c = malloc(sizeof(_container));
    c->table = calloc(LIST_COUNT, sizeof(_node));
    c->size = 0;
    c->keyCloneFunc = keyCloneFunc;
    c->keySizeFunc  = keySizeFunc;
    c->keyCompFunc  = keyCompFunc;
    c->valCloneFunc = valCloneFunc;
    return c;
}

void LSQ_DestroySequence(LSQ_HandleT handle) {
    _node *n;
    _node *pn;
    _container *con = handle;
    _list_index i;

    for (i = LIST_COUNT - 1; i >= 0; i--) {
        n = con->table[i];
        while (n  != NULL) {
            pn = n;
            n = n->next;
            free_node(pn);
        }
    }

    free(con->table);
    free(con);
}

LSQ_SizeT LSQ_GetSize(LSQ_HandleT handle) {
    return ((_container *)handle)->size;
}

int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->state == DEREFERENCABLE;
}

int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->state == PASTREAR;
}

int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
    return 0;
}

LSQ_BaseTypeT LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->node->value;
}

LSQ_KeyT LSQ_GetIteratorKey(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->node->key;
}

void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
    free(iterator);
}

LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
    _container *c = handle;
    _list_index i;
    for (i = 0; i < LIST_COUNT; i++)
        if (c->table[i] != NULL)
            return create_iter(c, i ,c->table[i], DEREFERENCABLE);
    return LSQ_GetPastRearElement(c);
}

LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
    return create_iter(handle, 0, NULL, PASTREAR);
}

void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
    _iterator *iter = iterator;
    _list_index i;
    if (LSQ_IsIteratorPastRear(iter)) return;
    if (iter->node->next != NULL) {
        iter->node = iter->node->next;
        return;
    }
    for (i = iter->index + 1; i < LIST_COUNT; i++)
        if (iter->con->table[i] != NULL) {
            iter->node = iter->con->table[i];
            iter->index = i;
            return;
        }
    iter->index = 0;
    iter->node = NULL;
    iter->state = PASTREAR;
}

LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_KeyT key) {
    _container *con = handle;
    _list_index ind = calc_hash(con, key);
    _node *node = con->table[ind];

    while (node != NULL && con->keyCompFunc(node->key, key) != 0) node = node->next;
    if (node == NULL) return LSQ_GetPastRearElement(handle);
    return create_iter(con, ind, node, DEREFERENCABLE);
}

void LSQ_InsertElement(LSQ_HandleT handle, LSQ_KeyT key, LSQ_BaseTypeT value) {
    _container *con = handle;
    _iterator *iter = LSQ_GetElementByIndex(con, key);
    _list_index ind;

    if (LSQ_IsIteratorPastRear(iter)) {
        ind = calc_hash(con, key);
        con->table[ind] = create_node(con->keyCloneFunc(key), con->valCloneFunc(value), con->table[ind]);
        con->size++;
    } else {
        free(iter->node->value);
        iter->node->value = con->valCloneFunc(value);
    }
}

void LSQ_DeleteElement(LSQ_HandleT handle, LSQ_KeyT key) {
    _container *con = handle;
    _list_index ind = calc_hash(con, key);
    _node *node = con->table[ind];
    _node *pnode = NULL;

    while (node != NULL && con->keyCompFunc(node->key, key) != 0) {
        pnode = node;
        node = node->next;
    }

    if (node == NULL) return;

    if (pnode == NULL)
        con->table[ind] = node->next;
    else
        pnode->next = node->next;
    free_node(node);
    con->size--;
}