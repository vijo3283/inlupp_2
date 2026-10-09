#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "linked_list.h"
#include "linked_list_iterator.h"
#include "linked_list_internal.h"


struct list_iterator {
    link_t *current;       
    ioopm_list_t *list; 
};

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    assert(l != NULL && "list pointer is null");


    ioopm_list_iterator_t *iter = malloc(sizeof(ioopm_list_iterator_t));

    if(iter == NULL)
    {
        return NULL;
    }

    iter->list = l;
    iter->current = l->head;

    return iter;
}


void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter)
{
    assert(iter != NULL && "Iterator pointer must not be NULL");

    return iter->current != NULL;
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter)
{
    assert(iter != NULL && "Iterator cannot be null");
    assert(ioopm_list_iterator_at_end(iter) && " iterator at end cannot advance");

    iter->current = iter->current->next;
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter)
{
    assert(iter != NULL && "iterator cannot be null");
    assert(iter->current != NULL && "no current value on a NULL, is at end");

    return iter->current->value;

}

//not implemented
elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *iter)
{
    (void)iter;
    return int_elem(-2);
}

//not implemented
void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    (void)iter;
    (void)element;

}

