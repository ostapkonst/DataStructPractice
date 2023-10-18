#include "linear_sequence_assoc.h"

typedef enum {
    DEREFERENCABLE,
    BEFOREFIRST,
    PASTREAR,
} _iter_state;

typedef struct node_s {
    int height;
    LSQ_IntegerIndexT key;
    LSQ_BaseTypeT val;
    struct node_s * left;
    struct node_s * right;
    struct node_s * parent;
} _node;

typedef struct {
    int size;
    _node * root;
} _container;

typedef struct {
    _container * con;
    _node * node;
    _iter_state state;
} _iterator;

/*
void preorder(_node * n) {
    if (n == NULL) return;
    printf("%d ", n->key);
    preorder(n->left);
    preorder(n->right);
}

void inorder(_node * n) {
    if (n == NULL) return;
    inorder(n->left);
    printf("%d ", n->key);
    inorder(n->right);
}

void postorder(_node * n) {
    if (n == NULL) return;
    postorder(n->left);
    postorder(n->right);
    printf("%d ", n->key);
}
*/

void free_node(_node * n) {
    if (n == NULL) return;
    free_node(n->left);
    free_node(n->right);
    free(n);
}

void free_tree(_container * c) {
    free_node(c->root);
}

_node * find_node(_node * n, LSQ_IntegerIndexT k) {
    _node * tmp = n;
    while (tmp != NULL)
        if (k < tmp->key)
            tmp = tmp->left;
        else if (k > tmp->key)
            tmp = tmp->right;
        else
            return tmp;
    return NULL;
}

_node * find_node_in_tree(_container * c, LSQ_IntegerIndexT k) {
    return find_node(c->root, k);
}

_iter_state get_state(_node * n, _iter_state s) {
    return n == NULL ? s : DEREFERENCABLE;
}

int MAX(int a, int b) {
    return a > b ? a : b;
}

_iterator * create_iter(_container * c, _node * n, _iter_state s) {
    _iterator * i =  malloc(sizeof(_iterator));
    i->con = c;
    i->node = n;
    i->state = s;
    return i;
}

_node  * find_node_by_con(_container * c, LSQ_IntegerIndexT k) {
    return find_node(c->root, k);
}

_node * find_right_most_child(_node * n) {
    if (n == NULL) return n;
    while (n->right != NULL) n = n->right;
    return n;
}

_node * find_right_most_child_by_con(_container * c) {
    return find_right_most_child(c->root);
}

_node * find_left_most_child(_node * n) {
    if (n == NULL) return NULL;
    while (n->left != NULL) n = n->left;
    return n;
}

_node * find_left_most_child_by_con(_container * c) {
    return find_left_most_child(c->root);
}

_node * find_left_ancestor(_node * n) {
    _node * pnode = n;
    n = n->parent;
    if (n == NULL) return NULL;
    while (n->parent != NULL && n->right != pnode) {
        pnode = n;
        n = n->parent;
    }
    return (n->right == pnode) ? n : NULL;
}

_node * find_right_ancestor(_node * n) {
    _node * pnode = n;
    n = n->parent;
    if (n == NULL) return NULL;
    while (n->parent != NULL && n->left != pnode) {
        pnode = n;
        n = n->parent;
    }
    return (n->left == pnode) ? n : NULL;
}

_node * create_node(LSQ_IntegerIndexT k, LSQ_BaseTypeT v, _node * p) {
    _node * n = malloc(sizeof(_node));
    n->key = k;
    n->val = v;
    n->height = 1;
    n->parent = p;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void replace_node(_container * c, _node * d, _node * s) {
    if (s != NULL) s->parent = d->parent;
    if (d->parent == NULL) c->root = s;
    else if (d->parent->left == d) d->parent->left = s;
    else if (d->parent->right == d) d->parent->right = s;
}

int height(_node * n) {
    return n == NULL ? 0 : n->height;
}

void update_height(_node * n) {
    n->height = MAX(height(n->left), height(n->right)) + 1;
}

int balance_factor(_node * n) {
    return height(n->left) - height(n->right);
}

void rotate_left(_container * c, _node * n) {
    _node * tmp = n->right->left;
    replace_node(c, n, n->right);
    n->parent = n->right;
    if (n->right->left != NULL)
        n->right->left->parent = n;
    n->right->left = n;
    n->right = tmp;
}

void rotate_right(_container * c, _node * n) {
    _node * tmp = n->left->right;
    replace_node(c, n, n->left);
    n->parent = n->left;
    if (n->left->right != NULL)
        n->left->right->parent = n;
    n->left->right = n;
    n->left = tmp;
}

 void rebalance_tree(_container *cont, _node *root) {
    int bfactor = balance_factor(root);

    if (bfactor == -2) {
        if (balance_factor(root->right) > 0)
            rotate_right(cont, root->right);
        rotate_left(cont, root);
    } else if (bfactor == 2) {
        if (balance_factor(root->left) < 0)
            rotate_left(cont, root->left);
        rotate_right(cont, root);
    }
}

LSQ_HandleT LSQ_CreateSequence(void) {
    _container * c = malloc(sizeof(_container));
    c->size = 0;
    c->root = NULL;
    return c;
}

void LSQ_DestroySequence(LSQ_HandleT handle) {
    free_tree(handle);
    free(handle);
}

LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle) {
    return ((_container *)handle)->size;
}

int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->state == DEREFERENCABLE;
}

int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->state == PASTREAR;
}

int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->state == BEFOREFIRST;
}

LSQ_BaseTypeT * LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
    return &((_iterator *)iterator)->node->val;
}

LSQ_IntegerIndexT LSQ_GetIteratorKey(LSQ_IteratorT iterator) {
    return ((_iterator *)iterator)->node->key;
}

LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index) {
    _node * n = find_node_by_con(handle, index);
    return create_iter(handle, n, get_state(n, PASTREAR));
}

LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
    _container * c = handle;
    _node * n = find_left_most_child_by_con(c);
    return create_iter(c, n, get_state(n, PASTREAR));
}

LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
    return create_iter(handle, NULL, PASTREAR);
}

void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
    free(iterator);
}

void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
    _iterator * iter = iterator;
    _node * tmp;
    if (iter->state == PASTREAR) return;
    if (iter->state == BEFOREFIRST) {
        iter->node = find_left_most_child_by_con(iter->con);
        iter->state = get_state(iter->node, PASTREAR);
        return;
    }
    tmp = find_left_most_child(iter->node->right);
    if (tmp == NULL)
        tmp = find_right_ancestor(iter->node);
    iter->node = tmp;
    iter->state = get_state(iter->node, PASTREAR);
}

void LSQ_RewindOneElement(LSQ_IteratorT iterator) {
    _iterator * iter = iterator;
    _node * tmp;
    if (iter->state == BEFOREFIRST) return;
    if (iter->state == PASTREAR) {
        iter->node = find_right_most_child_by_con(iter->con);
        iter->state = get_state(iter->node, BEFOREFIRST);
        return;
    }
    tmp = find_right_most_child(iter->node->left);
    if (tmp == NULL)
        tmp = find_left_ancestor(iter->node);
    iter->node = tmp;
    iter->state = get_state(iter->node, BEFOREFIRST);
}

void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift) {
    LSQ_IntegerIndexT i;

    for (i = 0; i < shift; i++)
        LSQ_AdvanceOneElement(iterator);

    for (i = 0; i > shift; i--)
        LSQ_RewindOneElement(iterator);
}

void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos) {
    ((_iterator *) iterator)->state = BEFOREFIRST;
    LSQ_ShiftPosition(iterator, pos + 1);
}

void run_balance_insert(_container * c, _node * pn) {
    _node * n;
    do {
        n = pn;
        pn = pn->parent;
        update_height(n);
        rebalance_tree(c, n);
    } while (pn != NULL && balance_factor(n));

}

void LSQ_InsertElement(LSQ_HandleT handle, LSQ_IntegerIndexT key, LSQ_BaseTypeT value) {
    _container * con = handle;
    _node * node;
    _node * newNode;
    _node * pnode;
    if (con->root == NULL) {
        con->root = create_node(key, value, NULL);
        con->size++;
        return;
    }
    node = con->root;

    for (;;)
        if (key < node->key) {
            if (node->left == NULL) break;
            node = node->left;
        } else if (key > node->key) {
            if (node->right == NULL) break;
            node = node->right;
        } else {
            node->val = value;
            return;
        }

    newNode = create_node(key, value, node);
    if (key < node->key)
        node->left = newNode;
    else
        node->right = newNode;
    con->size++;
    pnode = node;
    run_balance_insert(con, pnode);

}

void LSQ_DeleteFrontElement(LSQ_HandleT handle) {
    LSQ_IteratorT iterator = LSQ_GetFrontElement(handle);
    LSQ_DeleteElement(handle, LSQ_GetIteratorKey(iterator));
    LSQ_DestroyIterator(iterator);
}

void LSQ_DeleteRearElement(LSQ_HandleT handle) {
    LSQ_IteratorT iterator = LSQ_GetPastRearElement(handle);
    LSQ_RewindOneElement(iterator);
    LSQ_DeleteElement(handle, LSQ_GetIteratorKey(iterator));
    LSQ_DestroyIterator(iterator);
}

void run_balance_delete(_container * c, _node * pn) {
    _node * n;
    int bfactor;
        while (pn != NULL) {
        n = pn;
        pn = pn->parent;
        bfactor = balance_factor(n);
        if (bfactor == -1 || bfactor == 1) break;
        rebalance_tree(c, n);
    }
}

void LSQ_DeleteElement(LSQ_HandleT handle, LSQ_IntegerIndexT key) {
    _container * con = handle;
    _node * node = find_node_by_con(con, key);
    _node * tmp;
    _node * pnode;
    LSQ_IntegerIndexT tmpKey;
    if (node == NULL) return;
    if (node->left == NULL && node->right == NULL) replace_node(con, node, NULL);
    else if (node->left == NULL)
        replace_node(con, node, node->right);
    else if (node->right == NULL)
        replace_node(con, node, node->left);
    else {
         tmp = find_right_most_child(node->left);
        if (tmp == NULL)
            tmp = find_left_most_child(node->right);
        tmpKey = tmp->key;
        node->val = tmp->val;
        LSQ_DeleteElement(handle, tmpKey);
        node->key = tmpKey;
        return;
    }
    pnode = node->parent;
    free(node);
    con->size--;
    run_balance_delete(con, pnode);
}