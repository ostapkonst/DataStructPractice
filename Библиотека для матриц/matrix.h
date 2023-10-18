#ifndef MATR_H
#define MATR_H

typedef void * MATR_Handle; /* Дескриптор матрицы */
typedef double MATR_Mtype; /* Тип хранимых в матрице значений */
typedef int BOOL; /* Логический тип данных */

/* Выделяет память под матрицу, возвращает указатель на неё */
extern MATR_Handle MATR_Create(int n);
/* Создание единичной матрицы */
extern MATR_Handle MATR_CreateIdentity(int n);
/* Альтернативное создание новой матрицы */
extern MATR_Handle MATR_CreateAlternative(int n, MATR_Mtype * values);
/* Альтернативное создание новой матрицы */
extern MATR_Handle MATR_Create_and_Fill(int n, ...);

/* Удаляет матрицу с очисткой памяти */
extern void MATR_Destroy(MATR_Handle v1);

/* Возвращает размер матрицы */
extern int MATR_Size(MATR_Handle v1);
/* Общее количество ячеек в матрице */
extern int MATR_Length(MATR_Handle v1);
/* Возвращает указатель на массив со значениями матрицы */
extern MATR_Mtype * MATR_GetMap(MATR_Handle v1);

/* Побитовое сравнение значений двух матриц */
extern BOOL MATR_CompareMap(MATR_Mtype * m1, MATR_Mtype * m2, int size);
/* Сравнение двух матриц до n-ых */
extern BOOL MATR_CompareMatrix(MATR_Handle v1, MATR_Handle v2, float n);

/* Возвращает значение элемента матрицы */
extern MATR_Mtype MATR_GetValue(MATR_Handle v1, int y, int x);
/* Возвращает указатель на элемент матрицы */
extern MATR_Mtype * MATR_GetValuePointer(MATR_Handle v1, int y, int x);
/* Изменяет значение элемента */
extern void MATR_SetValue(MATR_Handle v1, int y, int x, MATR_Mtype value);

/* Решение матрицы методом Гаусса (приведение к треугольному виду) */
extern MATR_Handle MATR_Gauss(MATR_Handle v1);
/* Нахождение определителя матрицы */
extern MATR_Mtype MATR_Determinant(MATR_Handle v1);
/* Транспонирование матрицы */
extern MATR_Handle MATR_Transposed(MATR_Handle v1);
/* Нахождение обратной матрицы */
extern MATR_Handle MATR_Inverse(MATR_Handle v1);

/* Перемножение матриц */
extern MATR_Handle MATR_Multiplication(MATR_Handle v1, MATR_Handle v2);
/* Действия со значениями матрицы (сложение/вычитание/умножение/деление на число) */
/* sign принимает знак операции изменения матричных значений (+, -, *, /)       */
extern MATR_Handle MATR_ChangeByValue(MATR_Handle v1, MATR_Mtype value, char sign);
/* Действия со значениями матрицы (сложение/вычитание/умножение/деление на число) */
/* sign принимает знак операции изменения матричных значений (+, -, *, /)       */
extern MATR_Handle MATR_ChangeByMatrix(MATR_Handle v1, MATR_Handle v2, char sign);

#endif