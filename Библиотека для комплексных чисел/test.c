#include "complex_values.h"
#include <stdio.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define ACCURACY 0.00001
#define DEBUG_MODE 0

typedef BOOL (*TEST_FuncT)(void);

BOOL CompareComplexArguments(CMP_HandleT T_CV, CMP_RealT r, CMP_ImaginaryT i) {
#if DEBUG_MODE
    printf("(%f, %f) = (%f, %f)\n", CMP_GetReal(T_CV), CMP_GetImaginary(T_CV), r, i);
#endif
    return (fabs(CMP_GetReal(T_CV) - r) < ACCURACY &&
            fabs(CMP_GetImaginary(T_CV) - i) < ACCURACY) ? TRUE : FALSE;
}

BOOL TEST_CreateDestroyComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(11, 22);
    BOOL result = CompareComplexArguments(T_CV, 11, 22);

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_SetComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(0, 0);
    BOOL result;

    CMP_SetValues(T_CV, 8, 9);
    result = CompareComplexArguments(T_CV, 8, 9);

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_SetRealComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(0, 0);
    BOOL result;

    CMP_SetReal(T_CV, 8);
    result = CompareComplexArguments(T_CV, 8, 0);

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_SetImaginaryComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(0, 0);
    BOOL result;

    CMP_SetImaginary(T_CV, 9);
    result = CompareComplexArguments(T_CV, 0, 9);

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_GetRealComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(8, 9);
    BOOL result = CMP_GetReal(T_CV) == 8;

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_GetImaginaryComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(8, 9);
    BOOL result = CMP_GetImaginary(T_CV) == 9;

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_EquallyComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(8, 9), T_CV_B = CMP_Create(8, 9);

    BOOL result = CMP_Equally(T_CV_A, T_CV_B, ACCURACY, ACCURACY);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);

    return result;
}

BOOL TEST_AddComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(1, 3), T_CV_B = CMP_Create(4, -5), T_CV_C = CMP_Create(0, 0);
    BOOL result;

    CMP_Add(T_CV_A, T_CV_B, T_CV_C);
    result = CompareComplexArguments(T_CV_C, 5, -2);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);
    CMP_Destroy(T_CV_C);

    return result;
}

BOOL TEST_SubComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(-2, 1), T_CV_B = CMP_Create(sqrt(3), 5), T_CV_C = CMP_Create(0, 0);
    BOOL result;

    CMP_Sub(T_CV_A, T_CV_B, T_CV_C);
    result = CompareComplexArguments(T_CV_C, - 2 - sqrt(3), -4);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);
    CMP_Destroy(T_CV_C);

    return result;
}

BOOL TEST_MulComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(1, -1), T_CV_B = CMP_Create(3, 6), T_CV_C = CMP_Create(0, 0);
    BOOL result;

    CMP_Mul(T_CV_A, T_CV_B, T_CV_C);
    result = CompareComplexArguments(T_CV_C, 9, 3);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);
    CMP_Destroy(T_CV_C);

    return result;
}

BOOL TEST_DivComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(13, 1), T_CV_B = CMP_Create(7, -6), T_CV_C = CMP_Create(0, 0);
    BOOL result;

    CMP_Div(T_CV_A, T_CV_B, T_CV_C);
    result = CompareComplexArguments(T_CV_C, 1, 1);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);
    CMP_Destroy(T_CV_C);

    return result;
}

BOOL TEST_AbsComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(-2, 4);
    BOOL result = fabs(CMP_GetAbs(T_CV) - 2 * sqrt(5)) < ACCURACY;

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_SqrComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(2, 3), T_CV_B = CMP_Create(0, 0);
    BOOL result;

    CMP_Sqr(T_CV_A, T_CV_B);
    result = CompareComplexArguments(T_CV_B, -5, 12);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);

    return result;
}

BOOL TEST_GetArgComplexValue(void) {
    CMP_HandleT T_CV = CMP_Create(1, -sqrt(3));
    BOOL result = fabs(CMP_GetArg(T_CV) + M_PI / 3) < ACCURACY;

    CMP_Destroy(T_CV);

    return result;
}

BOOL TEST_ConjugateComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(2, 4), T_CV_B = CMP_Create(0, 0);
    BOOL result;

    CMP_Conjugate(T_CV_A, T_CV_B);
    result = CompareComplexArguments(T_CV_B, 2, -4);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);

    return result;
}

BOOL TEST_ImaginarySqrComplexValue(void) {
    return  CMP_ImaginarySqr(-4) == ONE &&
            CMP_ImaginarySqr(-3) == IMAGINARY &&
            CMP_ImaginarySqr(-2) == MINUS_ONE &&
            CMP_ImaginarySqr(-1) == MINUS_IMAGINARY &&
            CMP_ImaginarySqr(0) == ONE &&
            CMP_ImaginarySqr(1) == IMAGINARY &&
            CMP_ImaginarySqr(2) == MINUS_ONE &&
            CMP_ImaginarySqr(3) == MINUS_IMAGINARY &&
            CMP_ImaginarySqr(4) == ONE;
}

BOOL TEST_PowComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(2, 3), T_CV_B = CMP_Create(0, 0);
    BOOL result;

    CMP_Pow(T_CV_A, T_CV_B, 2);
    result = CompareComplexArguments(T_CV_B, -5, 12);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B);

    return result;
}

BOOL TEST_SqrtComplexValue(void) {
    CMP_HandleT T_CV_A = CMP_Create(1, sqrt(3)), T_CV_B[2];
    BOOL result;

    T_CV_B[0] = CMP_Create(0, 0);
    T_CV_B[1] = CMP_Create(0, 0);

    CMP_Sqrt(T_CV_A, T_CV_B, 2);
    result = CompareComplexArguments(T_CV_B[0], 1.224745, 0.707107) &&
             CompareComplexArguments(T_CV_B[1], -1.224745, -0.707107);

    CMP_Destroy(T_CV_A);
    CMP_Destroy(T_CV_B[0]);
    CMP_Destroy(T_CV_B[1]);

    return result;
}

int main(){
    TEST_FuncT funcs[] = {
        TEST_CreateDestroyComplexValue,
        TEST_SetComplexValue,
        TEST_SetRealComplexValue,
        TEST_SetImaginaryComplexValue,
        TEST_GetRealComplexValue,
        TEST_GetImaginaryComplexValue,
        TEST_EquallyComplexValue,
        TEST_AddComplexValue,
        TEST_SubComplexValue,
        TEST_MulComplexValue,
        TEST_DivComplexValue,
        TEST_AbsComplexValue,
        TEST_SqrComplexValue,
        TEST_GetArgComplexValue,
        TEST_ConjugateComplexValue,
        TEST_ImaginarySqrComplexValue,
        TEST_PowComplexValue,
        TEST_SqrtComplexValue
    };
    unsigned int i, size = sizeof(funcs) / sizeof(TEST_FuncT);

    printf("TEST COUNT %i\n", size);
    for (i = 0; i < size; i++)
        printf("TEST %i: %s\n", i + 1, funcs[i]() ? "OK" : "FAIL");
    printf("TEST IS FINISHED\n");

    system("PAUSE");
    return 0;
}
