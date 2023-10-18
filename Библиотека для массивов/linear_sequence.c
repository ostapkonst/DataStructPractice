#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "linear_sequence.h"
#include <string.h>

typedef struct {
    LSQ_IntegerIndexT capasity; /* Физический размер */
    LSQ_IntegerIndexT size; /* Реальный размер */
    LSQ_BaseTypeT * data; /* Указатель на память */
} T_LSQ_Array;

typedef struct {
    T_LSQ_Array * array; /* Указатель на массив */
    LSQ_IntegerIndexT index; /* Индекс элемента */
} T_LSQ_Iterator;

const int initial_value = 1;
const int factor_value = 2;

void Resize(T_LSQ_Array * arr) {
    arr->data = realloc(&arr->data[0], sizeof(LSQ_BaseTypeT) * arr->capasity);
}

void Resize_when_add(T_LSQ_Array * arr) {
    if (arr->size == arr->capasity) {
        arr->capasity *= factor_value;
        Resize(arr);
    }
}

void Resize_when_delite(T_LSQ_Array * arr) {
    if ((arr->size  == arr->capasity / factor_value) &&
        (initial_value <= arr->size)) {
        arr->capasity /= factor_value;
        Resize(arr);
    }
}

LSQ_HandleT LSQ_CreateSequence(void) {
    T_LSQ_Array * ans = malloc(sizeof(T_LSQ_Array));
    ans->data = malloc(sizeof(LSQ_BaseTypeT) * initial_value);

    ans->capasity = initial_value;
    ans->size = 0;

    return ans;
}

void LSQ_DestroySequence(LSQ_HandleT handle) {
    free(((T_LSQ_Array *)handle)->data);
    free(handle);
}

LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle) {
    return (((T_LSQ_Array *)handle)->size);
}

int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
    return !(LSQ_IsIteratorPastRear(iterator) || LSQ_IsIteratorBeforeFirst(iterator));
}

int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;
    return iter->index == iter->array->size;
}

int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
    return ((T_LSQ_Iterator *)iterator)->index == -1;
}

LSQ_BaseTypeT * LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;
    return &iter->array->data[iter->index];
}

LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index) {
    T_LSQ_Iterator * ans = malloc(sizeof(T_LSQ_Iterator));

    ans->array = handle;
    LSQ_SetPosition(ans, index);

    return ans;
}

LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
    T_LSQ_Iterator * ans = malloc(sizeof(T_LSQ_Iterator));

    ans->array = handle;
    ans->index = 0;

    return ans;
}

LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
    T_LSQ_Iterator * ans = malloc(sizeof(T_LSQ_Iterator));

    ans->array = handle;
    ans->index = ((T_LSQ_Array *)handle)->size;

    return ans;
}

void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
    free(iterator);
}

void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;

    if (LSQ_IsIteratorPastRear(iter)) return;

    iter->index++;
}

void LSQ_RewindOneElement(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;

    if (LSQ_IsIteratorBeforeFirst(iter)) return;

    iter->index--;
}

void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift) {
    LSQ_IntegerIndexT i;

    for (i = 0; i < shift; i++)
        LSQ_AdvanceOneElement(iterator);

    for (i = 0; i > shift; i--)
        LSQ_RewindOneElement(iterator);
}

void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos) {
    ((T_LSQ_Iterator *)iterator)->index = 0;
    LSQ_ShiftPosition(iterator, pos);
}

void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    T_LSQ_Array * arr = handle;

    Resize_when_add(arr);
    memmove(&arr->data[1], &arr->data[0], sizeof(LSQ_BaseTypeT) * arr->size);
    arr->data[0] = element;
    arr->size++;
}

void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    T_LSQ_Array * arr = handle;

    Resize_when_add(arr);
    arr->data[arr->size] = element;
    arr->size++;
}

void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement) {
    T_LSQ_Iterator * iter = iterator;
    T_LSQ_Array * arr = iter->array;

    if (LSQ_IsIteratorBeforeFirst(iter)) return;

    Resize_when_add(arr);
    memmove(&arr->data[iter->index + 1], &arr->data[iter->index], sizeof(LSQ_BaseTypeT) * (arr->size - iter->index));
    arr->data[iter->index] = newElement;
    arr->size++;
}

void LSQ_DeleteFrontElement(LSQ_HandleT handle) {
    T_LSQ_Array * arr = handle;

    if (arr->size == 0) return;

    arr->size--;
    memmove(&arr->data[0], &arr->data[1], sizeof(LSQ_BaseTypeT) * arr->size);
    Resize_when_delite(arr);
}

void LSQ_DeleteRearElement(LSQ_HandleT handle) {
    T_LSQ_Array * arr = handle;

    if (arr->size == 0) return;
    arr->size--;
    Resize_when_delite(arr);
}

void LSQ_DeleteGivenElement(LSQ_IteratorT iterator) {
    T_LSQ_Iterator * iter = iterator;
    T_LSQ_Array * arr = iter->array;

    if (LSQ_IsIteratorPastRear(iter) || arr->size == 0) return;

    iter->array->size--;
    memmove(&arr->data[iter->index], &arr->data[iter->index + 1], sizeof(LSQ_BaseTypeT) * (arr->size - iter->index));
    Resize_when_delite(arr);
}