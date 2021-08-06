#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mem.h"

// private functions
static cell_t* allocate_cell(mem_t* mem);

mem_t init_mem()
{
    if(sizeof(val_t) != sizeof(cell_t*))
	error("pointers are not 64 bits. ");
    
    size_t ncells = 100;
    cell_t* cells = malloc(ncells * sizeof(cell_t));

    // construct nil
    cell_t* nil = TAG_NIL(cells);

    // construct the free list
    // free points on the first free cell after nil
    cell_t* free = TAG_PAIR(cells + 1); 
    cells[1] = (cell_t){.car=nil, .cdr=TAG_PAIR(cells + 2)};
    for(size_t n = 1 ; n < ncells - 1; ++n)
	cells[n] = (cell_t){.car=nil, .cdr=TAG_PAIR(cells + n + 1)};
    cells[ncells - 1] = (cell_t){.car=nil, .cdr=nil};
    
    return (mem_t){.cells=cells, .free=free, .nil=nil};
}

void free_mem(mem_t* mem)
{
    free(mem->cells);
    memset(mem, 0, sizeof(mem_t));
}

cell_t* new_num(mem_t* mem, int num)
{
    cell_t* cell = allocate_cell(mem);
    cell->cdr = num << 4;

    return TAG_NUM(cell);
}

cell_t* new_sym(mem_t* mem, const char* name)
{
    cell_t* cell = allocate_cell(mem);

    size_t len = strlen(name) + 1;
    size_t nsym_cells = len / 8;
    size_t rem_chars = len % 8;

    cell_t* cur = allocate_cell(mem);
    cell->car = TAG_PAIR(cur);

    // fill all the filled pairs
    for(size_t n = 0 ; n < nsym_cells ; ++n)
    {
	cur->car = *CAST(val_t*, name + 8*n);
	cell_t* next = allocate_cell(mem);
	cur->cdr = TAG_PAIR(next);
	cur = next;
    }

    cur->car = 0;
    memcpy(&cur->car, name + 8 * nsym_cells, rem_chars);
    cur->cdr = mem->nil;
    
    return TAG_SYM(cell);
}

cell_t* new_pair(mem_t* mem, cell_t* car, cell_t* cdr)
{
    cell_t* cell = allocate_cell(mem);
    cell->car = car;
    cell->cdr = cdr;

    return TAG_PAIR(cell);
}

static cell_t* allocate_cell(mem_t* mem)
{
    if(nullp(&mem->free))
	error("garbage collector not yet implemented");

    cell_t* ret = UNTAG(mem->free);
    mem->free = cdr(mem->free);
    return ret;
}
