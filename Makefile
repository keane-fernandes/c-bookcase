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

sort_bc_one: bookcase
	./bookcase bookcases/ccbb-23.bc verbose

sort_bc_two: bookcase
	./bookcase bookcases/mwwbkwmkb-548.bc verbose

sort_bc_three: bookcase
	./bookcase bookcases/rgb-321.bc verbose

sort_bc_four: bookcase
	./bookcase bookcases/rgbrmrykwrrr-3521.bc verbose

sort_bc_five: bookcase
	./bookcase bookcases/rggrcyyc-557.bc verbose

sort_bc_six: bookcase
	./bookcase bookcases/rggrkkkk-446.bc verbose

sort_bc_seven: bookcase
	./bookcase bookcases/rrggccyy-437.bc verbose

sort_bc_eight: bookcase
	./bookcase bookcases/rrrr-222.bc verbose

sort_bc_nine: bookcase
	./bookcase bookcases/yby-222.bc verbose

sort_bc_ten: bookcase
	./bookcase bookcases/yyybbb-334.bc verbose

clean:
	rm -f bookcase
