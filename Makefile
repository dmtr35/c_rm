CFLAGS = -Wall -std=c99

all main

main: rr.c check_func.c extra_func.c remove_files.c
	gcc $(CFLAGS) $^ -o $@












