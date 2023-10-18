#ifndef GRID_H
#define GRID_H

#include <stdlib.h>

/* Логический тип */
#define BOOL int

/* Дескриптор данных параметра */
#define GRID_ParamDataT void*

/* Дескриптор функции, изменяющей данные параметра */
typedef void (*GRID_ParamChangeFuncT)(GRID_ParamDataT data);

/* Дескриптор функции, отображающей параметр в логическое значение */
typedef BOOL (*GRID_ParamBOOLFuncT)(GRID_ParamDataT data);

typedef struct {
    GRID_ParamDataT data; /*данные параметра*/
    GRID_ParamChangeFuncT f_next; /*следующее значение параметра*/
    GRID_ParamChangeFuncT f_init; /*инициализация параметра начальным значением*/
    GRID_ParamBOOLFuncT f_is_after_last; /*значение после последнего*/
} GRID_ParamT;


/* Дескриптор параметра */
#define GRID_ParamHandleT GRID_ParamT*

/* Неинициализированное значение параметра */
#define GRID_ParamHandleInvalid NULL

/* Функция, создающая параметр. Возвращает дескриптор. */
extern GRID_ParamHandleT GRID_ParamCreate(
    GRID_ParamDataT data,
    GRID_ParamChangeFuncT f_next,
    GRID_ParamChangeFuncT f_init,
    GRID_ParamBOOLFuncT f_is_after_last
);

/* Функция, уничтожающая параметр с заданным дескриптором. Освобождает принадлежащую ему память. */
extern void GRID_ParamDestroy(GRID_ParamHandleT param_handle);


typedef struct {
    GRID_ParamHandleT *param_list; /*список параметров*/
    int size; /*количество параметров (размерность решётки)*/
    BOOL is_after_last; /*значение после последнего*/
} GRID_BasicT;

/* Дескриптор решётки */
#define GRID_HandleT GRID_BasicT*

/* Неинициализированное значение дескриптора решётки */
#define GRID_HandleInvalid NULL

/* Функция, создающая решётку с заданным количеством размерностей. Возвращает дескриптор. */
extern GRID_HandleT GRID_Create(int param_number);

/* Функция, уничтожающая решётку с заданным дескриптором. Освобождает принадлежащую ей память. */
extern void GRID_Destroy(GRID_HandleT grid_handle);

/* Функция, инициализирующая указанный параметр решётки заданным значением. */
extern void GRID_SetParam(
    GRID_HandleT grid_handle,
    int param_idx,
    GRID_ParamHandleT param_handle
);

/* Функция, определяющая, является ли состояние указанной решётки следующим за конечным. */
extern BOOL GRID_IsAfterLast(GRID_HandleT grid_handle);

/* Функция, переводящая состояние решётки на следующее. */
extern void GRID_Next(GRID_HandleT grid_handle);

/* Функция, возвращающая дескриптор на параметр заданной решётки. */
extern GRID_ParamDataT GRID_GetParam(GRID_HandleT grid_handle, int param_idx);


#endif
