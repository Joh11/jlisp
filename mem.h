#pragma once

#include "lisp.h"

typedef struct
{
    cell_t* cells;
    cell_t* free;
    cell_t* nil;
} mem_t;

mem_t init_mem();
void free_mem(mem_t* mem);

cell_t* new_num(mem_t* mem, int num);
cell_t* new_sym(mem_t* mem, const char* name);
cell_t* new_pair(mem_t* mem, cell_t* car, cell_t* cdr);
