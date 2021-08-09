#pragma once

#include "lisp.h"

struct sym_list_struct;
typedef struct sym_list_struct
{
    char* name;
    cell_t* cell;
    struct sym_list_struct* next;
    
} sym_list_t;

struct stack_struct;
typedef struct stack_struct
{
    cell_t** names;
    cell_t** values;
    size_t n;

    struct stack_struct* next;
} stack_t;

typedef struct
{
    cell_t* cells;
    cell_t* free;
    
    cell_t* nil;
    cell_t* unbound;
    cell_t* and_rest;
    cell_t* gvar;

    sym_list_t* syms;

    stack_t* stack;

    cell_t* global_vars;
} mem_t;

typedef cell_t* prim_t(mem_t* mem, cell_t* args);

mem_t init_mem();
void free_mem(mem_t* mem);

void garbage_collect(mem_t* mem);
void mark(mem_t* mem, cell_t* cell);

cell_t* new_num(mem_t* mem, int num);
cell_t* new_sym(mem_t* mem, const char* name);
cell_t* new_pair(mem_t* mem, cell_t* car, cell_t* cdr);
cell_t* new_prim(mem_t* mem, prim_t* prim);
cell_t* new_lambda(mem_t* mem, cell_t* arg_body);
cell_t* new_macro(mem_t* mem, cell_t* arg_body);

cell_t* find_symbol(const mem_t* mem, const char* name);
void add_symbol(mem_t* mem, const char* name, cell_t* cell);

void mem_stack_push_params(mem_t* mem, cell_t* names);
void mem_stack_pop_params(mem_t* mem);
