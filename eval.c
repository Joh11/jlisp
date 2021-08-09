#include <stdlib.h>
#include <stdio.h>

#include "print.h"

#include "eval.h"

static cell_t* call_dispatch(mem_t* mem, cell_t* callable, cell_t* args);
static cell_t* call_lambda(mem_t* mem, cell_t* arg_names, cell_t* arg_vals, cell_t* body);

cell_t* eval(mem_t* mem, cell_t* exp)
{
    switch(cell_type(exp))
    {
    case NUM:
    case NIL:
    case PRIMITIVE:
    case LAMBDA:
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
    }
}

static cell_t* call_dispatch(mem_t* mem, cell_t* callable, cell_t* args)
{
    callable = eval(mem, callable);
    if(callable == mem->unbound)
	    error("function not found");
    
    switch(cell_type(callable))
    {
    case PRIMITIVE:
    {
	prim_t* fun = CAST(prim_t*, UNTAG(callable)->car);
	return fun(mem, args);
    }
    case LAMBDA:
	// a lambda has its car pointing to a
	// ((arg1 arg2 ...) (exp1 exp2 ...)) list
	return call_lambda(mem, car(car(callable)), args, (cdr(car(callable))));
    default:
	error("expected a callable");
	break;
    }
}

static cell_t* call_lambda(mem_t* mem, cell_t* arg_names, cell_t* arg_vals, cell_t* body)
{
    // push the initial value of the params
    mem_stack_push_params(mem, arg_names);
    
    // set params
    while(not nullp(arg_names))
    { // TODO error handling
	cell_t* name = car(arg_names);
	UNTAG(name)->cdr = CAST(val_t, eval(mem, car(arg_vals)));
	
	arg_names = cdr(arg_names);
	arg_vals = cdr(arg_vals);
    }

    // eval body
    cell_t* ret = mem->nil;
    while(not nullp(body))
    {
	ret = eval(mem, car(body));
	body = cdr(body);
    }

    // pop back the initial value of the params
    mem_stack_pop_params(mem);

    return ret;
}
