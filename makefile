CC = gcc
CFLAGS = 
LDFLAGS = 

CFLAGS += -g # debug
# CFLAGS += -O3 # release

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.c, build/%.o, $(SOURCES))

TEST_OBJECTS = $(patsubst test/%.c, test/%.o, $(wildcard test/*.c)) $(filter-out build/main.o, $(OBJECTS))

all: jlisp

tests: test/test 
	./test/test

test/test: $(TEST_OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -f jlisp build/*.o

tags:
	etags `find . -name "*.h" -o -name "*.c"`

build/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

# test/%.o: test/%.c
# 	$(CC) $(CFLAGS) -c $^ -o $@

jlisp: $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.PHONY: all clean tags tests
