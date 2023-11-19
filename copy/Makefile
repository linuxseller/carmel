CFLAGS=-Wall -Wextra -Werror -pedantic
CLIBS=-L. -I.
OUTFILE=cinit
CC=clang
all: compile
compile:
	$(CC) $(CFLAGS) main.c -o build/$(OUTFILE) $(CLIBS)
run: compile
	./$(OUTFILE)
clean:
	rm $(OUTFILE)

install:
	cp build/$(OUTFILE) /usr/local/bin/
