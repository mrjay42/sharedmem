CC = gcc
CFLAGS = -Wall -O2

all: shared

shared: shared.c
	$(CC) $(CFLAGS) shared.c -o shared

run: shared
	@echo "=== Running shared mmap demo ==="
	@./shared
	@echo "=== Done ==="

clean:
	rm -f shared
