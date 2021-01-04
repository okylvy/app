.PHONY: clean

build:
	$(CC) -o ping.bin ping.c

test: test.bin
	./test.bin

test.bin: test.c Makefile
	$(CC) -o $@ test.c

clean:
	rm -rf *.bin
	rm -rf *.o
