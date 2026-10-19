COV_SOURCES = backend_cunit_tests.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c


program: front_end.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c utils.c
	gcc -Wall -Wextra -g front_end.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c utils.c -o program


run: program
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program

text: program
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < webstoretest.txt


backend_tests: backend_cunit_tests.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c
	gcc -g -Wall -Wextra -o backend_tests backend_cunit_tests.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c -lcunit

test_backend: backend_tests
	./backend_tests

valgrind_backend: backend_tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./backend_tests

backend_tests_cov: $(COV_SOURCES)
	gcc -g -Wall -Wextra --coverage -o $@ $(COV_SOURCES) -lcunit

gcov: backend_tests_cov
	rm -f backend_tests_cov-*.gcda
	./backend_tests_cov
	gcov -b -c backend_tests_cov-*.gcno


clean:
	rm -f program *.gcov *.gcda *.gcno *_cov

.PHONY: test test-interactive gcov