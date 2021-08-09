#pragma once

#include "lisp.h"
#include "mem.h"

// car cdr cons quote cond lambda define set

cell_t* prim_atom(mem_t* mem, cell_t* args);
cell_t* prim_eq(mem_t* mem, cell_t* args);
cell_t* prim_car(mem_t* mem, cell_t* args);
cell_t* prim_cdr(mem_t* mem, cell_t* args);
cell_t* prim_cons(mem_t* mem, cell_t* args);
cell_t* prim_quote(mem_t* mem, cell_t* args);
cell_t* prim_cond(mem_t* mem, cell_t* args);
cell_t* prim_lambda(mem_t* mem, cell_t* args);
cell_t* prim_define(mem_t* mem, cell_t* args);
cell_t* prim_set(mem_t* mem, cell_t* args);
cell_t* prim_macro(mem_t* mem, cell_t* args);

// other primitives
cell_t* prim_mod(mem_t* mem, cell_t* args);

cell_t* bool_to_cell(mem_t* mem, bool b);
