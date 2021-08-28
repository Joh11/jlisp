#include <stdio.h>

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

cell_t* prim_lambda(mem_t* mem, cell_t* args)
{
    return new_lambda(mem, args);
}

cell_t* prim_define(mem_t* mem, cell_t* args)
{
    cell_t* name = eval(mem, car(args));
    cell_t* val = eval(mem, car(cdr(args)));

    // push name to the global vars list
    mem->global_vars = new_pair(mem, name, mem->global_vars);
    
    UNTAG(name)->cdr = CAST(val_t, val);
    
    return name;
}

cell_t* prim_set(mem_t* mem, cell_t* args)
{
    cell_t* name = eval(mem, car(args));
    cell_t* val = eval(mem, car(cdr(args)));

    UNTAG(name)->cdr = CAST(val_t, val);
    
    return val;
}

cell_t* prim_macro(mem_t* mem, cell_t* args)
{
    return new_macro(mem, args);
}

cell_t* prim_plus(mem_t* mem, cell_t* args)
{
    val_t ret = 0;
    while(args != mem->nil)
    {
	ret += get_num(eval(mem, car(args)));
	args = cdr(args);
    }
    return new_num(mem, ret);
}

cell_t* prim_minus(mem_t* mem, cell_t* args)
{
    val_t ret = get_num(car(args));
    args = cdr(args);
    while(args != mem->nil)
    {
	ret -= get_num(eval(mem, car(args)));
	args = cdr(args);
    }
    return new_num(mem, ret);
}

cell_t* prim_times(mem_t* mem, cell_t* args)
{
    val_t ret = 1;
    while(args != mem->nil)
    {
	ret *= get_num(eval(mem, car(args)));
	args = cdr(args);
    }
    return new_num(mem, ret);
}

cell_t* prim_divide(mem_t* mem, cell_t* args)
{
    val_t ret = get_num(eval(mem, car(args)));
    args = cdr(args);
    while(args != mem->nil)
    {
	val_t n = get_num(eval(mem, car(args)));
	if(n == 0)
	    error("division by zero");
	ret /= n;
	args = cdr(args);
    }
    return new_num(mem, ret);
}
    
cell_t* prim_mod(mem_t* mem, cell_t* args)
{
    val_t a = get_num(eval(mem, car(args)));
    val_t b = get_num(eval(mem, car(cdr(args))));

    return new_num(mem, a % b);
}

cell_t* bool_to_cell(mem_t* mem, bool b)
{
    return b ? new_sym(mem, "t") : mem->nil;
}
