#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prim.h"

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
    // symbols bound to nothing will point to this
    cell_t* unbound = TAG_NIL(cells + 1);

    size_t beg = 2; // number of already reserved cells
    
    // construct the free list
    // free points on the first free cell after nil and unbound
    cell_t* free = TAG_PAIR(cells + beg); 
    cells[beg] = (cell_t){.car=CAST(val_t, nil), .cdr=CAST(val_t, TAG_PAIR(cells + beg))};
    for(size_t n = beg ; n < ncells - 1; ++n)
	cells[n] = (cell_t){.car=CAST(val_t, nil), .cdr=CAST(val_t, TAG_PAIR(cells + n + 1))};
    cells[ncells - 1] = (cell_t){.car=CAST(val_t, nil), .cdr=CAST(val_t, nil)};


    // construct the symbol list
    sym_list_t* syms = malloc(sizeof(sym_list_t));
    debug("sym: %p", syms);
    syms->name = malloc(4);
    debug("name: %p", syms->name);
    const char nil_str[] = "nil";
    strcpy(syms->name, nil_str);
    syms->cell = nil;
    syms->next = NULL;
    
    mem_t mem = (mem_t){.cells=cells, .free=free, .nil=nil, .unbound=unbound, .syms=syms};

    // load primitives
    cell_t* prim_sym = new_sym(&mem, "quote");
    UNTAG(prim_sym)->cdr = CAST(val_t, new_prim(&mem, &prim_quote));
    
    return mem;
}

void free_mem(mem_t* mem)
{
    free(mem->cells);

    // free the symbol list
    sym_list_t* sym = mem->syms;
    while(sym != NULL)
    {
	free(sym->name);
	free(sym);
	sym = sym->next;
    }
    
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
    // try to find it first
    cell_t* cell = find_symbol(mem, name);
    if(cell != NULL) return cell;

    // else make a new one
    /* cell_t*  */cell = allocate_cell(mem);

    size_t len = strlen(name) + 1;
    size_t nsym_cells = len / 8;
    size_t rem_chars = len % 8;

    cell_t* cur = allocate_cell(mem);
    cell->car = CAST(val_t, TAG_PAIR(cur));
    cell->cdr = CAST(val_t, mem->unbound);

    // fill all the filled pairs
    for(size_t n = 0 ; n < nsym_cells ; ++n)
    {
	cur->car = *CAST(val_t*, name + 8*n);
	cell_t* next = allocate_cell(mem);
	cur->cdr = CAST(val_t, TAG_PAIR(next));
	cur = next;
    }

    cur->car = 0;
    memcpy(&cur->car, name + 8 * nsym_cells, rem_chars);
    cur->cdr = CAST(val_t, mem->nil);

    cell = TAG_SYM(cell);
    // add the symbol to the symbol table
    add_symbol(mem, name, cell);
    
    return cell;
}

cell_t* new_pair(mem_t* mem, cell_t* car, cell_t* cdr)
{
    cell_t* cell = allocate_cell(mem);
    cell->car = CAST(val_t, car);
    cell->cdr = CAST(val_t, cdr);

    return TAG_PAIR(cell);
}

cell_t* new_prim(mem_t* mem, prim_t* prim)
{
    // car: function pointer
    cell_t* cell = allocate_cell(mem);
    cell->car = CAST(val_t, prim);
    return TAG_PRIMITIVE(cell);
}

static cell_t* allocate_cell(mem_t* mem)
{
    if(nullp(mem->free))
	error("garbage collector not yet implemented");

    cell_t* ret = UNTAG(mem->free);
    mem->free = cdr(mem->free);
    return ret;
}

cell_t* find_symbol(const mem_t* mem, const char* name)
{
    sym_list_t* sym = mem->syms;
    while(sym != NULL)
    {
	if(streq(name, sym->name))
	    return sym->cell;

	sym = sym->next;
    }
    return NULL;
}

void add_symbol(mem_t* mem, const char* name, cell_t* cell)
{
    sym_list_t* sym_list = malloc(sizeof(sym_list_t));
    size_t len = 1 + strlen(name);
    sym_list->name = malloc(len);
    strcpy(sym_list->name, name);
    sym_list->cell = cell;
    sym_list->next = mem->syms;

    mem->syms = sym_list;
}

/* char* symbol_name(cell_t* sym) */
/* { */
/*     // first pass to get the size */
/*     size_t len = 0; */
/*     cell_t cell = cdr(sym); */
/*     while(not nullp(cell)) */
/*     { */
/* 	len += 8; */
/* 	cell = cdr(cell); */
/*     } */

/*     char* name = malloc(len); */

/*     return name; */
/* } */
