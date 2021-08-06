#pragma once

#include "lisp.h"

struct sym_list_struct;
typedef struct sym_list_struct
{
    char* name;
    cell_t* cell;
    struct sym_list_struct* next;
    
} sym_list_t;

typedef struct
{
    cell_t* cells;
    cell_t* free;
    
    cell_t* nil;
    cell_t* unbound;

    sym_list_t* syms;
} mem_t;

mem_t init_mem();
void free_mem(mem_t* mem);

cell_t* new_num(mem_t* mem, int num);
cell_t* new_sym(mem_t* mem, const char* name);
cell_t* new_pair(mem_t* mem, cell_t* car, cell_t* cdr);

cell_t* find_symbol(const mem_t* mem, const char* name);
void add_symbol(mem_t* mem, const char* name, cell_t* cell);
