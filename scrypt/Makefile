all: clean address memory undefined
address: clean
	clang -fsanitize=address *.c && ./a.out
memory: clean
	clang -fsanitize=memory *.c && ./a.out
undefined: clean
	clang -fsanitize=undefined *.c && ./a.out
mike:
	cat *.c >> mike.c
ccomp: clean mike
	ccomp mike.c && ./a.out
interp: clean mike
	ccomp -interp -quiet mike.c
clean:
	rm -rf *.o *.out mike.c
