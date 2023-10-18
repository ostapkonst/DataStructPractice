#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vector.h"

typedef struct {
    int n;
    VECT_Vtype * map;
} VECT_Vector;

VECT_Handle VECT_Create(int n) {
    VECT_Vector * ans = malloc(sizeof(VECT_Vector));

    ans->n = n;
    ans->map = malloc(sizeof(VECT_Vtype) * n);

    return ans;
}

void VECT_Destroy(VECT_Handle v1) {
    free(((VECT_Vector *)v1)->map);
    free(v1);
}

VECT_Handle VECT_CreateAlternative(int n, VECT_Vtype * values) {
    VECT_Vector * ans = VECT_Create(n);

    memcpy(ans->map, values, sizeof(VECT_Vtype) * n);

    return ans;
}

VECT_Handle VECT_Create_and_Fill(int n, ...) {
    int *p = &n;
    return VECT_CreateAlternative(n, (VECT_Vtype *)++p);
}

VECT_Vtype VECT_GetValue(VECT_Handle v1, int n) {
    return ((VECT_Vector *)v1)->map[n];
}

VECT_Vtype * VECT_GetValuePointer(VECT_Handle v1, int n) {
    return &((VECT_Vector *)v1)->map[n];
}

void VECT_SetValue(VECT_Handle v1, int n, VECT_Vtype value) {
    *VECT_GetValuePointer(v1, n) = value;
}

int VECT_Length(VECT_Handle v1) {
    return ((VECT_Vector *)v1)->n;
}

VECT_Vtype VECT_Len(VECT_Handle v1) {
    int i;
    VECT_Vtype ans = 0;

    for (i = 0; i < VECT_Length(v1); i++) {
        ans += powf(VECT_GetValue(v1, i), 2);
    }

    return (VECT_Vtype)sqrt(ans);
}

VECT_Handle VECT_ChangeByValue(VECT_Handle v1, VECT_Vtype value, char sign) {
    int i;
    VECT_Vector * ans = VECT_Create(VECT_Length(v1));

    for (i = 0; i < VECT_Length(v1); i++) {
        switch (sign) {
            case '+' : *VECT_GetValuePointer(ans, i) = VECT_GetValue(v1, i) + value; break;
            case '-' : *VECT_GetValuePointer(ans, i) = VECT_GetValue(v1, i) - value; break;
            case '*' : *VECT_GetValuePointer(ans, i) = VECT_GetValue(v1, i) * value; break;
            case '/' : *VECT_GetValuePointer(ans, i) = VECT_GetValue(v1, i) / value; break;
        }
    }

    return ans;
}

VECT_Handle VECT_ChangeByVector(VECT_Handle v1, VECT_Handle v2, char sign) {
    int i;
    VECT_Vector * ans = VECT_ChangeByValue(v1, 0, '+');

    for (i = 0; i < VECT_Length(v1); i++) {
        switch (sign) {
            case '+' : *VECT_GetValuePointer(ans, i) += VECT_GetValue(v2, i); break;
            case '-' : *VECT_GetValuePointer(ans, i) -= VECT_GetValue(v2, i); break;
            case '*' : *VECT_GetValuePointer(ans, i) *= VECT_GetValue(v2, i); break;
            case '/' : *VECT_GetValuePointer(ans, i) /= VECT_GetValue(v2, i); break;
        }
    }

    return ans;
}

BOOL VECT_Collinearity(VECT_Handle v1, VECT_Handle v2) {
    int i;
    VECT_Vtype col;

    col = VECT_GetValue(v1, 0) / VECT_GetValue(v2, 0);
    for (i = 1; i < VECT_Length(v1); i++) {
        if (col != VECT_GetValue(v1, i) / VECT_GetValue(v2, i)) {
            return 1;
        }
    }

    return 0;
}

VECT_Vtype VECT_Dot(VECT_Handle v1, VECT_Handle v2) {
    int i;
    VECT_Vtype ans = 0;

    for (i = 0; i < VECT_Length(v1); i++) {
        ans += VECT_GetValue(v1, i) * VECT_GetValue(v2, i);
    }

    return ans;
}

VECT_Handle VECT_Cross_3(VECT_Handle v1, VECT_Handle v2) {
    VECT_Vector * ans = VECT_Create(VECT_Length(v1));

    *VECT_GetValuePointer(ans, 0) = VECT_GetValue(v1, 1) * VECT_GetValue(v2, 2) - VECT_GetValue(v1, 2) * VECT_GetValue(v2, 1);
    *VECT_GetValuePointer(ans, 1) = VECT_GetValue(v1, 2) * VECT_GetValue(v2, 0) - VECT_GetValue(v1, 0) * VECT_GetValue(v2, 2);
    *VECT_GetValuePointer(ans, 2) = VECT_GetValue(v1, 0) * VECT_GetValue(v2, 1) - VECT_GetValue(v1, 1) * VECT_GetValue(v2, 0);

    return ans;
}

VECT_Vtype * VECT_GetMap(VECT_Handle v1) {
    return ((VECT_Vector *)v1)->map;
}

BOOL VECT_CompareMap(VECT_Vtype * m1, VECT_Vtype * m2, int size) {
    return memcmp(m1, m2, sizeof(VECT_Vtype) * size);
}

BOOL VECT_CompareVector(VECT_Handle v1, VECT_Handle v2, float n) {
    int i;

    if (VECT_Length(v1) != VECT_Length(v2)) {
        return 1;
    }

    for (i = 0; i < VECT_Length(v1); i++) {
        if (fabs(VECT_GetValue(v1, i) - VECT_GetValue(v2, i)) > n) {
            return 1;
        }
    }

    return 0;
}