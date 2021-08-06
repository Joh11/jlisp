#include "lisp.h"

type_t cell_type(const cell_t* cell)
{
    int tag = CAST(val_t, cell) & 0b1111;
    switch(tag){
    case 0b0000:
	return NUM;
    case 0b0001:
	return SYM;
    case 0b0010:
	return PAIR;
    case 0b0011:
	return NIL;
    }
}

int get_num(const cell_t* cell)
{
    return CAST(int, UNTAG(cell)->cdr) >> 4;
}

bool atomp(const cell_t* cell)
{
    return !(CAST(num_t, cell) & 0b10);
}

bool nullp(const cell_t* cell)
{
    return CAST(val_t, cell) & 0b11 != 0;
}

cell_t* car(const cell_t* cell)
{
    return CAST(cell_t*, UNTAG(cell)->car);
}

cell_t* cdr(const cell_t* cell)
{
    return CAST(cell_t*, UNTAG(cell)->cdr);
}
