.PHONY: clean

build:
	$(CC) -o httpclient.bin httpclient.c

test: test.bin
	./test.bin

test.bin: test.c Makefile
	$(CC) -o $@ test.c

clean:
	rm -rf *.bin
	rm -rf *.o
