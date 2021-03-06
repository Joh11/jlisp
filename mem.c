#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "prim.h"

#include "mem.h"

// private functions
static cell_t* allocate_cell(mem_t* mem);
static void add_primitive(mem_t* mem, const char* name, prim_t* prim);

mem_t init_mem()
{
    if(sizeof(val_t) != sizeof(cell_t*))
	error("pointers are not 64 bits. ");
    
    size_t ncells = 2048; assert(ncells % 8 == 0);
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
    syms->name = malloc(4);
    const char nil_str[] = "nil";
    strcpy(syms->name, nil_str);
    syms->cell = nil;
    syms->next = NULL;
    
    mem_t mem = (mem_t){.ncells=ncells, .cells=cells,
	.free=free, .nil=nil, .unbound=unbound, .and_rest=NULL, .gvar=NULL,
	.syms=syms, .stack=NULL, .marks=NULL, .global_vars=nil};

    // set the marks array for GC
    mem.marks = malloc(ncells / 8);
    memset(mem.marks, 0, ncells / 8);
    
    // add &rest, the symbol to allow variadic arguments in lambdas
    // and macros
    mem.and_rest = new_sym(&mem, "&rest");
    // TODO don't garbage collect it

    mem.gvar = new_sym(&mem, "&gvars");
    // TODO don't garbage collect it
    
    // load primitives
    add_primitive(&mem, "quote", &prim_quote);
    add_primitive(&mem, "atom?", &prim_atom);
    add_primitive(&mem, "eq", &prim_eq);
    add_primitive(&mem, "car", &prim_car);
    add_primitive(&mem, "cdr", &prim_cdr);
    add_primitive(&mem, "cons", &prim_cons);
    add_primitive(&mem, "cond", &prim_cond);
    add_primitive(&mem, "lambda", &prim_lambda);
    add_primitive(&mem, "define", &prim_define);
    add_primitive(&mem, "set", &prim_set);
    add_primitive(&mem, "macro", &prim_macro);

    add_primitive(&mem, "+", &prim_plus);
    add_primitive(&mem, "-", &prim_minus);
    add_primitive(&mem, "*", &prim_times);
    add_primitive(&mem, "/", &prim_divide);
    add_primitive(&mem, "%", &prim_mod);

    add_primitive(&mem, "print", &prim_print);
    add_primitive(&mem, "exit", &prim_exit);
    add_primitive(&mem, "list?", &prim_listp);
    add_primitive(&mem, "or", &prim_or);
    add_primitive(&mem, "and", &prim_and);
    
    return mem;
}

void free_mem(mem_t* mem)
{
    free(mem->cells);
    free(mem->marks);
    
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

void maybe_garbage_collect(mem_t* mem)
{
    if(nfree_cells(mem) < 30)
    {
	debug("garbage collecting ...");
	garbage_collect(mem);
    }
}

void garbage_collect(mem_t* mem)
{
    debug("garbage collecting ...");

    // what counts as a root:
    // global variables
    // stack values

    // what is persistent (= not collected no matter what)
    // nil, unbound and &rest

    // 1. mark
    mark(mem, mem->global_vars); // global vars
    // stack values
    stack_t* stack = mem->stack;
    while(stack)
    {
	for(size_t n = 0 ; n < stack->n ; ++n)
	{
	    mark(mem, stack->names[n]);
	    mark(mem, stack->values[n]);
	}
	
	stack = stack->next;
    }

    // mark the persistent values
    mark(mem, mem->nil);
    mark(mem, mem->unbound);
    mark(mem, mem->gvar);
    mark(mem, mem->and_rest);

    // 2. sweep
    sweep(mem);

    // TODO remove uninterned symbols from the symbol table
    remove_uninterned_symbols(mem);
    
    // 3. unmark
    memset(mem->marks, 0, mem->ncells / 8);
}

void remove_uninterned_symbols(mem_t* mem)
{
    // TODO
}

void mark(mem_t* mem, cell_t* cell)
{
    // get the index in the cells array
    size_t idx = UNTAG(cell) - mem->cells;
    if(markp(mem, idx)) return;

    // mark the cell
    setmark(mem, idx);

    // mark its children
    switch(cell_type(cell))
    {
    case SYM:
	// mark value if bound
	if(not (UNTAG(cell)->cdr == CAST(val_t, mem->unbound)))
	    mark(mem, CAST(cell_t*, UNTAG(cell)->cdr));
	// mark symbol name
	cell_t* pair = car(cell);
	while(pair != mem->nil)
	{
	    setmark(mem, UNTAG(pair) - mem->cells);
	    pair = cdr(pair);
	}
	break;
    case PAIR:
	if(not nullp(CAST(cell_t*, UNTAG(cell)->car)))
	    mark(mem, CAST(cell_t*, UNTAG(cell)->car));
	if(not nullp(CAST(cell_t*, UNTAG(cell)->cdr)))
	    mark(mem, CAST(cell_t*, UNTAG(cell)->cdr));
	break;
    case LAMBDA:
    case MACRO:
	mark(mem, car(cell));
	break;
    default: // don't do anything
	break;
    }
}

bool markp(mem_t* mem, size_t idx)
{
    return mem->marks[idx / 8] >> (idx % 8) & 0b1;
}

bool setmark(mem_t* mem, size_t idx)
{
    mem->marks[idx / 8] |= 0b1 << (idx % 8);
}

bool occupiedp(const mem_t* mem, const cell_t* cell)
{ // TODO this is quadratic in the number of free cells, nothing
  // better ?
    // don't think so, and anyway the number of free cells on a
    // garbage collector call should be small anyway.
    cell = UNTAG(cell);
    cell_t* free = mem->free;
    while(free != mem->nil)
    {
	if(UNTAG(free) == cell)
	    return false;
	free = cdr(free);
    }
    return true;
}

void sweep(mem_t* mem)
{
    size_t count = 0;
    for(size_t n = 0 ; n < mem->ncells ; ++n)
    {
	if(not markp(mem, n) and occupiedp(mem, mem->cells+n)) // free it !
	{
	    ++count;
	    mem->cells[n] = (cell_t){.car=CAST(val_t, mem->nil),
		.cdr=CAST(val_t, mem->free)};
	    mem->free = TAG_PAIR(mem->cells + n);
	}
    }
    debug("freed %ld cells. ", count);
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
    cell = allocate_cell(mem);

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

cell_t* new_lambda(mem_t* mem, cell_t* args_body)
{
    // car: (args ...body) list
    cell_t* cell = allocate_cell(mem);
    cell->car = CAST(val_t, args_body);
    return TAG_LAMBDA(cell);
}

cell_t* new_macro(mem_t* mem, cell_t* args_body)
{
    // same as lambda
    cell_t* cell = allocate_cell(mem);
    cell->car = CAST(val_t, args_body);
    return TAG_MACRO(cell);
}

static cell_t* allocate_cell(mem_t* mem)
{
    if(nullp(mem->free))
	error("no more cells");

    cell_t* ret = UNTAG(mem->free);
    mem->free = cdr(mem->free);
    return ret;
}

static void add_primitive(mem_t* mem, const char* name, prim_t* prim)
{
    cell_t* prim_sym = new_sym(mem, name);
    UNTAG(prim_sym)->cdr = CAST(val_t, new_prim(mem, prim));
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

void mem_stack_push_params(mem_t* mem, cell_t* names)
{
    size_t n = list_len(names);
    stack_t* new = malloc(sizeof(stack_t));

    new->names = malloc(sizeof(cell_t*) * n);
    new->values = malloc(sizeof(cell_t*) * n);
    new->n = n;
    new->next = mem->stack;

    mem->stack = new;

    // fill the name -> value pairs
    for(size_t i = 0 ; i < n ; ++i)
    {
	new->names[i] = car(names);
	new->values[i] = cdr(car(names));
	names = cdr(names);
    }
}

void mem_stack_pop_params(mem_t* mem)
{
    for(size_t i = 0 ; i < mem->stack->n ; ++i)
	UNTAG(mem->stack->names[i])->cdr = CAST(val_t, mem->stack->values[i]);

    free(mem->stack->names);
    free(mem->stack->values);
    stack_t* new = mem->stack->next;
    free(mem->stack);

    mem->stack = new;
}

size_t nfree_cells(const mem_t* mem)
{
    size_t count = 0;
    cell_t* next = mem->free;
    while(next != mem->nil)
    {
	++count;
	next = cdr(next);
    }
    return count;
}
