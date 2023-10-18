#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "linear_sequence.h"

typedef struct LSQ_Node {
    LSQ_BaseTypeT value;
    struct LSQ_Node * next_node;
    struct LSQ_Node * previous_node;
} T_LSQ_Node;

typedef struct {
    LSQ_IntegerIndexT size;
    struct LSQ_Node * first_node;
    struct LSQ_Node * last_node;
} T_LSQ_Container;

typedef struct {
    T_LSQ_Container * container;
    struct LSQ_Node * current_node;
} T_LSQ_Iterator;

LSQ_HandleT LSQ_CreateSequence(void) {
    T_LSQ_Container * ans = malloc(sizeof(T_LSQ_Container));
    ans->first_node = malloc(sizeof(T_LSQ_Node));
    ans->last_node = malloc(sizeof(T_LSQ_Node));

    ans->size = 0;

    ans->first_node->previous_node = LSQ_HandleInvalid;
    ans->first_node->next_node = ans->last_node;
    ans->first_node->value = 0;

    ans->last_node->previous_node = ans->first_node;
    ans->last_node->next_node = LSQ_HandleInvalid;
    ans->last_node->value = 0;

    return ans;
}

void LSQ_DestroySequence(LSQ_HandleT handle) {
    T_LSQ_Container * cont = handle;
    int i;

    for (i = cont->size; i > 0; i--) {
        LSQ_DeleteRearElement(cont);
    }

    free(cont->last_node);
    free(cont->first_node);
    free(cont);
}

LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle) {
    return (((T_LSQ_Container *)handle)->size);
}

int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
    return !(LSQ_IsIteratorPastRear(iterator) || LSQ_IsIteratorBeforeFirst(iterator));
}

int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;
    return iter->current_node == ((T_LSQ_Container *)iter->container)->last_node;
}

int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;
    return iter->current_node == ((T_LSQ_Container *)iter->container)->first_node;
}

LSQ_BaseTypeT * LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
    return &((T_LSQ_Iterator *)iterator)->current_node->value;
}

LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index) {
    T_LSQ_Iterator * ans = malloc(sizeof(T_LSQ_Iterator));

    ans->container = handle;
    LSQ_SetPosition(ans, index);

    return ans;
}

LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
    T_LSQ_Iterator * ans = malloc(sizeof(T_LSQ_Iterator));

    ans->container = handle;
    ans->current_node = ((T_LSQ_Container *)handle)->first_node->next_node;

    return ans;
}

LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
    T_LSQ_Iterator * ans = malloc(sizeof(T_LSQ_Iterator));

    ans->container = handle;
    ans->current_node = ((T_LSQ_Container *)handle)->last_node;

    return ans;
}

void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
    free(iterator);
}

void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;

    if (LSQ_IsIteratorPastRear(iter)) return;
    iter->current_node = iter->current_node->next_node;
}

void LSQ_RewindOneElement(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;

    if (LSQ_IsIteratorBeforeFirst(iterator)) return;

    iter->current_node = iter->current_node->previous_node;
}

void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift) {
    LSQ_IntegerIndexT i;

    for (i = 0; i < shift; i++)
        LSQ_AdvanceOneElement(iterator);

    for (i = 0; i > shift; i--)
        LSQ_RewindOneElement(iterator);
}

void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos) {
    T_LSQ_Iterator * iter = iterator;

    iter->current_node = ((T_LSQ_Container *)iter->container)->first_node->next_node;
    LSQ_ShiftPosition(iterator, pos);
}

void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    T_LSQ_Container * cont = handle;
    T_LSQ_Node * new_node = malloc(sizeof(T_LSQ_Node));

    new_node->next_node = cont->first_node->next_node;
    new_node->previous_node = cont->first_node;

    new_node->next_node->previous_node = new_node;
    new_node->previous_node->next_node = new_node;
    new_node->value = element;

    cont->size++;
}

void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    T_LSQ_Container * cont = handle;
    T_LSQ_Node * new_node = malloc(sizeof(T_LSQ_Node));

    new_node->next_node = cont->last_node;
    new_node->previous_node = cont->last_node->previous_node;

    new_node->next_node->previous_node = new_node;
    new_node->previous_node->next_node = new_node;
    new_node->value = element;

    cont->size++;
}

void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement) {
    T_LSQ_Iterator * iter = iterator;

    if (LSQ_IsIteratorBeforeFirst(iter)) return;

    T_LSQ_Node * new_node = malloc(sizeof(T_LSQ_Node));

    new_node->next_node = iter->current_node;
    new_node->previous_node = iter->current_node->previous_node;

    new_node->next_node->previous_node = new_node;
    new_node->previous_node->next_node = new_node;
    new_node->value = newElement;

    iter->container->size++;

    LSQ_RewindOneElement(iterator);
}

void LSQ_DeleteFrontElement(LSQ_HandleT handle) {
    T_LSQ_Container * cont = handle;

    if (cont->first_node->next_node == cont->last_node) return;

    cont->first_node->next_node = cont->first_node->next_node->next_node;
    free(cont->first_node->next_node->previous_node);
    cont->first_node->next_node->previous_node = cont->first_node;

    cont->size--;
}

void LSQ_DeleteRearElement(LSQ_HandleT handle) {
    T_LSQ_Container * cont = handle;

    if (cont->last_node->previous_node == cont->first_node) return;

    cont->last_node->previous_node = cont->last_node->previous_node->previous_node;
    free(cont->last_node->previous_node->next_node);
    cont->last_node->previous_node->next_node = cont->last_node;

    cont->size--;
}

void LSQ_DeleteGivenElement(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;
    T_LSQ_Node * node = iter->current_node;

    if (LSQ_IsIteratorPastRear(iter) || iter->container->size == 0) return;

    node->next_node->previous_node = node->previous_node;
    node->previous_node->next_node = node->next_node;

    LSQ_AdvanceOneElement(iterator);

    iter->container->size--;

    free(node);
}