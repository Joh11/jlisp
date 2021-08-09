#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lisp.h"
#include "mem.h"
#include "print.h"
#include "parser.h"
#include "eval.h"

void load_file(mem_t* mem, const char* path);

void free_cells(const mem_t* mem)
{
    size_t count = 0;
    cell_t* next = mem->free;
    while(next != mem->nil)
    {
	debug("one more");
	++count;
	next = cdr(next);
    }
    debug("There are %ld used cells", mem->ncells - count);
}

int main(int argc, char *argv[])
{
    mem_t mem = init_mem();

    load_file(&mem, "init.l");

    if(argc > 1)
    {
	for(size_t n = 1 ; n < argc ; ++n)
	{
	    printf("loading %s... \n", argv[n]);
	    load_file(&mem, argv[n]);
	}
    }

    while(true)
    {
	printf("> ");
	cell_t* sexp = parse_one(&mem, stdin, NULL);
	print_sexp(sexp);
	printf(" => ");
	print_sexp(eval(&mem, sexp));
	printf("\n");
    }
    
    free_mem(&mem);
    return 0;
}

void load_file(mem_t* mem, const char* path)
{
    FILE* f = fopen(path, "r");
    cell_t* cell = NULL;

    while(cell = parse_one(mem, f, NULL))
	eval(mem, cell);
    
    fclose(f);
}
