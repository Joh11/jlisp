#pragma once

extern size_t test_count;

#define TEST_ASSERT(message, val) do {		\
	printf("testing %s ... ", message);	\
	++test_count;				\
	if(!(val))				\
	{					\
	    fprintf(stderr, "test failed !\n"); \
	    exit(EXIT_FAILURE);			\
	}					\
	printf("\n");				\
    } while(0)
