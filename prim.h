#pragma once

#include "lisp.h"
#include "mem.h"

// atom eq car cdr cons quote cond lambda define set

cell_t* prim_quote(mem_t* mem, cell_t* args);
