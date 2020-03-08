all: clean address memory undefined ccomp interp
address:
	clang -Wall -Wextra -fsanitize=address *.c && ./a.out
memory:
	clang -Wall -Wextra -fsanitize=memory *.c && ./a.out
undefined:
	clang -Wall -Wextra -fsanitize=undefined *.c && ./a.out
mike:
	rm -rf mike.c
	cat *.c >> mike.c
ccomp: mike
	ccomp mike.c && ./a.out
interp: mike
	ccomp -interp mike.c
clean:
	rm -rf *.o *.out mike.c
