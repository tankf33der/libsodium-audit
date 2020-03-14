all: clean address memory undefined
address: clean
	clang -Wall -Wextra -fsanitize=address *.c && ./a.out
memory: clean
	clang -Wall -Wextra -fsanitize=memory *.c && ./a.out
undefined: clean
	clang -Wall -Wextra -fsanitize=undefined *.c && ./a.out
mike:
	cat *.c >> mike.c
ccomp: clean mike
	ccomp -fstruct-passing mike.c && ./a.out
interp: clean mike
	ccomp -interp -quiet -fstruct-passing mike.c
clean:
	rm -rf *.o *.out mike.c
