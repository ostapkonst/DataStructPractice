#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

GRID_ParamHandleT GRID_ParamCreate(
    GRID_ParamDataT data,
    GRID_ParamChangeFuncT f_next,
    GRID_ParamChangeFuncT f_init,
    GRID_ParamBOOLFuncT f_is_after_last
) {
    GRID_ParamHandleT ans = (GRID_ParamT *)malloc(sizeof(GRID_ParamT));

    ans->data = data;
    ans->f_next = f_next;
    ans->f_init = f_init;
    ans->f_is_after_last = f_is_after_last;

    return ans;
}

void GRID_ParamDestroy(GRID_ParamHandleT param_handle){
    free(param_handle->data);
    free(param_handle);
    param_handle = GRID_ParamHandleInvalid;
}

GRID_HandleT GRID_Create(int param_number){
    GRID_HandleT ans = (GRID_BasicT *)malloc(sizeof(GRID_BasicT));
    ans->param_list = (GRID_ParamHandleT *)calloc(param_number, sizeof(GRID_ParamHandleT));

    ans->size = param_number;
    ans->is_after_last = 1;

    return ans;
}

void GRID_Destroy(GRID_HandleT grid_handle){
    int i;

    for (i = 0; i < grid_handle->size; i++) {
        if (grid_handle->param_list[i] != GRID_ParamHandleInvalid) {
            GRID_ParamDestroy(grid_handle->param_list[i]);
        }
    }

    free(grid_handle->param_list);
    free(grid_handle);
}

BOOL GRID_IsAfterLast(GRID_HandleT grid_handle){
    return grid_handle->is_after_last;
}

GRID_ParamDataT GRID_GetParam(GRID_HandleT grid_handle, int param_idx){
    return grid_handle->param_list[param_idx]->data;
}

void GRID_SetParam(
    GRID_HandleT grid_handle,
    int param_idx,
    GRID_ParamHandleT param_handle
){
    param_handle->f_init(param_handle->data);

    if (!param_handle->f_is_after_last(param_handle->data)) {
        grid_handle->is_after_last = 0;
    }

    grid_handle->param_list[param_idx] = param_handle;
}

void GRID_Next(GRID_HandleT grid_handle){
    int i;

    if (!grid_handle->is_after_last) {
        grid_handle->is_after_last = 1;
        for (i = grid_handle->size - 1; i >= 0; i--) {
            if (grid_handle->param_list[i] != GRID_ParamHandleInvalid) {
                grid_handle->param_list[i]->f_next(grid_handle->param_list[i]->data);
                if (grid_handle->param_list[i]->f_is_after_last(grid_handle->param_list[i]->data)) {
                    grid_handle->param_list[i]->f_init(grid_handle->param_list[i]->data);
                } else {
                    grid_handle->is_after_last = 0;
                    return;
                }
            }
        }
    }
}
