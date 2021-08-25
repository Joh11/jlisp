#include "print.h"

// private functions
static void fprint_sym(FILE* stream, const cell_t* cell);

void fprint_sexp(FILE* stream, const cell_t* cell)
{
    switch(cell_type(cell))
    {
    case NUM:
	fprintf(stream, "%d", get_num(cell));
	break;
    case SYM:
	fprint_sym(stream, cell);
	break;
    case PAIR:
	fprintf(stream, "(");
	while(true)
	{
	    fprint_sexp(stream, car(cell));
	    if(nullp(cdr(cell))) // 
	    {
		fprintf(stream, ")");
		break;
	    } // dotted list
	    else if(atomp(cdr(cell)))
	    {
		fprintf(stream, " . ");
		fprint_sexp(stream, cdr(cell));
		fprintf(stream, ")");
		break;
	    }
	    else
	    {
		fputc(' ', stream);
		cell = cdr(cell);
	    }
	    
	}
	break;
    case NIL:
	fprintf(stream, "nil");
	break;
    case PRIMITIVE:
	fprintf(stream, "<PRIMITIVE>");
	break;
    case LAMBDA:
	fprintf(stream, "<LAMBDA ");
	fprint_sexp(stream, car(car(cell)));
	fprintf(stream, ">");
	break;
    case MACRO:
	fprintf(stream, "<MACRO ");
	fprint_sexp(stream, car(car(cell)));
	fprintf(stream, ">");
	break;
    }
}

void print_sexp(const cell_t* cell)
{
    fprint_sexp(stdout, cell);
}

static void fprint_sym(FILE* stream, const cell_t* cell)
{
    cell = car(cell);
    bool cont = true;
    while(cont)
    {
	const char* c = CAST(const char*, &(UNTAG(cell)->car));
	for(size_t n = 0 ; n < 8 ; ++n)
	{
	    if(c[n] == '\0')
	    {
		cont = false;
		break;
	    }
	    fputc(c[n], stream);
	}
	cell = cdr(cell);
    }
}
