#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

#define _Debug

void VectorPrint(VECT_Handle v1) {
    int i;

    printf("\n");

    for (i = 0; i < VECT_Length(v1); i++) {
        printf("%.2f ", VECT_GetValue(v1, i));
    }
    printf("\n");
}

VECT_Handle VectorTest_() {
    VECT_Vtype a[6] = {3,6,1,2,5,2};
    VECT_Handle v1 = VECT_CreateAlternative(6, a);

    return v1;
}

VECT_Handle VectorTest0() {
    VECT_Vtype a[6] = {1,2,3,4,5,6};
    VECT_Handle v1 = VECT_CreateAlternative(6, a);

    return v1;
}

// Тест на вывод вектора в консоль
int VectorTest1() {
    VECT_Handle v1 = VectorTest0();

#ifdef Debug
    VectorPrint(v1);
#endif

    VECT_Destroy(v1);

    return 0;
}

// Тест проверяющий модуль числа
int VectorTest2() {
    int test = 0;
    VECT_Handle v1 = VectorTest0();
    VECT_Vtype ans = VECT_Len(v1);

#ifdef Debug
    printf("\n%.2f\n", ans);
#endif

    if (ceil(ans) != 10) {
        test = 1;
    }

    VECT_Destroy(v1);

    return test;
}

// Тест скалярного произведения векторов
int VectorTest3() {
    int test = 0;
    VECT_Handle v1 = VectorTest0();
    VECT_Handle v2 = VectorTest_();
    VECT_Vtype ans = VECT_Dot(v1, v2);

#ifdef Debug
    printf("\n%.2f\n", ans);
#endif

    if (ceil(ans) != 63) {
        test = 1;
    }

    VECT_Destroy(v1);

    return test;
}

// Тест применение оператора к матрице
int VectorTest4() {
    int test = 0;
    VECT_Handle v1 = VectorTest0();
    VECT_Handle ans = VECT_ChangeByValue(v1, 3, '+');

    VECT_Vtype a[6] = {4,5,6,7,8,9};
    VECT_Handle answer = VECT_CreateAlternative(6, a);



#ifdef Debug
    VectorPrint(ans);
#endif

    if (VECT_CompareVector(ans, answer, 0.0001f) || VECT_CompareVector(v1, v1, 0.0001f)) {
        test = 1;
    }

    VECT_Destroy(ans);
    VECT_Destroy(v1);
    VECT_Destroy(answer);


    return test;
}

// Тест применение оператора к матрице
int VectorTest5() {
    int test = 0;

    VECT_Vtype a[3] = {1,2,3};
    VECT_Handle v1 = VECT_CreateAlternative(3, a);
    VECT_Vtype b[3] = {2,1,-2};
    VECT_Handle v2 = VECT_CreateAlternative(3, b);
    VECT_Handle answer = VECT_Cross_3(v1, v2);
    VECT_Vtype c[3] = {-7,8,-3};
    VECT_Handle ans = VECT_CreateAlternative(3, c);

#ifdef Debug
    VectorPrint(answer);
#endif

    if (VECT_CompareVector(ans, answer, 0.0001f)) {
        test = 1;
    }

    VECT_Destroy(v2);
    VECT_Destroy(v1);
    VECT_Destroy(answer);
    VECT_Destroy(ans);

    return test;
}

int main() {
    system("chcp 1251 > nul");

    printf("Test 1"); printf(VectorTest1() ? "\nERROR\n" : "\nOK\n");
    printf("Test 2"); printf(VectorTest2() ? "\nERROR\n" : "\nOK\n");
    printf("Test 3"); printf(VectorTest3() ? "\nERROR\n" : "\nOK\n");
    printf("Test 4"); printf(VectorTest4() ? "\nERROR\n" : "\nOK\n");
    printf("Test 5"); printf(VectorTest5() ? "\nERROR\n" : "\nOK\n");

    system("pause");

    return 0;
}