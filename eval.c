#include <stdlib.h>
#include <stdio.h>

#include "print.h"

#include "eval.h"

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
	error("not yet implemented");
	break;
    case NIL:
	return exp;
    }
}
