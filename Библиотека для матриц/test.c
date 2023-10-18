#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#define _Debug

void MatrixPrint(MATR_Handle v1) {
    int y, x;

    printf("\n");

    for (y = 0; y < MATR_Size(v1); y++) {
        for (x = 0; x < MATR_Size(v1); x++) {
            printf("%.2f ", MATR_GetValue(v1, y, x));
        }
        printf("\n");
    }
}

// Тест на создание матрицы
MATR_Handle MatrixTest_() {
    MATR_Handle v1 = MATR_Create(4);

    MATR_SetValue(v1, 0, 0, 1); MATR_SetValue(v1, 0, 1, 2); MATR_SetValue(v1, 0, 2, 3); MATR_SetValue(v1, 0, 3, 1);
    MATR_SetValue(v1, 1, 0, 3); MATR_SetValue(v1, 1, 1, 4); MATR_SetValue(v1, 1, 2, 1); MATR_SetValue(v1, 1, 3, 2);
    MATR_SetValue(v1, 2, 0, 3); MATR_SetValue(v1, 2, 1, 2); MATR_SetValue(v1, 2, 2, 1); MATR_SetValue(v1, 2, 3, 3);

    return v1;
}

MATR_Handle MatrixTest0() {
    MATR_Mtype a[9] = {1,2,3,3,4,1,3,2,1};
    MATR_Handle v1 = MATR_Create_and_Fill(3, 1.0, 2.0, 3.0, 3.0, 4.0, 1.0, 3.0, 2.0, 1.0);

    return v1;
}

// Тест на вывод значений матрицы в консольное окно
int MatrixTest1() {
    MATR_Handle v1 = MatrixTest0();

#ifdef Debug
    MatrixPrint(v1);
#endif

    MATR_Destroy(v1);

    return 0;
}

// Тест на получение обратной матрицы
int MatrixTest2() {
    int test = 0;
    MATR_Handle v1 = MatrixTest0();
    MATR_Handle ans = MATR_Inverse(v1);

    MATR_Mtype a[9] = {-0.13,-0.25,0.63,0,0.5,-0.5,0.38,-0.25,0.13};
    MATR_Handle answer = MATR_CreateAlternative(3, a);


#ifdef Debug
    MatrixPrint(ans);
#endif

    if (MATR_CompareMatrix(ans, answer, 0.01f) || MATR_CompareMatrix(v1, v1, 0.0001f)) {
        test = 1;
    }

    MATR_Destroy(v1);
    MATR_Destroy(ans);
    MATR_Destroy(answer);

    return test;
}

// Тест на получение транспонированной матрицы
int MatrixTest3() {
    int test = 0;
    MATR_Handle v1 = MatrixTest0();
    MATR_Handle ans = MATR_Transposed(v1);

    MATR_Mtype a[9] = {1,3,3,2,4,2,3,1,1};
    MATR_Handle answer = MATR_CreateAlternative(3, a);


#ifdef Debug
    MatrixPrint(ans);
#endif

    if (MATR_CompareMatrix(ans, answer, 0.0001f) || MATR_CompareMatrix(v1, v1, 0.0001f)) {
        test = 1;
    }

    MATR_Destroy(v1);
    MATR_Destroy(ans);
    MATR_Destroy(answer);

    return test;
}

// Тест на получение определителья матрицы
int MatrixTest4() {
    int test = 0;
    MATR_Handle v1 = MatrixTest0();
    MATR_Mtype ans = MATR_Determinant(v1);

#ifdef Debug
    printf("\n%.2f\n", ans);
#endif

    if (ans != -16) {
        test = 1;
    }

    MATR_Destroy(v1);

    return test;
};

// Тест на получение результата перемножения двух матриц
int MatrixTest5() {
    int test = 0;
    MATR_Handle v1 = MatrixTest0();
    MATR_Handle v2 = MatrixTest0();
    MATR_Handle ans = MATR_Multiplication(v1, v2);

    MATR_Mtype a[9] = {16,16,8,18,24,14,12,16,12};
    MATR_Handle answer = MATR_CreateAlternative(3, a);

#ifdef Debug
    MatrixPrint(ans);
#endif

    if (MATR_CompareMatrix(ans, answer, 0.0001f) || MATR_CompareMatrix(v1, v1, 0.0001f)) {
        test = 1;
    }

    MATR_Destroy(v1);
    MATR_Destroy(v2);
    MATR_Destroy(ans);
    MATR_Destroy(answer);

    return test;
};

// Тест на получение новой матрицы - результата применения операци (+ / - / * / /)
int MatrixTest6() {
    int test = 0;
    MATR_Handle v1 = MatrixTest0();
    MATR_Handle ans = MATR_ChangeByValue(v1, 3, '-');

    MATR_Mtype a[9] = {-2,-1,0,0,1,-2,0,-1,-2};
    MATR_Handle answer = MATR_CreateAlternative(3, a);

#ifdef Debug
    MatrixPrint(ans);
#endif

    if (MATR_CompareMatrix(ans, answer, 0.0001f) || MATR_CompareMatrix(v1, v1, 0.0001f)) {
        test = 1;
    }

    MATR_Destroy(v1);
    MATR_Destroy(ans);
    MATR_Destroy(answer);

    return test;
};

// Тест на решение матрицы методом Гаусса
int MatrixTest7() {
    int test = 0;
    int i;
    MATR_Handle v1 = MatrixTest_();
    MATR_Mtype * ans = MATR_Gauss(v1);
    MATR_Mtype a[3] = {1.25, -0.5, 0.25};

#ifdef Debug
    printf("\n", ans);
    for (i = 0; i < 3; i++)
        printf("%.2f\n", ans[i]);
#endif

    for (i = 0; i < 3; i++)
        if (ans[i] != a[i]) {
            test = 1;
            break;
        }

    MATR_Destroy(v1);
    free(ans);

    return test;
};

int main() {
    system("chcp 1251 > nul");

    printf("Test 1"); printf(MatrixTest1() ? "\nERROR\n" : "\nOK\n");
    printf("Test 2"); printf(MatrixTest2() ? "\nERROR\n" : "\nOK\n");
    printf("Test 3"); printf(MatrixTest3() ? "\nERROR\n" : "\nOK\n");
    printf("Test 4"); printf(MatrixTest4() ? "\nERROR\n" : "\nOK\n");
    printf("Test 5"); printf(MatrixTest5() ? "\nERROR\n" : "\nOK\n");
    printf("Test 6"); printf(MatrixTest6() ? "\nERROR\n" : "\nOK\n");
    printf("Test 7"); printf(MatrixTest7() ? "\nERROR\n" : "\nOK\n");

    system("pause");

    return 0;
}