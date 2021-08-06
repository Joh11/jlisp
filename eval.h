#pragma once

#include "lisp.h"
#include "mem.h"

cell_t* eval(mem_t* mem, cell_t* exp);
