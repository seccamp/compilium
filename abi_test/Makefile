abi_test.bin : main.c helper.S
	$(CC) -Wall -o $@ main.c helper.S

test : abi_test.bin
	./abi_test.bin

format :
	clang-format -i *.c
