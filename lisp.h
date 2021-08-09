#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <iso646.h>

#define CAST(type, val) ((type)(val))
#define PACKED __attribute__((__packed__))

typedef enum { false, true } PACKED bool;

#define error(...) (fprintf(stderr, __VA_ARGS__), \
		    fprintf(stderr, "\n"),	  \
		    exit(1))

#define debug(...) (fprintf(stderr, "[debug] %s:%d ", __FILE__, __LINE__), \
		    fprintf(stderr, __VA_ARGS__),			\
		    fprintf(stderr, "\n"))


#define streq(a, b) (strcmp(a, b) == 0)

typedef uint64_t val_t;
typedef uint64_t num_t;

typedef struct
{
    val_t car;
    val_t cdr;
} PACKED cell_t;

typedef enum
{
    NUM, SYM, PAIR, NIL, PRIMITIVE, LAMBDA, MACRO
} type_t;

#define UNTAG(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) & ~CAST(val_t, 0b1111))

#define TAG_NUM(cell_ptr) (cell_ptr)
#define TAG_SYM(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) | 0b01)
#define TAG_PAIR(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) | 0b10)
#define TAG_NIL(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) | 0b11)
#define TAG_PRIMITIVE(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) | 0b100)
#define TAG_LAMBDA(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) | 0b101)
#define TAG_MACRO(cell_ptr) CAST(cell_t*, CAST(val_t, cell_ptr) | 0b110)

type_t cell_type(const cell_t* cell);
const char* cell_type_string(type_t type);

int get_num(const cell_t* cell);

bool atomp(const cell_t* cell);
bool nullp(const cell_t* cell);

cell_t* car(const cell_t* cell);
cell_t* cdr(const cell_t* cell);

size_t list_len(const cell_t* cell);
