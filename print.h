#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "lisp.h"

void fprint_sexp(FILE* stream, const cell_t* cell);
void print_sexp(const cell_t* cell);

