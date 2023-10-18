#ifndef COMPLEX_VALUES_H
#define COMPLEX_VALUES_H

#ifdef DEFINE_BOOL
#define BOOL int
#define FALSE 0
#define TRUE 1
#else
typedef enum {
    FALSE,
    TRUE
} BOOL;
#endif

typedef enum {
    ONE,
    IMAGINARY,
    MINUS_ONE,
    MINUS_IMAGINARY,
} CMP_ImaginaryOneT; /* Значения мнимой единицы*/

#include <stdlib.h>

typedef float CMP_ImaginaryT; /* Мнимая часть */
typedef float CMP_RealT; /* Действительная часть */

/* Дескриптор комплексного числа */
typedef void* CMP_HandleT;

/* Создание комплексного числа в алгебраической форме */
extern CMP_HandleT CMP_Create(CMP_RealT r, CMP_ImaginaryT i);
/* Удаление комплексного числа */
extern void CMP_Destroy(CMP_HandleT handle);

/* Следующие три функции задают значение ранее созданного комплексного числа */
extern void CMP_SetValues(CMP_HandleT handle,  CMP_RealT r, CMP_ImaginaryT i);
extern void CMP_SetReal(CMP_HandleT handle, CMP_RealT r);
extern void CMP_SetImaginary(CMP_HandleT handle, CMP_ImaginaryT i);

/* Две функции возвращают значения вещественной и мнимой частей соответственно */
extern CMP_RealT CMP_GetReal(const CMP_HandleT handle);
extern CMP_ImaginaryT CMP_GetImaginary(const CMP_HandleT handle);

/* Сравнение двух комплексных чисел (возвращает TRUE если равно и FALSE в противном случае)*/
extern BOOL CMP_Equally(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_RealT exp_r, CMP_ImaginaryT exp_i);

/* Основные действия над комплексными числами */
extern void CMP_Add(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C);
extern void CMP_Sub(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C);
extern void CMP_Mul(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C);
extern void CMP_Div(const CMP_HandleT in_A, const CMP_HandleT in_B, CMP_HandleT out_C);

/* Две функции возвращают значения модуля и фи соответственно */
extern CMP_RealT CMP_GetAbs(const CMP_HandleT handle);
extern CMP_ImaginaryT CMP_GetArg(const CMP_HandleT handle);

/* Возведение комплексного числа во вторую степень */
extern void CMP_Sqr(const CMP_HandleT in_A, CMP_HandleT out_B);
/* Возвращает комплексно-сопряженное число */
extern void CMP_Conjugate(const CMP_HandleT in_A, CMP_HandleT out_B);

/* Возвращает значение мнимой единицы в степени n */
extern CMP_ImaginaryOneT CMP_ImaginarySqr(int n);

/* Возведение комплексного числа в степень n */
extern void CMP_Pow(const CMP_HandleT in_A, CMP_HandleT out_B, int n);
/* Корень из комплексного числа */
extern void CMP_Sqrt(const CMP_HandleT in_A, CMP_HandleT out_B[], unsigned int n);

#endif