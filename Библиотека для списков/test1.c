#include "linear_sequence.h"
#include <stdio.h>
#include <time.h>
#define BOOL int
#define TRUE 1
#define FALSE 0
typedef BOOL (*TEST_FuncT)(void);

void PrintContainer(LSQ_IteratorT handle, LSQ_IntegerIndexT direction){
    printf("Container:\n");
    while (!(LSQ_IsIteratorPastRear(handle) && direction == 1) && !(LSQ_IsIteratorBeforeFirst(handle) && direction == -1)){
        if (!LSQ_IsIteratorDereferencable(handle)){
            printf("Can't dereference iterator\n");
            LSQ_ShiftPosition(handle, direction);
            continue;
        }
        printf("%d ", *LSQ_DereferenceIterator(handle));
        LSQ_ShiftPosition(handle, direction);
    }
}

BOOL TEST_CreateDestroyEmptySequence(void){
    printf("TEST: Create - Destroy empty sequence: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    // LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_CreateDestroyIteratorForEmptySequence(void){
    printf("TEST: Create - Destroy iterator for empty sequence: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_FrontOutOfRangeRight(void){
    printf("TEST: Empty sequence, GetFrontElement, trying Advance 2 elements: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    LSQ_AdvanceOneElement(iterator);
    LSQ_AdvanceOneElement(iterator);
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_FrontOutOfRangeLeft(void){
    printf("TEST: Empty sequence, GetFrontElement, trying Rewind 2 elements: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    LSQ_RewindOneElement(iterator);
    LSQ_RewindOneElement(iterator);
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_RearOutOfRangeRight(void){
    printf("TEST: Empty sequence, GetPastRearElement, trying Advance 2 elements: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetPastRearElement(container);
    LSQ_AdvanceOneElement(iterator);
    LSQ_AdvanceOneElement(iterator);
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_RearOutOfRangeLeft(void){
    printf("TEST: Empty sequence, GetPastRearElement, trying Rewind 2 elements: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetPastRearElement(container);
    LSQ_RewindOneElement(iterator);
    LSQ_RewindOneElement(iterator);
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_Edges(void){
    printf("TEST: IsIteratorPastRear, IsIteratorBeforeFirst: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetPastRearElement(container);
    LSQ_AdvanceOneElement(iterator);
    if (!LSQ_IsIteratorPastRear(iterator)){
        LSQ_DestroyIterator(iterator);
        LSQ_DestroySequence(container);
        return FALSE;
    }
    LSQ_RewindOneElement(iterator);
    LSQ_RewindOneElement(iterator);
    LSQ_RewindOneElement(iterator);
    if (!LSQ_IsIteratorBeforeFirst(iterator)){
        LSQ_DestroyIterator(iterator);
        LSQ_DestroySequence(container);
        return FALSE;
    }
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_IsIteratorDereferencableEdges(void){
    printf("TEST: IsIteratorDereferencable: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    if (LSQ_IsIteratorDereferencable(iterator)){
        LSQ_DestroyIterator(iterator);
        LSQ_DestroySequence(container);
        return FALSE;
    }
    LSQ_RewindOneElement(iterator);
    if (LSQ_IsIteratorDereferencable(iterator)){
        LSQ_DestroyIterator(iterator);
        LSQ_DestroySequence(container);
        return FALSE;
    }
    LSQ_AdvanceOneElement(iterator);
    LSQ_AdvanceOneElement(iterator);
    if (LSQ_IsIteratorDereferencable(iterator)){
        LSQ_DestroyIterator(iterator);
        LSQ_DestroySequence(container);
        return FALSE;
    }
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_InsertRearSequence(void){
    printf("TEST: Fill sequence using LSQ_InsertRearElement: ");
    srand(time(NULL));
    int i, n = 10;
    int *data;
    LSQ_HandleT container = LSQ_CreateSequence();
    data = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++){
        data[i] = rand() % 100;
        LSQ_InsertRearElement(container, data[i]);
    }
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    i = 0;
    BOOL has_error = FALSE;
    while (i < n){
        if (!LSQ_IsIteratorDereferencable(iterator))
            has_error = TRUE;
        else if (*LSQ_DereferenceIterator(iterator) != data[i])
            has_error = TRUE;
        if (has_error){
            LSQ_DestroyIterator(iterator);
            LSQ_DestroySequence(container);
            return FALSE;
        }
        else{
            i++;
            LSQ_AdvanceOneElement(iterator);
        }
    }
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_InsertFrontSequence(void){
    printf("TEST: Fill sequence using LSQ_InsertFrontElement: ");
    srand(time(NULL));
    int i, n = 10;
    int *data;
    LSQ_HandleT container = LSQ_CreateSequence();
    data = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++){
        data[n - i - 1] = rand() % 100;
        LSQ_InsertFrontElement(container, data[n - i - 1]);
    }
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    i = 0;
    BOOL has_error = FALSE;
    while (i < n){
        if (!LSQ_IsIteratorDereferencable(iterator))
            has_error = TRUE;
        else if (*LSQ_DereferenceIterator(iterator) != data[i])
            has_error = TRUE;
        if (has_error){
            LSQ_DestroyIterator(iterator);
            LSQ_DestroySequence(container);
            return FALSE;
        }
        else{
            i++;
            LSQ_AdvanceOneElement(iterator);
        }
    }
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_GetSize(void){
    printf("TEST: Get size: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    srand(time(NULL));
    int n = rand() % 100;
    int i;
    for (i = 0; i < n; i++)
        LSQ_InsertFrontElement(container, 1);
    int tmp_n = LSQ_GetSize(container);
    LSQ_DestroySequence(container);
    return tmp_n == n;
}

BOOL TEST_TwoItersForOneSequence(void){
    printf("TEST: Creating and destroying two iters step by step: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator1 = LSQ_GetFrontElement(container);
    LSQ_DestroyIterator(iterator1);
    LSQ_IteratorT iterator2 = LSQ_GetFrontElement(container);
    LSQ_DestroyIterator(iterator2);
    LSQ_DestroySequence(container);
    return TRUE;
}

BOOL TEST_SetPosition(void){
    printf("TEST: Set position, critical and normal using: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    srand(time(NULL));
    int n = rand() % 200 + 5;
    int i;
    for (i = 0; i < n; i++)
        LSQ_InsertRearElement(container, i);
    LSQ_SetPosition(iterator, 5);
    int has_error = FALSE;
    if (*LSQ_DereferenceIterator(iterator) != 5)
        has_error = TRUE;
    LSQ_SetPosition(iterator, -1);
    if (LSQ_IsIteratorDereferencable(iterator))
        has_error = TRUE;
    LSQ_SetPosition(iterator, n + 10);
    if (LSQ_IsIteratorDereferencable(iterator))
        has_error = TRUE;
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return !has_error;
}

BOOL TEST_DeleteGivenElement(void){
    printf("TEST: Destroy all sequence using DeleteGivenElement: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    srand(time(NULL));
    int n = rand() % 15;
    int i;
    BOOL has_error = FALSE;
    for (i = 0; i < n; i++)
        LSQ_InsertFrontElement(container, i);
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    for (i = 0; i < n + 10; i++)
        LSQ_DeleteGivenElement(iterator);
    if (LSQ_IsIteratorDereferencable(iterator))
        has_error = TRUE;
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return !has_error;
}

BOOL TEST_DeleteFrontElement(void){
    printf("TEST: Destroy all sequence using DeleteFrontElement: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    srand(time(NULL));
    int n = rand() % 15;
    int i;
    BOOL has_error = FALSE;
    for (i = 0; i < n; i++)
        LSQ_InsertFrontElement(container, i);
    for (i = 0; i < n + 10; i++)
        LSQ_DeleteFrontElement(container);
    if (LSQ_GetSize(container) != 0)
        has_error = TRUE;
    LSQ_DestroySequence(container);
    return !has_error;
}

BOOL TEST_DeleteRearElement(void){
    printf("TEST: Destroy all sequence using DeleteRearElement: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    srand(time(NULL));
    int n = rand() % 15;
    int i;
    BOOL has_error = FALSE;
    for (i = 0; i < n; i++)
        LSQ_InsertFrontElement(container, i);
    for (i = 0; i < n + 10; i++)
        LSQ_DeleteRearElement(container);
    if (LSQ_GetSize(container) != 0)
        has_error = TRUE;
    LSQ_DestroySequence(container);
    return !has_error;
}

BOOL TEST_InsertBeforeGivenElement(void){
    printf("TEST: Checking insert into pos == 0, pos == n, pos == n / 2: ");
    LSQ_HandleT container = LSQ_CreateSequence();
    LSQ_IteratorT iterator = LSQ_GetFrontElement(container);
    srand(time(NULL));
    int n = rand() % 15;
    int i;
    BOOL has_error = FALSE;
    for (i = 0; i < n; i ++)
        LSQ_InsertRearElement(container, i);
    LSQ_InsertElementBeforeGiven(iterator, n);
    LSQ_RewindOneElement(iterator);
    LSQ_InsertElementBeforeGiven(iterator, n + 1);
    LSQ_SetPosition(iterator, 0);
    LSQ_InsertElementBeforeGiven(iterator, n + 2);
    // PrintContainer(iterator, -1);
    LSQ_DestroyIterator(iterator);
    LSQ_DestroySequence(container);
    return TRUE;
}

int main(){
    int foo_cnt = 16;
    TEST_FuncT funcs[foo_cnt];
    funcs[0] = (TEST_FuncT)TEST_CreateDestroyEmptySequence;
    funcs[1] = (TEST_FuncT)TEST_CreateDestroyIteratorForEmptySequence;
    funcs[2] = (TEST_FuncT)TEST_FrontOutOfRangeRight;
    funcs[3] = (TEST_FuncT)TEST_FrontOutOfRangeLeft;
    funcs[4] = (TEST_FuncT)TEST_RearOutOfRangeRight;
    funcs[5] = (TEST_FuncT)TEST_RearOutOfRangeLeft;
    funcs[6] = (TEST_FuncT)TEST_Edges;
    funcs[7] = (TEST_FuncT)TEST_IsIteratorDereferencableEdges;
    funcs[8] = (TEST_FuncT)TEST_InsertRearSequence;
    funcs[9] = (TEST_FuncT)TEST_InsertFrontSequence;
    funcs[10] = (TEST_FuncT)TEST_GetSize;
    funcs[11] = (TEST_FuncT)TEST_TwoItersForOneSequence;
    funcs[12] = (TEST_FuncT)TEST_SetPosition;
    funcs[13] = (TEST_FuncT)TEST_DeleteGivenElement;
    funcs[14] = (TEST_FuncT)TEST_DeleteFrontElement;
    funcs[15] = (TEST_FuncT)TEST_InsertBeforeGivenElement;
    // printf("%d", funcs[7]());
    int i;
    for (i = 0; i < foo_cnt; i++)
        if (funcs[i]())
            printf("OK\n");
        else
            printf("FAIL\n");
    return 0;
}