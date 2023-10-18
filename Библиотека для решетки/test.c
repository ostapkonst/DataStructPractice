#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"

const int int_min = 0;
const int int_max = 29;
const int int_count = 30;

const char char_min = 'A';
const char char_max = 'Z';
const int char_count = 26;

const char text_min = 'A';
const char text_max = 'C';
const int text_count = 26;

void int_init(GRID_ParamDataT data){
    *(int*)data = int_min;
}
void int_next(GRID_ParamDataT data){
    (*(int*)data)++;
}
BOOL int_is_after_last(GRID_ParamDataT data){
    return *(int*)data > int_max;
}

void char_init(GRID_ParamDataT data){
    ((char*)data)[0] = char_min;
    ((char*)data)[1] = 0;
}
void char_next(GRID_ParamDataT data){
    ((char*)data)[0]++;
}
BOOL char_is_after_last(GRID_ParamDataT data){
    return ((char*)data)[0] > char_max;
}

void text_init(GRID_ParamDataT data){
    ((char*)data)[0] = text_min;
    ((char*)data)[1] = text_min;
    ((char*)data)[2] = text_min;
    ((char*)data)[3] = 0;
}
void text_next(GRID_ParamDataT data){
    int i;
    for (i = 2; i >= 0; i--) {
        ((char*)data)[i]++;
        if (((char*)data)[i] > text_max) {
            ((char*)data)[i] = text_min;
        } else {
            break;
        }
    }
}
BOOL text_is_after_last(GRID_ParamDataT data){
    return  ((char*)data)[0] == text_max &&
            ((char*)data)[1] == text_max &&
            ((char*)data)[2] == text_max;
}

int Test_1() {
    int count = 0;
    GRID_HandleT grid = GRID_Create(4);

    GRID_SetParam(grid,
                  0,
                  GRID_ParamCreate(malloc(sizeof(int)),
                  int_next,
                  int_init,
                  int_is_after_last)
    );

    GRID_SetParam(grid,
                  1,
                  GRID_ParamCreate(malloc(sizeof(int)),
                  int_next,
                  int_init,
                  int_is_after_last)
    );

    GRID_SetParam(grid,
                  2,
                  GRID_ParamCreate(malloc(sizeof(char)*2),
                  char_next,
                  char_init,
                  char_is_after_last)
    );

    GRID_SetParam(grid,
                  3,
                  GRID_ParamCreate(malloc(sizeof(char)*4),
                  text_next,
                  text_init,
                  text_is_after_last)
    );

    while (!GRID_IsAfterLast(grid)) {
        /*printf("%d, %d, %s, %s\n",
                *(int*)GRID_GetParam(grid, 0),
                *(int*)GRID_GetParam(grid, 1),
                (char*)GRID_GetParam(grid, 2),
                (char*)GRID_GetParam(grid, 3)
        );*/

        GRID_Next(grid);
        count++;
    }

    GRID_Destroy(grid);

    return count == int_count * int_count * char_count *text_count;
}

int Test_2(){
    int i,
        count = 0,
        real = 1,
        params = rand() % 5 + 1;

    GRID_ParamHandleT p;
    GRID_HandleT grid = GRID_Create(params);

    for (i = 0; i < params; i++) {
        switch (rand() % 4) {
            case 0 :
                p = GRID_ParamCreate(malloc(sizeof(char) * 2),
                                     char_next,
                                     char_init,
                                     char_is_after_last);
                real *= char_count;
                GRID_SetParam(grid, i, p);
                break;
            case 1 :
                p = GRID_ParamCreate(malloc(sizeof(int)),
                                     int_next,
                                     int_init,
                                     int_is_after_last);
                real *= int_count;
                GRID_SetParam(grid, i, p);
                break;
            case 2 :
                p = GRID_ParamCreate(malloc(sizeof(int) * 4),
                                     text_next,
                                     text_init,
                                     text_is_after_last);
                real *= text_count;
                GRID_SetParam(grid, i, p);
                break;
        }
    }

    while (!GRID_IsAfterLast(grid)){
        GRID_Next(grid);
        count++;
    }
    GRID_Destroy(grid);

    return count == real || real == 1;
}

int main() {
    printf("TEST 1 ");
    printf(Test_1() ? "OK\n" : "ERROR\n");
    printf("TEST 2 ");
    printf(Test_2() ? "OK\n" : "ERROR\n");
    return 0;
}