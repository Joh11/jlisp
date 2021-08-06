CC = gcc
CFLAGS = 
LDFLAGS = 

CFLAGS += -g # debug
# CFLAGS += -O3 # release

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

all: jlisp

clean:
	rm -f jlisp *.o

tags:
	etags `find . -name "*.h" -o -name "*.c"`

jlisp: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: all clean tags
