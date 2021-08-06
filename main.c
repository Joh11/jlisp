#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lisp.h"
#include "mem.h"
#include "print.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    mem_t mem = init_mem();

    while(true)
    {
	cell_t* sexp = parse_one(&mem, stdin, NULL);
	print_sexp(sexp);
	printf("\n");
    }
    
    free_mem(&mem);
    return 0;
}
