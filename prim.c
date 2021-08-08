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

cell_t* prim_car(mem_t* mem, cell_t* args)
{
    cell_t* arg = eval(mem, car(args));
    return car(arg);
}

cell_t* prim_cdr(mem_t* mem, cell_t* args)
{
    cell_t* arg = eval(mem, car(args));
    return cdr(arg);
}

cell_t* prim_cons(mem_t* mem, cell_t* args)
{
    cell_t* a = eval(mem, car(args));
    cell_t* b = eval(mem, car(cdr(args)));
    
    return new_pair(mem, a, b); // TODO why b not printed ?
}

cell_t* prim_quote(mem_t* mem, cell_t* args)
{ // TODO error handling
    return car(args);
}

cell_t* prim_cond(mem_t* mem, cell_t* args)
{ // TODO error handling
    while(not nullp(args))
    {
	cell_t* cond = car(args);
	cell_t* test = eval(mem, car(cond));
	if(not nullp(test))
	    return eval(mem, car(cdr(cond)));
	
	args = cdr(args);
    }

    return mem->nil;
}


cell_t* bool_to_cell(mem_t* mem, bool b)
{
    return b ? new_sym(mem, "t") : mem->nil;
}
