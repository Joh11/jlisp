#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "lisp.h"
#include "mem.h"

// consume a single token from the given string
char* tokenize(FILE *f);

// try to parse a single sexp
// if failed return NULL
cell_t* parse_one(mem_t* mem, FILE* f, char* token);

// returns a description of the error, or NULL if no error happened
const char* parse_error();

// return TRUE if token is a single char string "c"
static bool token_char(const char* token, char c);

static cell_t* parse_number(mem_t* mem, char *token);
