SHELL = /bin/sh

CC = gcc
CFLAGS = -pedantic
OBJECTS = obj/bignat.o \
          obj/bigfloat.o \
          obj/interval.o \
          obj/memnode.o \
          obj/memgraph.o \
          obj/list.o \

MKDR = mkdir

.PHONY: all clean

all: algoritmo_risolutivo

algoritmo_risolutivo: obj obj/main.o $(OBJECTS)
	$(CC) obj/main.o $(OBJECTS) -o $@ $(CFLAGS)

obj:
	$(MKDR) $@
obj/main.o: src/main.c
	$(CC) -c src/main.c -o $@ $(CFLAGS)
obj/bignat.o: src/bignat/bignat.c
	$(CC) -c src/bignat/bignat.c -o $@ $(CFLAGS)
obj/bigfloat.o: src/bigfloat/bigfloat.c
	$(CC) -c src/bigfloat/bigfloat.c -o $@ $(CFLAGS)
obj/interval.o: src/interval/interval.c
	$(CC) -c src/interval/interval.c -o $@ $(CFLAGS)
obj/memnode.o: src/memgraph/memnode/memnode.c
	$(CC) -c src/memgraph/memnode/memnode.c -o $@ $(CFLAGS)
obj/memgraph.o: src/memgraph/memgraph.c
	$(CC) -c src/memgraph/memgraph.c -o $@ $(CFLAGS)
obj/list.o: src/list/list.c
	$(CC) -c src/list/list.c -o $@ $(CFLAGS)

clean:
	rm -Rf obj
