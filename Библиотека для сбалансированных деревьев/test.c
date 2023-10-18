#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "linear_sequence_assoc.h"
//#define debug

int PrintLSQ(LSQ_HandleT a){
    LSQ_IteratorT iter = LSQ_GetPastRearElement(a);
    LSQ_RewindOneElement(iter);
    int t = 0;
    while (LSQ_IsIteratorDereferencable(iter)){
#ifdef debug
        printf("PrintLSQ: key: %d", LSQ_GetIteratorKey(iter));
        printf("PrintLSQ: key: %d, value: %d\n", LSQ_GetIteratorKey(iter), *LSQ_DereferenceIterator(iter));
#endif
        LSQ_RewindOneElement(iter);
        t++;
    }
#ifdef debug
    printf("\nPrinted = %d\n", t);
    printf("LSQ size = %d\n", LSQ_GetSize(a));
#endif
    LSQ_DestroyIterator(iter);
    return t;
}

int test0(){
    LSQ_HandleT a = LSQ_CreateSequence();
    LSQ_InsertElement(a, 1, 162342);
    LSQ_InsertElement(a, 3, 16234);
    LSQ_InsertElement(a, 2, 162);
    int r = PrintLSQ(a) == 3 && LSQ_GetSize(a) == 3;
    LSQ_DestroySequence(a);
    return r;
}

int test1(){
    LSQ_HandleT a = LSQ_CreateSequence();
    LSQ_InsertElement(a, 5, 162342);
    LSQ_InsertElement(a, 10, 8712);
    LSQ_InsertElement(a, 3, 8712);
    LSQ_InsertElement(a, 4, 8712);
    LSQ_InsertElement(a, 2, 8213122);
    LSQ_InsertElement(a, 7, 8712);
    LSQ_InsertElement(a, 8, 8712);
    LSQ_InsertElement(a, 6, 8712);
    LSQ_InsertElement(a, 12, 8712);
    int i;
    int r = PrintLSQ(a) == 9 && LSQ_GetSize(a) == 9;
    LSQ_IteratorT iter = LSQ_GetPastRearElement(a);
    LSQ_RewindOneElement(iter);
    while(LSQ_IsIteratorDereferencable(iter)){
        i = LSQ_GetIteratorKey(iter);
        LSQ_RewindOneElement(iter);
        LSQ_DeleteElement(a, i);
    }
    LSQ_DestroySequence(a);
    return r;
}

int test2(){
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 0; i <= 199; i++) {
        LSQ_InsertElement(a, i-200, rand()%50);
        LSQ_InsertElement(a, i+1, rand()%50);
    }
    for (int i = 1; i <= 100; i++){
        int b = PrintLSQ(a);
        LSQ_DeleteFrontElement(a);
        LSQ_DeleteRearElement(a);
    }
    int r = 1;
    LSQ_IteratorT iter = LSQ_GetFrontElement(a);
    for (int i = -100; i <= 100; i++) {
        if (i == 0) continue;
        r = r && i == LSQ_GetIteratorKey(iter);
        LSQ_AdvanceOneElement(iter);
    }
    r = r && PrintLSQ(a) == 200;
    r = r && LSQ_GetSize(a) == 200;
    LSQ_DestroyIterator(iter);
    LSQ_DestroySequence(a);
    return r;
}

int test3(){
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 1; i <= 200; i++) {
        LSQ_InsertElement(a, i, i);

    }
    LSQ_IteratorT iter = LSQ_GetPastRearElement(a);
    int k = 20, r = LSQ_IsIteratorPastRear(iter);
    LSQ_ShiftPosition(iter, -30);
    LSQ_ShiftPosition(iter, 10);
    while (!LSQ_IsIteratorBeforeFirst(iter)){
        LSQ_RewindOneElement(iter);
        k += 1;
    }
    LSQ_AdvanceOneElement(iter); //переход к первому элементу
    r = k && k == 200 + 1;
    for (int i = 1; i <= 200; i++){
        r = r && i == *LSQ_DereferenceIterator(iter) && i == LSQ_GetIteratorKey(iter);
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
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 1; i <= 100; i++) {
        LSQ_InsertElement(a, i, i);
    }
    LSQ_IteratorT iter = LSQ_GetFrontElement(a);
    LSQ_SetPosition(iter, 50);
    LSQ_IteratorT iter2 = LSQ_GetElementByIndex(a, 51);
    int r = LSQ_GetIteratorKey(iter) == LSQ_GetIteratorKey(iter2);
    r = r && LSQ_DereferenceIterator(iter) == LSQ_DereferenceIterator(iter2);
    printf("%d - %d\n", *LSQ_DereferenceIterator(iter), *LSQ_DereferenceIterator(iter2)); //значение - 51
    LSQ_DestroyIterator(iter);
    LSQ_DestroyIterator(iter2);
    iter = LSQ_GetElementByIndex(a, 500);
    r = r && LSQ_IsIteratorPastRear(iter);
    LSQ_DestroyIterator(iter);
    LSQ_DestroySequence(a);
    return r;
}

int test5(){
    int b[1000];
    for (int i = 0; i < 1000; i++) b[i] = 0;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 0; i < 10; i++){
        int j = rand()%1000;
        if (b[j]) continue;
        LSQ_InsertElement(a, j, rand());
        printf("j %d\n", j);
        b[j] = 1;
    }
    for (int i = 0; i < rand(); i++){
        int j = rand()%1000;
        if (!b[j]) continue;
        printf("delete %d\n", j);
        LSQ_DeleteElement(a, j);
        b[j] = 0;
    }
    int elemCount = 0;
    for (int i = 0; i < 1000; i++) {
        if (b[i]) elemCount++;
    }
    int r = PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == elemCount;
    LSQ_DestroySequence(a);
    return r;
}

int test6(){
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 0; i < 3000; i++){
        LSQ_InsertElement(a, i, rand());
    }
    int r = PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == 3000;

    //int *b = calloc(3000, sizeof(int));
    int b[3000];
    for (int i = 0; i < 3000; i++) b[i] = 0;
    for (int i = 0; i < 3000; i++){
        int j = rand()%3000;
        if (b[j]) continue;
        b[j] = 1;
        LSQ_DeleteElement(a, j);
    }
    int elemCount = 0;
    for (int i = 0; i < 3000; i++) {
        if (!b[i]) elemCount++;
    }
    r = r && PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == elemCount;
    LSQ_DestroySequence(a);
    return r;
}

int test7(){
    LSQ_HandleT a = LSQ_CreateSequence();
    LSQ_IteratorT iter = LSQ_GetPastRearElement(a);
    int r = !LSQ_IsIteratorDereferencable(iter);
    r = r && LSQ_IsIteratorPastRear(iter);
    LSQ_RewindOneElement(iter);
    r = r && LSQ_IsIteratorBeforeFirst(iter);
    r = r && !LSQ_IsIteratorDereferencable(iter);
    LSQ_DestroyIterator(iter);
    r = r && PrintLSQ(a) == 0 && LSQ_GetSize(a) == 0;
    LSQ_DestroySequence(a);

    a = LSQ_CreateSequence();
    iter = LSQ_GetFrontElement(a);
    r = r && !LSQ_IsIteratorDereferencable(iter);
    r = r && LSQ_IsIteratorPastRear(iter);
    LSQ_RewindOneElement(iter);
    r = r && LSQ_IsIteratorBeforeFirst(iter);
    r = r && !LSQ_IsIteratorDereferencable(iter);
    LSQ_DestroyIterator(iter);
    r = r && PrintLSQ(a) == 0 && LSQ_GetSize(a) == 0;
    LSQ_DestroySequence(a);
    return r;
}

int test8(){ //проверка переходов внутри дерева
    int b[10000];
    for (int i = 0; i < 10000; i++) b[i] = 0;
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 0; i < rand(); i++){
        int j = rand()%10000;
        //if (b[j]) continue;
        LSQ_InsertElement(a, j, j+5);
        b[j] = 1;
        printf("%d\n", i);
    }
    int r = 1;
    printf("%s\n", "lasas1");
    LSQ_IteratorT iter = LSQ_GetPastRearElement(a);
    printf("%s\n", "lasas2");
    for (int j = 9999; j >= 0; j--){
        if (!b[j]) continue;
        LSQ_RewindOneElement(iter);
        r = r && j == LSQ_GetIteratorKey(iter) && j == *LSQ_DereferenceIterator(iter)-5;
        //printf("%d - %d\n", j, LSQ_GetIteratorKey(iter));
    }
    LSQ_RewindOneElement(iter);
    printf("%s\n", "lasas6");
    r = r && LSQ_IsIteratorBeforeFirst(iter) && !LSQ_IsIteratorDereferencable(iter);
    printf("%s\n", "lasas7");
    LSQ_DestroyIterator(iter);
    printf("%s\n", "lasas8");

    iter = LSQ_GetFrontElement(a);
    for (int j = 0; j < 10000; j++){
        if (!b[j]) continue;
        r = r && j == LSQ_GetIteratorKey(iter) && j == *LSQ_DereferenceIterator(iter)-5;
        LSQ_AdvanceOneElement(iter);
        //printf("%d - %d\n", j, LSQ_GetIteratorKey(iter));
    }
    r = r && LSQ_IsIteratorPastRear(iter) && !LSQ_IsIteratorDereferencable(iter);
    LSQ_DestroyIterator(iter);

    int elemCount = 0;
    for (int i = 0; i < 10000; i++) {
        if (b[i]) elemCount++;
    }
    r = r && PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == elemCount;
    LSQ_DestroySequence(a);
    return r;
}

int test9(){
    LSQ_HandleT a = LSQ_CreateSequence();
    for (int i = 0; i < 100; i++){
        LSQ_InsertElement(a, i, i-1);
    }
    int r = 1;
    LSQ_IteratorT iter = LSQ_GetFrontElement(a); //0
    r = r && 0 == LSQ_GetIteratorKey(iter) && 0 == *LSQ_DereferenceIterator(iter)+1;
    LSQ_SetPosition(iter, 10); //10
    r = r && 10 == LSQ_GetIteratorKey(iter) && 10 == *LSQ_DereferenceIterator(iter)+1;
    LSQ_RewindOneElement(iter); //9
    r = r && 9 == LSQ_GetIteratorKey(iter) && 9 == *LSQ_DereferenceIterator(iter)+1;
    LSQ_ShiftPosition(iter, 11); //20
    r = r && 20 == LSQ_GetIteratorKey(iter) && 20 == *LSQ_DereferenceIterator(iter)+1;
    LSQ_ShiftPosition(iter, -10); //10
    r = r && 10 == LSQ_GetIteratorKey(iter) && 10 == *LSQ_DereferenceIterator(iter)+1;
    LSQ_ShiftPosition(iter, -11); //-1
    r = r && LSQ_IsIteratorBeforeFirst(iter) && !LSQ_IsIteratorDereferencable(iter);
    LSQ_SetPosition(iter, 100); //100
    r = r && LSQ_IsIteratorPastRear(iter) && !LSQ_IsIteratorDereferencable(iter);
    r = r && PrintLSQ(a) == LSQ_GetSize(a);
    r = r && LSQ_GetSize(a) == 100;
    LSQ_DestroySequence(a);
    LSQ_DestroyIterator(iter);
    return r;
}

int main(){
    //printf("%d", time(NULL));
    srand(time(NULL));
//#define memtest
#ifdef memtest
    while(1){
        printf(test1()?"":"WA 1\n");
        printf(test2()?"":"WA 2\n");
        printf(test3()?"":"WA 3\n");
        printf(test4()?"":"WA 4\n");
        printf(test5()?"":"WA 5\n");
        printf(test6()?"":"WA 6\n");
        printf(test7()?"1":"WA 7\n");
        printf(test8()?"":"WA 8\n");
        printf(test9()?"":"WA 9\n");
    }
#endif
    printf(test0()?"OK 0\n":"WA 1\n");
    printf(test1()?"OK 1\n":"WA 1\n");
    printf(test2()?"OK 2\n":"WA 2\n");
    printf(test3()?"OK 3\n":"WA 3\n");
    printf(test4()?"OK 4\n":"WA 4\n");
    printf(test5()?"OK 5\n":"WA 5\n");
    /*printf(test6()?"OK 6\n":"WA 6\n");
    printf(test7()?"OK 7\n":"WA 7\n");
    printf(test8()?"OK 8\n":"WA 8\n");
    printf(test9()?"OK 9\n":"WA 9\n");*/
    return 0;
}
