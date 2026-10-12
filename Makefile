program: front_end.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c utils.c
	gcc -Wall -Wextra -g front_end.c back_end.c hash_table.c hash_table_iterator.c linked_list.c linked_list_iterator.c utils.c -o program

run: program
	./program

clean:
	rm -f program