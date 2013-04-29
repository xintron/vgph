CC=gcc -std=c11

all: vgph

debug: CFLAGS += -DDEBUG -g
debug: vgph

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

vgph: vgph.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o vgph

test: all
	sh vgph-test.sh

.PHONY: clean test
