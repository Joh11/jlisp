#include <stdlib.h>
#include <stdio.h>

#include "print.h"

#include "eval.h"

static cell_t* call_dispatch(mem_t* mem, cell_t* callable, cell_t* args);

cell_t* eval(mem_t* mem, cell_t* exp)
{
    switch(cell_type(exp))
    {
    case NUM:
	return exp;
    case SYM:
	if(cdr(exp) == mem->unbound)
	{
	    printf("undefined variable: ");
	    print_sexp(exp);
	    error("fatal error. ");
	}
	return cdr(exp);
    case PAIR:
	return call_dispatch(mem, car(exp), cdr(exp));
    case NIL:
	return exp;
    }
}

static cell_t* get_callable(cell_t* exp)
{
    type_t t = cell_type(exp);

    if(t == SYM)
	return car(exp);
    if(t == LAMBDA)
	return exp;
    
    error("not callable");
}

static cell_t* call_dispatch(mem_t* mem, cell_t* callable, cell_t* args)
{
    if(cell_type(callable) == SYM)
    {
	callable = cdr(callable);
	if(callable == mem->unbound)
	    error("function not found");
    }

    switch(cell_type(callable))
    {
    case PRIMITIVE:
    {
	prim_t* fun = CAST(prim_t*, UNTAG(callable)->car);
	return fun(mem, args);
    }
    case LAMBDA:
	return mem->nil; // TODO
    default:
	error("expected a callable");
	break;
    }
}
