#include "prim.h"

cell_t* prim_quote(mem_t* mem, cell_t* args)
{ // TODO error handling
    return car(args);
}
