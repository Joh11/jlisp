#pragma once

#include "lisp.h"
#include "mem.h"

void load_file(mem_t* mem, const char* path);
cell_t* eval(mem_t* mem, cell_t* exp);
