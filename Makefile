CFLAGS=-Wall -Wextra -Werror -pedantic
CLIBS=-L. -I.
OUTFILE=carmel
CC=clang
all: compile
compile:
	mkdir -p build
	$(CC) $(CFLAGS) main.c -o build/$(OUTFILE) $(CLIBS)
run: compile
	./$(OUTFILE)
clean:
	rm -v builf/*

install:
	cp build/$(OUTFILE) /usr/local/bin/
