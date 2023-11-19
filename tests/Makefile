CFLAGS=-Wall -Wextra -Werror -pedantic
CLIBS=-L. -I.
OUTFILE=run
CC=cc
all: compile
compile: 
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c -o build/$(OUTFILE) $(CLIBS)
run: compile
	./$(OUTFILE)
clean:
	rm -v build/*

