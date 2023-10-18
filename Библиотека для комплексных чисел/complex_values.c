#include <stdio.h>
#include <stdlib.h>
#include "complex_values.h"
#define _USE_MATH_DEFINES
#include <math.h>

typedef enum {
    ALGEBRAIC,
    EXPONENTIAL
} CMP_DefineFormT;

typedef struct {
    CMP_DefineFormT form;
    union {CMP_RealT r, re;};
    union {CMP_ImaginaryT i, fe;};
} CMP_ComplexT;

static void CMP_Coppy(const CMP_ComplexT *in_A, CMP_ComplexT *out_B) {
    out_B->form = in_A->form;
    out_B->r = in_A->r;
    out_B->i = in_A->i;
}

static CMP_ComplexT *CMP_CreateCoppy(const CMP_ComplexT *in_A) {
    CMP_ComplexT *CV = CMP_Create(0, 0);
    CMP_Coppy(in_A, CV);
    return CV;
}

static void CMP_AlgToExp(CMP_ComplexT *Alg) {
    CMP_ComplexT *CV;
    if (Alg->form != ALGEBRAIC) return;
    CV = CMP_CreateCoppy(Alg);
    Alg->form = EXPONENTIAL;
    Alg->re = sqrt(pow(CV->r, 2) + pow(CV->i, 2));
    Alg->fe = atan2(CV->i, CV->r);
    CMP_Destroy(CV);
}

static void CMP_ExpToAlg(CMP_ComplexT *Exp) {
    CMP_ComplexT *CV;
    if (Exp->form != EXPONENTIAL) return;
    CV = CMP_CreateCoppy(Exp);
    Exp->form = ALGEBRAIC;
    Exp->r = CV->re * cos(CV->fe);
    Exp->i = CV->re * sin(CV->fe);
    CMP_Destroy(CV);
}

CMP_HandleT CMP_Create(CMP_RealT r, CMP_ImaginaryT i) {
    CMP_ComplexT *CV = malloc(sizeof(CMP_ComplexT));
    CV->form = ALGEBRAIC;
    CV->r = r;
    CV->i = i;
    return CV;
}
void CMP_Destroy(CMP_HandleT handle) {
    free(handle);
}

void CMP_SetValues(CMP_HandleT handle, CMP_RealT r, CMP_ImaginaryT i) {
    CMP_ComplexT *CV = handle;
    CMP_ExpToAlg(CV);
    CV->r = r;
    CV->i = i;
}

void CMP_SetReal(CMP_HandleT handle, CMP_RealT r) {
    CMP_ComplexT *CV = handle;
    CMP_ExpToAlg(CV);
    CV->r = r;
}

void CMP_SetImaginary(CMP_HandleT handle, CMP_ImaginaryT i) {
    CMP_ComplexT *CV = handle;
    CMP_ExpToAlg(CV);
    CV->i = i;
}

CMP_RealT CMP_GetReal(const CMP_HandleT handle) {
    CMP_ComplexT *CV = handle;
    return CV->form == ALGEBRAIC ? CV->r : CV->re * cos(CV->fe);
}

CMP_ImaginaryT CMP_GetImaginary(const CMP_HandleT handle) {
    CMP_ComplexT *CV = handle;
    return CV->form == ALGEBRAIC ? CV->i : CV->re * sin(CV->fe);
}

BOOL CMP_Equally(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_RealT exp_r, CMP_ImaginaryT exp_i) {
    CMP_ComplexT *CV_A = in_A, *CV_B = in_B;
    return (fabs(CV_A->r - CV_B->r) < exp_r && fabs(CV_A->i - CV_B->i) < exp_i) ? TRUE : FALSE;
}

void CMP_Add(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C) {
    CMP_ComplexT *CV_A = in_A, *CV_B = in_B, *CV_C = out_C;
    CMP_ExpToAlg(CV_A);
    CMP_ExpToAlg(CV_B);
    CMP_ExpToAlg(CV_C);
    CV_C->r = CV_A->r + CV_B->r;
    CV_C->i = CV_A->i + CV_B->i;
}

void CMP_Sub(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C) {
    CMP_ComplexT *CV_A = in_A, *CV_B = in_B, *CV_C = out_C;
    CMP_ExpToAlg(CV_A);
    CMP_ExpToAlg(CV_B);
    CMP_ExpToAlg(CV_C);
    CV_C->r = CV_A->r - CV_B->r;
    CV_C->i = CV_A->i - CV_B->i;
}

void CMP_Mul(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C) {
    CMP_ComplexT *CV_A = in_A, *CV_B = in_B, *CV_C = out_C;
    CMP_ExpToAlg(CV_A);
    CMP_ExpToAlg(CV_B);
    CMP_ExpToAlg(CV_C);
    CV_C->r = CV_A->r * CV_B->r - CV_A->i * CV_B->i;
    CV_C->i = CV_A->r * CV_B->i + CV_A->i * CV_B->r;
}

void CMP_Div(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C) {
    CMP_ComplexT *CV_A = in_A, *CV_B = in_B, *CV_C = out_C;
    double d;
    CMP_ExpToAlg(CV_A);
    CMP_ExpToAlg(CV_B);
    CMP_ExpToAlg(CV_C);
    d = (pow(CV_B->r, 2) + pow(CV_B->i, 2));
    CV_C->r = (CV_A->r * CV_B->r + CV_A->i * CV_B->i) / d;
    CV_C->i = (CV_A->i * CV_B->r - CV_A->r * CV_B->i) / d;
}

CMP_RealT CMP_GetAbs(const CMP_HandleT handle) {
    CMP_ComplexT *CV = handle;
    return CV->form == EXPONENTIAL ? CV->re : sqrt(pow(CV->r, 2) + pow(CV->i, 2));
}

void CMP_Sqr(const CMP_HandleT in_A, CMP_HandleT out_B) {
    CMP_ComplexT *CV_A = in_A, *CV_B = out_B;
    CMP_ExpToAlg(CV_A);
    CMP_ExpToAlg(CV_B);
    CV_B->r = pow(CV_A->r, 2) - pow(CV_A->i, 2);
    CV_B->i = 2 * CV_A->r * CV_A->i;
}

CMP_ImaginaryT CMP_GetArg(const CMP_HandleT handle) {
    CMP_ComplexT *CV = handle;
    CMP_ExpToAlg(CV);
    return CV->form == EXPONENTIAL ? CV->fe : atan2(CV->i, CV->r);
}

void CMP_Conjugate(const CMP_HandleT in_A, CMP_HandleT out_B) {
    CMP_ComplexT *CV_A = in_A, *CV_B = out_B;
    CMP_ExpToAlg(CV_A);
    CMP_ExpToAlg(CV_B);
    CV_B->r = CV_A->r;
    CV_B->i = -CV_A->i;
}

CMP_ImaginaryOneT CMP_ImaginarySqr(int n) {
    return n < 0 ? 3 + (n + 1) % 4 : n % 4;
}

void CMP_Pow(const CMP_HandleT in_A, CMP_HandleT out_B, int n) {
    CMP_ComplexT *CV_A = in_A, *CV_B = out_B;
    CMP_AlgToExp(CV_A);
    CMP_AlgToExp(CV_B);
    CV_B->re = pow(CV_A->re, n);
    CV_B->fe = n * CV_A->fe;
}

void CMP_Sqrt(const CMP_HandleT in_A, CMP_HandleT out_B[], unsigned int n) {
    CMP_ComplexT *CV_A = in_A, **CV_B = out_B;
    unsigned int i;
    double r;
    CMP_AlgToExp(CV_A);
    r = pow(CV_A->re, 1.0f / n);
    for (i = 0; i < n; i++) {
        CMP_AlgToExp(CV_B[i]);
        CV_B[i]->re = r;
        CV_B[i]->fe = (CV_A->fe + 2 * M_PI * i) / n;
    }
}