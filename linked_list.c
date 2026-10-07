#include <stdbool.h>
#include <stdio.h>
#include "linked_list.h"
#include "linked_list_internal.h"

ioopm_list_t *ioopm_list_create(void)
{
    return calloc(1, sizeof(ioopm_list_t));
}

void ioopm_list_destroy(ioopm_list_t *list)
{
    link_t *p1 = list->first;

    size_t size = ioopm_list_size(list);
    if (list != NULL)
    {
        for (size_t i = 0; i < size; i++)
        {   
            // Destroys element and redirects the links
            link_t *pn = p1->next;
            free(p1);
            p1 = pn;
        }
    } 
    free(list);
}

void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
    link_t *new = calloc(1, sizeof(link_t));
    new->value = value;

    if (ioopm_list_is_empty(list))
    {
        // If empty, place first
        list->first = new;
    }
    else 
    {
        list->last->next = new;
    }
    list->last = new;
    list->size++;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
{
    link_t *new = calloc(1, sizeof(link_t));
    new->value = value;
    new->next = list->first;
    list->first = new;
    list->size++;
}

elem_t ioopm_list_head(ioopm_list_t *list)
{
    if (ioopm_list_is_empty(list))
    {
        return ptr_elem(NULL);
    }
    return list->first->value;
}

elem_t ioopm_list_last(ioopm_list_t *list)
{
    if (ioopm_list_is_empty(list))
    {
        return ptr_elem(NULL);
    }
    return list->last->value;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{

    if (index > ioopm_list_size(list))
    {
        printf("Index %ld utanför storlek!\n", index);
    }
    
    else
    {
        link_t *new = calloc(1, sizeof(link_t));
        new->value = value;
        link_t *previous = list->first;

        for (size_t i = 0; i < index - 1; i++)
        {
            // Update the pointers position until we are at the correct index (index - 1 because we want to point to the insert)
            previous = previous->next;
        }
        new->next = previous->next;
        previous->next = new;
        list->size++;
    }

}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    link_t *previous = list->first;
    link_t *to_remove = previous->next;

    if (index > ioopm_list_size(list))
    {
        printf("Index %ld utanför storlek!\n", index);
    }
    
    else
    {
        for (size_t i = 0; i < index - 1; i++)
        {
            // Update the pointers position until we are at the correct index (index - 1 because we want to point to what to remove)
            previous = previous->next;
        }
        elem_t remove_value = to_remove->value;
        previous->next = to_remove->next;
        free(to_remove);
        list->size--;
        return remove_value;
    }

    return ptr_elem(NULL);
}

elem_t ioopm_list_get(ioopm_list_t *list, size_t index)
{
    link_t *current = list->first;
    size_t size = ioopm_list_size(list);
    if (index > size)
    {
        printf("Index %ld utanför storlek!\n", index);
    }
    else
    {
        for (size_t i = 0; i < size; i++)
        {
            if (i == index)
            {
                return current->value;
            }
            else 
            {
                current = current->next;
            }
        }
    }
    return ptr_elem(NULL); 
}

size_t ioopm_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
    if (list->size == 0)
    {
        return true;
    }
    return false;
}
