#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "linear_sequence.h"
#define debug

int PrintLSQ(LSQ_HandleT a){
    LSQ_IteratorT iter = LSQ_GetFrontElement(a);
    int t = 0;
    while (LSQ_IsIteratorDereferencable(iter)){
#ifdef debug
        if (t) printf(", ");
        printf("%d", *LSQ_DereferenceIterator(iter));
#endif
        LSQ_AdvanceOneElement(iter);
        t++;
    }
#ifdef debug
    printf("\nPrinted = %d\n", t);
    printf("LSQ size = %d\n", LSQ_GetSize(a));
#endif
    LSQ_DestroyIterator(iter);
    return t;
}

int test1(){
    int i;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (i = 1; i <= 20; i++) {
        LSQ_InsertRearElement(a, i);
    }
    int r = PrintLSQ(a) == 20 && LSQ_GetSize(a) == 20;
    LSQ_DestroySequence(a);
    return r;
}

int test2(){
    int i;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (i = 1; i <= 200; i++) {
        LSQ_InsertRearElement(a, i);
        LSQ_InsertFrontElement(a, i);
    }
    for (i = 1; i <= 100; i++) {
        LSQ_DeleteFrontElement(a);
        LSQ_DeleteRearElement(a);
    }
    PrintLSQ(a);
    int r = 1;
    LSQ_IteratorT iter = LSQ_GetFrontElement(a);
    for (i = -100; i <= 100; i++) {
        if (i == 0) continue;
        r = r && abs(i) == *LSQ_DereferenceIterator(iter);
        LSQ_AdvanceOneElement(iter);
    }
    r = r && PrintLSQ(a) == 200;
    r = r && LSQ_GetSize(a) == 200;
    LSQ_DestroyIterator(iter);
    LSQ_DestroySequence(a);
    return r;
}

int test3(){
    int i;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (i = 1; i <= 200; i++) {
        LSQ_InsertRearElement(a, i);
    }
    LSQ_IteratorT iter = LSQ_GetPastRearElement(a);
    int k = 20, r = LSQ_IsIteratorPastRear(iter);
    LSQ_ShiftPosition(iter, -30);
    LSQ_ShiftPosition(iter, 10);
    while (!LSQ_IsIteratorBeforeFirst(iter)) {
        LSQ_RewindOneElement(iter);
        k += 1;
    }
    LSQ_AdvanceOneElement(iter); //переход к первому элементу
    r = k && k == 200 + 1;
    for (i = 1; i <= 200; i++){
        r = r && i == *LSQ_DereferenceIterator(iter);
        LSQ_AdvanceOneElement(iter);
    }
    r = r && LSQ_IsIteratorPastRear(iter);
    r = r && PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == 200;
    LSQ_DestroyIterator(iter);
    LSQ_DestroySequence(a);
    return r;
}

int test4(){
    int i;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (i = 1; i <= 100; i++) {
        LSQ_InsertRearElement(a, i);
    }
    LSQ_IteratorT iter = LSQ_GetElementByIndex(a, 50); //значение: 51
    printf("%d \n",LSQ_DereferenceIterator(LSQ_GetElementByIndex(a, 50)));
    for (i = 0; i < 50; i++) {
        LSQ_InsertElementBeforeGiven(iter, i);
    }
    LSQ_SetPosition(iter, 60); //значение: 39
    //printf("%d",LSQ_GetElementByIndex(a, 60));
    PrintLSQ(a);
    for (i = 0; i < 10; i++) {
        LSQ_DeleteGivenElement(iter);
    }
    LSQ_SetPosition(iter, 0);
    int r = 1;
    for (i = 1; i <= 50; i++) {
        r = r && i == *LSQ_DereferenceIterator(iter);
        LSQ_AdvanceOneElement(iter);
    }
    for (i = 49; i >= 0; i--) {
        if (i <= 39 && i > 29) continue;
        r = r && i == *LSQ_DereferenceIterator(iter);
        LSQ_AdvanceOneElement(iter);
    }
    for (i = 51; i <= 100; i++) {
        r = r && i == *LSQ_DereferenceIterator(iter);
        LSQ_AdvanceOneElement(iter);
    }
    r = r && LSQ_IsIteratorPastRear(iter);
    r = r && PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == 140;
    LSQ_DestroyIterator(iter);
    LSQ_DestroySequence(a);
    return r;
}

int test5(){
    int i;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (i = 1; i <= 100; i++) {
        LSQ_InsertRearElement(a, i);
    }
    LSQ_IteratorT iter = LSQ_GetFrontElement(a);
    LSQ_SetPosition(iter, 50);
    LSQ_IteratorT iter2 = LSQ_GetElementByIndex(a, 50);
    int r = LSQ_DereferenceIterator(iter) == LSQ_DereferenceIterator(iter2);
    //printf("%d - %d", *LSQ_DereferenceIterator(iter), *LSQ_DereferenceIterator(iter2));
    LSQ_DestroyIterator(iter);
    LSQ_DestroyIterator(iter2);
    LSQ_DestroySequence(a);
    return r;
}

int main(){
    printf(test1()?"OK 1\n":"WA 1\n");
    printf(test2()?"OK 2\n":"WA 2\n");
    printf(test3()?"OK 3\n":"WA 3\n");
    printf(test4()?"OK 4\n":"WA 4\n");
    printf(test5()?"OK 5\n":"WA 5\n");

    return 0;
}
