CC = gcc
CFLAGS = -Wall -O2

all: writer reader

writer: writer.c
	$(CC) $(CFLAGS) writer.c -o writer

reader: reader.c
	$(CC) $(CFLAGS) reader.c -o reader

run: all
	@echo "=== Running writer & reader demo ==="
	@./writer &
	@sleep 1
	@./reader
	@echo "=== Done ==="

clean:
	rm -f writer reader /tmp/mmap_share.bin
