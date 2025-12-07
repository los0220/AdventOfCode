CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -pedantic

.PHONY:all
all: part1.bin part2.bin

part1.bin: part1.c
	$(CC) $(CFLAGS) $^ -o $@

part2.bin: part2.c
	$(CC) $(CFLAGS) $^ -o $@
