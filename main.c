#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lisp.h"
#include "mem.h"
#include "print.h"
#include "parser.h"
#include "eval.h"

void load_argv_files(mem_t* mem, int argc, char* argv[]);

int main(int argc, char *argv[])
{
    mem_t mem = init_mem();

    load_file(&mem, "init.l");
    load_argv_files(&mem, argc, argv);

    cell_t* c = parse_from_string(&mem, "((lambda (x) (+ 1 x)) 1)");
    // cell_t* c = parse_from_string(&mem, "((lambda (x) x) 1)");
    print_sexp(c);
    printf(" => ");
    print_sexp(eval(&mem, c));
    printf("\n");
    
    while(true)
    {
	printf("> ");
	cell_t* sexp = parse_one(&mem, stdin, NULL);
	if(sexp == NULL) // likely that EOF or error happened
	{
	    const char* msg = parse_error();
	    if(msg != NULL)
		error("parse error: %s", msg);
	    break;
	}
	
	print_sexp(sexp);
	printf(" => ");
	print_sexp(eval(&mem, sexp));
	printf("\n");

	maybe_garbage_collect(&mem);
    }
    
    free_mem(&mem);
    return 0;
}

void load_argv_files(mem_t* mem, int argc, char* argv[])
{
    for(size_t n = 1 ; n < argc ; ++n)
    {
	printf("loading %s... \n", argv[n]);
	load_file(mem, argv[n]);
    }
}
