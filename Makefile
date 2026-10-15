program: front_end.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c utils.c
	gcc -Wall -Wextra -g front_end.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c utils.c -o program

run: program
	./program


test: program
	{ \
        n=1; \
        while [ "$$n" -le 21 ]; do \
            printf 'A\nTestvara%02d\nBeskrivning %02d\n%d\nP\nTestvara%02d\n5\nA%02d\n' "$$n" "$$n" "$$((n * 10))" "$$n" "$$n"; \
            n=$$((n + 1)); \
        done; \
        cat /dev/tty; \
	} | ./program

text: program
	./program < webstoretest.txt

clean:
	rm -f program

.PHONY: test test-interactive