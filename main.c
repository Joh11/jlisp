#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lisp.h"
#include "mem.h"
#include "print.h"

int main(int argc, char *argv[])
{
    mem_t mem = init_mem();

    cell_t* bonjour = new_sym(&mem, "bonjour");
    assert(cell_type(bonjour) == SYM);
    print_sexp(bonjour);

    free_mem(&mem);
    return 0;
}
