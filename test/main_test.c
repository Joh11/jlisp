#include <stdlib.h>
#include <stdio.h>

#include "test.h"

size_t test_count = 0;

void test_parse()
{
    TEST_ASSERT("trivial test", 1);
}

int main()
{
    test_parse();
    
    printf("all %ld tests passed\n", test_count);
    return 0;
}
