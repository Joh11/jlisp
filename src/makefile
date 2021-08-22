CC = gcc
CFLAGS = 
LDFLAGS = 

CFLAGS += -g # debug
# CFLAGS += -O3 # release

OBJECTS = $(patsubst %.c, ../build/%.o, $(wildcard *.c))

all: ../jlisp

clean:
	rm -f jlisp ../build/*.o

tags:
	etags `find . -name "*.h" -o -name "*.c"`

../build/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

../jlisp: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: all clean tags
