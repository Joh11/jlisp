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
    case 0b0100:
	return PRIMITIVE;
    case 0b0101:
	return LAMBDA;
    case 0b0110:
	return MACRO;
    }
}

const char* cell_type_string(type_t type)
{
    static const char* names[] =
	{"NUM", "SYM", "PAIR", "NIL", "PRIMITIVE", "LAMBDA", "MACRO"};
    return names[type];
}

int get_num(const cell_t* cell)
{
    return CAST(int, UNTAG(cell)->cdr) >> 4;
}

bool atomp(const cell_t* cell)
{
    return cell_type(cell) != PAIR;
}

bool nullp(const cell_t* cell)
{
    return cell_type(cell) == NIL;
}

cell_t* car(const cell_t* cell)
{
    return CAST(cell_t*, UNTAG(cell)->car);
}

cell_t* cdr(const cell_t* cell)
{
    return CAST(cell_t*, UNTAG(cell)->cdr);
}

size_t list_len(const cell_t* cell)
{ // TODO error handling
    size_t n = 0;
    while(not nullp(cell))
    {
	++n;
	cell = cdr(cell);
    }

    return n;
}
