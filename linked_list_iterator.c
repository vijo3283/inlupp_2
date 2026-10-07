#include <stdbool.h>
#include <stdlib.h>

#include "linked_list.h"
#include "linked_list_iterator.h"
#include "linked_list_internal.h"

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    ioopm_list_iterator_t *il = calloc(1, sizeof(ioopm_list_iterator_t));
    il->list = l;
    il->current_node = l->first;
    il->current_element = ioopm_list_head(l);
    return il;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter)
{
    // Checks if the current element is the last element (.p because we don't know the type)
    return iter->current_node == NULL;
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter)
{
    if (!ioopm_list_iterator_at_end(iter))
    {
        // Advances the node if not at end.

        iter->current_node = iter->current_node->next;
        // iter->current_element = iter->current_node->value;
         if(iter->current_node != NULL)
        {
            // Updates current node if it exists.
            iter->current_element = iter->current_node->value;
        }
     }
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter)
{   
    return iter->current_element;
}

elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *iter)
{
    // TODO: Create remove function
    (void)iter;
    return ptr_elem(NULL);
}

void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    // TODO: Create insert function
    (void)iter;
    (void)element;
}