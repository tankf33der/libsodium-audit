all: address memory undefined ccomp interp
address:
	clang -fsanitize=address *.c && ./a.out
memory:
	clang -fsanitize=memory *.c && ./a.out
undefined:
	clang -fsanitize=undefined *.c && ./a.out
mike:
	cat my_utils.c shorthash_siphashx24_ref.c test_siphashx24.c >> mike.c
ccomp: mike
	ccomp mike.c && ./a.out
interp: mike
	ccomp -interp -quiet mike.c
clean:
	rm -rf *.o *.out mike.c
