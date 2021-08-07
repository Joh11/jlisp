#include "eval.h"

#include "prim.h"

cell_t* prim_atom(mem_t* mem, cell_t* args)
{
    cell_t* arg = eval(mem, car(args));
    return bool_to_cell(mem, atomp(arg));
}

cell_t* prim_eq(mem_t* mem, cell_t* args)
{
    cell_t* a = eval(mem, car(args));
    cell_t* b = eval(mem, car(cdr(args)));

    if(cell_type(a) == NUM and cell_type(b) == NUM)
	return bool_to_cell(mem, get_num(a) == get_num(b));
    
    return bool_to_cell(mem, a == b);
}

cell_t* prim_quote(mem_t* mem, cell_t* args)
{ // TODO error handling
    return car(args);
}



cell_t* bool_to_cell(mem_t* mem, bool b)
{
    return b ? new_sym(mem, "t") : mem->nil;
}
