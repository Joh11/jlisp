#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../mem.h"
#include "../parser.h"

#include "test.h"

size_t test_count = 0;

void test_parse()
{
    mem_t mem = init_mem();
    TEST_ASSERT("trivial test", true);

    // try parsing a number
    char* num_str = "   42   ";
    FILE* s = fmemopen(num_str, strlen(num_str), "r");
    cell_t* cell = parse_one(&mem, s, NULL);
    TEST_ASSERT("parse number type", cell_type(cell) == NUM);
    TEST_ASSERT("parse number value", get_num(cell) == 42);
    
    free_mem(&mem);
}

int main()
{
    test_parse();
    
    printf("all %ld tests passed\n", test_count);
    return 0;
}
