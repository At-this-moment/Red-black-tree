.PHONY: test clean

CFLAGS=-I ../common -Wall -O -DSENTINEL

test: test-rbtree
	./test-rbtree
	valgrind -s --leak-check=full ./test-rbtree

clean:
	rm -f test-rbtree *.o

test-rbtree: test-rbtree.o rbtree.o

test-rbtree.o: ../common/test-rbtree.c
	$(CC) $(CFLAGS) -c ../common/test-rbtree.c