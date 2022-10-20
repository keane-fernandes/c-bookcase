CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

bookcase : bookcase.c neillsimplescreen.c neillsimplescreen.h
	$(CC) neillsimplescreen.c bookcase.c -o bookcase $(COMMON)

run_bookcase: bookcase
	./bookcase

clean:
	rm -f bookcase
