#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

typedef struct {
    int n;
    MATR_Mtype * map;
} MATR_Matrix;

MATR_Handle MATR_Create(int n) {
    MATR_Matrix * ans = malloc(sizeof(MATR_Matrix));

    ans->n = n;
    ans->map = malloc(sizeof(MATR_Mtype) * n * n);

    return ans;
}

MATR_Handle MATR_CreateIdentity(int n) {
    int i, j;
    MATR_Matrix * ans = MATR_Create(n);

    for (i = 0; i < MATR_Size(ans); i++) {
        for (j = 0; j < MATR_Size(ans); j++) {
            *MATR_GetValuePointer(ans, i, j) = (MATR_Mtype)(i == j ? 1 : 0);
        }
    }

    return ans;
}

MATR_Handle MATR_CreateAlternative(int n, MATR_Mtype * values) {
    MATR_Matrix * ans = MATR_Create(n);

    memcpy(ans->map, values, sizeof(MATR_Mtype) * n * n);

    return ans;
}

MATR_Handle MATR_Create_and_Fill(int n, ...) {
    int *p = &n;
    return MATR_CreateAlternative(n, (MATR_Mtype *)++p);
}

void MATR_Destroy(MATR_Handle v1) {
    free(((MATR_Matrix *)v1)->map);
    free(v1);
}

int MATR_Size(MATR_Handle v1) {
    return ((MATR_Matrix *)v1)->n;
}

int MATR_Length(MATR_Handle v1) {
    return MATR_Size(v1) * MATR_Size(v1);
}

MATR_Mtype * MATR_GetMap(MATR_Handle v1) {
    return ((MATR_Matrix *)v1)->map;
}

BOOL MATR_CompareMap(MATR_Mtype * m1, MATR_Mtype * m2, int size) {
    return memcmp(m1, m2, sizeof(MATR_Mtype) * size);
}

BOOL MATR_CompareMatrix(MATR_Handle v1, MATR_Handle v2, float n) {
    int i, j;

    if (MATR_Size(v1) != MATR_Size(v2)) {
        return 1;
    }

    for (i = 0; i < MATR_Size(v1); i++) {
        for (j = 0; j < MATR_Size(v1); j++) {
            if (fabs(MATR_GetValue(v1, i, j) - MATR_GetValue(v2, i, j)) > n) {
                    return 1;
            }
        }
    }

    return 0;
}

MATR_Mtype MATR_GetValue(MATR_Handle v1, int y, int x) {
    return ((MATR_Matrix *)v1)->map[MATR_Size(v1) * y + x];
}

MATR_Mtype * MATR_GetValuePointer(MATR_Handle v1, int y, int x) {
    return &((MATR_Matrix *)v1)->map[MATR_Size(v1) * y + x];
}

void MATR_SetValue(MATR_Handle v1, int y, int x, MATR_Mtype value) {
    *MATR_GetValuePointer(v1, y, x) = value;
}

MATR_Handle MATR_Gauss(MATR_Handle v1) {
    int i, j, k;
    MATR_Matrix * ans = MATR_ChangeByValue(v1, 0, '+');
    MATR_Mtype * x = malloc(sizeof(MATR_Mtype) * (MATR_Size(ans) - 1));

    for (i = 0; i < MATR_Size(ans) - 1; i++) {
        MATR_Mtype a = MATR_GetValue(ans, i, i);
        for (j = i + 1; j < MATR_Size(ans) - 1; j++) {
            MATR_Mtype b = MATR_GetValue(ans, j, i);
            for (k = i; k < MATR_Size(ans); k++) {
                *MATR_GetValuePointer(ans, j, k) = MATR_GetValue(ans, i, k) * b - MATR_GetValue(ans, j, k) * a;
            }
        }
    }

    for (i = MATR_Size(ans) - 2; i >= 0; i--) {
        MATR_Mtype summ = 0;
        for (j = i + 1; j < MATR_Size(ans) - 1; j++) {
            summ += MATR_GetValue(ans, i, j) * x[j];
        }
        summ = MATR_GetValue(ans, i, MATR_Size(ans) - 1) - summ;
        x[i] = summ / MATR_GetValue(ans, i, i);
    }

    MATR_Destroy(ans);
    return x;
}

MATR_Mtype MATR_Determinant(MATR_Handle v1) {
    int i, j, k;
    MATR_Mtype answer = 1;
    MATR_Matrix * ans = MATR_ChangeByValue(v1, 0, '+');

    for (i = 0; i < MATR_Size(ans); i++) {
        for (j = i + 1; j < MATR_Size(ans); j++) {
            MATR_Mtype b = MATR_GetValue(ans, j, i) / MATR_GetValue(ans, i, i);
            for(k = i; k < MATR_Size(ans); k++) {
                *MATR_GetValuePointer(ans, j, k) -= MATR_GetValue(ans, i, k) * b;
            }
        }
        answer *= MATR_GetValue(ans, i, i);
    }

    MATR_Destroy(ans);
    return answer;
}

MATR_Handle MATR_Transposed(MATR_Handle v1) {
    int i, j;
    MATR_Matrix * answer = MATR_Create(MATR_Size(v1));

    for (i = MATR_Size(v1) - 1; i >= 0; i--) {
        for (j = MATR_Size(v1) - 1; j >= 0; j--) {
            *MATR_GetValuePointer(answer, j, i) = MATR_GetValue(v1, i, j);
        }
    }

    return answer;
}

MATR_Handle MATR_Inverse(MATR_Handle v1) {
    int i, j, k;
    MATR_Mtype tmp;
    MATR_Matrix * ans = MATR_ChangeByValue(v1, 0, '+');
    MATR_Matrix * answer = MATR_CreateIdentity(MATR_Size(ans));

    for(i = 0; i < MATR_Size(ans); i++) {
        tmp = MATR_GetValue(ans, i, i);
        for(j = MATR_Size(ans) - 1; j >=0; j--) {
            *MATR_GetValuePointer(answer, i, j) /= tmp;
            *MATR_GetValuePointer(ans, i, j) /= tmp;
        }

        for(j = 0; j < MATR_Size(ans); j++) {
            if (j != i) {
                tmp = MATR_GetValue(ans, j, i);
                for(k = MATR_Size(ans)-1; k >= 0; k--) {
                    *MATR_GetValuePointer(answer, j, k) -= MATR_GetValue(answer, i, k) * tmp;
                    *MATR_GetValuePointer(ans, j, k) -= MATR_GetValue(ans, i, k) * tmp;
                }
            }
        }
    }

    MATR_Destroy(ans);
    return answer;
}

MATR_Handle MATR_Multiplication(MATR_Handle v1, MATR_Handle v2) {
    int i, j, k;
    MATR_Matrix * answer = MATR_Create(MATR_Size(v1));

    for (i = 0; i < MATR_Size(v1); i++) {
        for (j = 0; j < MATR_Size(v1); j++) {
            *MATR_GetValuePointer(answer, i, j) = 0;
            for (k = 0; k < MATR_Size(v1); k++) {
                *MATR_GetValuePointer(answer, i, j) += MATR_GetValue(v1, i, k) * MATR_GetValue(v2, k, j);
            }
        }
    }

    return answer;
}

MATR_Handle MATR_ChangeByValue(MATR_Handle v1, MATR_Mtype value, char sign) {
    int i, j;
    MATR_Matrix * answer = MATR_Create(MATR_Size(v1));

    for (i = 0; i < MATR_Size(v1); i++) {
        for (j = 0; j < MATR_Size(v1); j++) {
            switch (sign) {
                case '+' : *MATR_GetValuePointer(answer, i, j) = MATR_GetValue(v1, i, j) + value; break;
                case '-' : *MATR_GetValuePointer(answer, i, j) = MATR_GetValue(v1, i, j) - value; break;
                case '*' : *MATR_GetValuePointer(answer, i, j) = MATR_GetValue(v1, i, j) * value; break;
                case '/' : *MATR_GetValuePointer(answer, i, j) = MATR_GetValue(v1, i, j) / value; break;
            }
        }
    }

    return answer;
}

MATR_Handle MATR_ChangeByMatrix(MATR_Handle v1, MATR_Handle v2, char sign) {
    int i, j;
    MATR_Matrix * answer = MATR_ChangeByValue(v1, 0, '+');

    for (i = 0; i < MATR_Size(v1); i++) {
        for (j = 0; j < MATR_Size(v1); j++) {
            switch (sign) {
                case '+' : *MATR_GetValuePointer(answer, i, j) += MATR_GetValue(v2, i, j); break;
                case '-' : *MATR_GetValuePointer(answer, i, j) -= MATR_GetValue(v2, i, j); break;
                case '*' : *MATR_GetValuePointer(answer, i, j) *= MATR_GetValue(v2, i, j); break;
                case '/' : *MATR_GetValuePointer(answer, i, j) /= MATR_GetValue(v2, i, j); break;
            }
        }
    }

    return answer;
};