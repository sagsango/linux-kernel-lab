CC=gcc

.PHONY: all clean

all: test_syscall_s2_encrypt

test_syscall_s2_encrypt: test_syscall_s2_encrypt.c
	$(CC) -o $@ $<

clean:
	rm -f test_syscall_s2_encrypt

