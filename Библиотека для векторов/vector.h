#ifndef VECT_H
#define VECT_H

typedef void * VECT_Handle; /* Дескриптор вектора */
typedef double VECT_Vtype; /* Тип хранимого в векторе значений */
typedef int BOOL; /* Логический тип данных */

/* Создает новый вектор, возвращает указатель на выделенную память */
extern VECT_Handle VECT_Create(int n);
/* Удаляет вектор вместе с занятой под неё памятью */
extern void VECT_Destroy(VECT_Handle v1);
/* Альтернативное создание вектора */
extern VECT_Handle VECT_CreateAlternative(int n, VECT_Vtype * values);
/* Альтернативное создание вектора */
extern VECT_Handle VECT_Create_and_Fill(int n, ...);

/* Возвращает значение по одной из координат вектора */
extern VECT_Vtype VECT_GetValue(VECT_Handle v1, int n);
/* Возвращает указатель на значение по одной из координат вектора */
extern VECT_Vtype * VECT_GetValuePointer(VECT_Handle v1, int n);
/* Изменяет значение вектора */
extern void VECT_SetValue(VECT_Handle v1, int n, VECT_Vtype value);

/* Общее количество элементов в векторе */
extern int VECT_Length(VECT_Handle v1);

/* Длина вектора (модуль) */
extern VECT_Vtype VECT_Len(VECT_Handle v1);

/* Применяет оператор (сложение / вычитание / умножение / деление) ко всем значениям вектора */
/* sign - знак операции (+ / - / * / /)                                                      */
extern VECT_Handle VECT_ChangeByValue(VECT_Handle v1, VECT_Vtype value, char sign);
/* Применяет оператор (сложение / вычитание / умножение / деление) ко всем значениям вектора */
/* sign - знак операции (+ / - / * / /)                                                      */
extern VECT_Handle VECT_ChangeByVector(VECT_Handle v1, VECT_Handle v2, char sign);

/* Коллинеарность векторов */
extern BOOL VECT_Collinearity(VECT_Handle v1, VECT_Handle v2);
/* Скалярное произведение векторов */
extern VECT_Vtype VECT_Dot(VECT_Handle v1, VECT_Handle v2);
/* Векторное произведение векторов */
extern VECT_Handle VECT_Cross_3(VECT_Handle v1, VECT_Handle v2);

/* Возвращает указатель на массив со значениями вектора */
extern VECT_Vtype * VECT_GetMap(VECT_Handle v1);
/* Побитовое сравнение значений вектора */
extern BOOL VECT_CompareMap(VECT_Vtype * m1, VECT_Vtype * m2, int size);

/* Сравнение двух векторов до n-ых */
extern BOOL VECT_CompareVector(VECT_Handle v1, VECT_Handle v2, float n);

#endif