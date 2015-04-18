btree_test: btree_test.o ../btree.o
	gcc -o btree_test btree_test.o ../btree.o

btree_test.o: btree_test.c ../btree.h
	gcc -c btree_test.c

btree.o: ../btree.c ../btree.h
	gcc -c ../btree.c
