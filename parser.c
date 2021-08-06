#include <ctype.h>
#include <string.h>

#include "parser.h"

#define PARSER_TOKEN_SIZE 1024 // TODO remove this hard limit
static char token[PARSER_TOKEN_SIZE];

char* tokenize(FILE *f)
{
    // remove whitespaces
    int c = fgetc(f);
    while(isspace(c))
	c = fgetc(f);
    if(c == EOF)
	return NULL;

    // shortcutting single char tokens
    switch(c)
    {
    case '(':
    case '.':
    case ')':
    case '\'':
	token[0] = c;
	token[1] = '\0';
	return token;
	break;
    default:
	break;
    }

    // number
    if(isdigit(c))
    {
	size_t k = 0;
	while(isdigit(c))
	{
	    token[k] = c;
	    k++;
	    c = fgetc(f);
	}
	token[k] = '\0';

	if(c != EOF) // put back the last char
	    ungetc(c, f);

	return token;
    }

    // symbol
    size_t k = 0;
    while(!isspace(c) && (c != EOF)
	  && (c != '(') && (c != ')') && (c != '.'))
    {
	token[k] = c;
	k++;
	c = fgetc(f);
    }
    token[k] = '\0';
    
    if(c != EOF) // put back the last char
	ungetc(c, f);
    
    return token;
}

cell_t* parse_one(mem_t* mem, FILE* f, char* token)
{
    // just a trick to be able to cancel the consumption of a token
    if(token == NULL)
	token = tokenize(f);

    if(token == NULL) // EOF happened
	return NULL;

    if(isdigit(token[0]))
	return parse_number(mem, token);
    else if(token_char(token, '('))
    {
	// first check for nil
	token = tokenize(f);
	if(token == NULL) return NULL;
	if(token_char(token, ')'))
	    return mem->nil;

	// if not ) then we are sure it is at least a cons
	cell_t* root = new_pair(mem, mem->nil, mem->nil);
	if(token_char(token, '.'))
	    return NULL; // there cannot be a . in first position
	// parse the car
	cell_t* car = parse_one(mem, f, token);
	if(car == NULL) return NULL;
	UNTAG(root)->car = CAST(val_t, car);
	
	cell_t* last_cons = root;
	while(true)
	{
	    token = tokenize(f);
	    if(token == NULL) return NULL;
	    
	    if(token_char(token, ')'))
	    {
		// in this case end the parsing
		return root;
	    }

	    if(token_char(token, '.'))
	    {
		// parse the cdr and end
		cell_t* cdr = parse_one(mem, f, token);
		if(cdr == NULL) return NULL;
		UNTAG(last_cons)->cdr = CAST(val_t, cdr);
		return root;
	    }

	    // else it is just another sexp to parse
	    car = parse_one(mem, f, token);
	    if(car == NULL) return NULL;
	    
	    UNTAG(last_cons)->cdr = CAST(val_t, new_pair(mem, car, mem->nil));
	    last_cons = CAST(cell_t*, UNTAG(last_cons)->cdr);
	}
    }
    else if(token_char(token, '\''))
    {
	// make a new quote cons before reading the inside
	cell_t* next = parse_one(mem, f, NULL);
	if(next == NULL) return NULL;

	// TODO put a proper quote symbol
	return new_pair(mem, mem->nil, next);
    }
    else if(token_char(token, ')'))
	return NULL; // unexpected )
    else // in this case it is a symbol
	return new_sym(mem, token);
}

static bool token_char(const char* token, char c)
{
    return (strlen(token) == 1) && (token[0] == c);
}

static cell_t* parse_number(mem_t* mem, char* token)
{
    char* check = token;
    int num = strtol(token, &check, 10);
    if(check == token)
	return NULL;

    return new_num(mem, num);
}
