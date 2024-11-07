CC	= gcc
CFLAG = -fPIC -fprofile-arcs -ftest-coverage
CCFLAGS = -Wall -g -pedantic

# Hash table test
test_hash_table: ./hash_table/test_hash_table.c ./hash_table/hash_table.c ./linked_list/linked_list.c
	$(CC) $(CCFLAGS) $(CFLAG) ./hash_table/test_hash_table.c ./linked_list/linked_list.c ./hash_table/hash_table.c -lcunit -o test_hash_table

run_test_hash_table: test_hash_table
	./test_hash_table

valgrind_test_hash_table: test_hash_table
	valgrind --leak-check=full ./test_hash_table

# Linked list test
test_linked_list: ./linked_list/test_linked_list.c ./linked_list/linked_list.c
	gcc $(CCFLAGS) $(CFLAG) ./linked_list/test_linked_list.c ./linked_list/linked_list.c -lcunit -o test_linked_list

run_test_linked_list: test_linked_list
	./test_linked_list

valgrind_test_linked_list: test_linked_list
	valgrind --leak-check=full ./test_linked_list

# Iterator test
test_iterator: ./iterator/iterator.c ./iterator/test_iterator.c 
	gcc $(CCFLAGS) $(CFLAG) ./iterator/test_iterator.c ./iterator/iterator.c -lcunit -o test_iterator

run_test_iterator: test_iterator
	./test_iterator

valgrind_test_iterator: test_iterator
	valgrind --leak-check=full ./test_iterator

# Frequency count test
test_freq_count: ./freq_count/freq_count.c ./hash_table/hash_table.c ./linked_list/linked_list.c ./iterator/iterator.c
	gcc -pg -Wall -o $(CFLAG) ./freq_count/freq_count.c ./hash_table/hash_table.c ./linked_list/linked_list.c ./iterator/iterator.c
	

run_test_freq_count: test_freq_count  
	./freq_count/freq_count 1k-long-words.txt

#Coverage test
coverage_test: run_test_hash_table run_test_iterator run_test_linked_list
	gcovr -r . --exclude 'freq_count/freq_count\.c'
