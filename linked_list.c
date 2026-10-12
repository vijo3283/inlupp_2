#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "linked_list.h"
#include "linked_list_iterator.h"
#include "linked_list_internal.h"
#include "common.h"



static link_t *create_link(elem_t value)
{
    link_t *new_link = malloc(sizeof(link_t));

    new_link->value = value;

    new_link->next = NULL;

    return new_link;

}

static link_t *find_link_at_index(ioopm_list_t *list, size_t index)
{
    link_t *current = list->head;

    for(size_t i = 0; i < index; i ++)
    {
        current = current->next;
    }

    return current;
}

ioopm_list_t *ioopm_list_create(void)
{
    ioopm_list_t *new_list;
    new_list = calloc(1, sizeof(ioopm_list_t));
    return new_list;
}

void ioopm_list_destroy(ioopm_list_t *list)
{
    assert(list != NULL &&"Trying to destroy a NULL list");
    link_t *first_link = list->head;

    while(first_link != NULL)
    {
        link_t *current = first_link;

        first_link = first_link->next;

        free(current);
    }

    free(list);
}

void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
    link_t *new_link = create_link(value);
    new_link->next = NULL;

    if(list->head == NULL)
    {
        list->head = new_link;
        list->tail = new_link;
    }
    else
    {
        list->tail->next = new_link;

        list->tail = new_link;
    }

    list->size++;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
{
    link_t *new_link = create_link(value);

    if(list->head == NULL)
    {
        list->head = new_link;
        list->tail = new_link;

    }
    else
    {
        new_link->next = list->head;
        list->head = new_link;
    }
    list->size++;
}

elem_t ioopm_list_head(ioopm_list_t *list)
{
    assert(!ioopm_list_is_empty(list) && "Listan är tom och har ej head");
    return list->head->value;
}

elem_t ioopm_list_last(ioopm_list_t *list)
{
    assert(!ioopm_list_is_empty(list) && "Listan är tom och har ej tail");
    return list->tail->value;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
    assert(index <= list->size && "Index out of bounds");

    if(index == 0)
    {
        ioopm_list_prepend(list, value);
    }
    else if (index == list->size)
    {
        ioopm_list_append(list, value);
    }
    else
    {
        link_t *before = find_link_at_index(list,index-1);
        link_t *after = find_link_at_index(list, index);
        link_t *new_link = create_link(value);

        before->next = new_link;
        new_link->next = after;

        list->size += 1;
    }
    
  
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    assert(!ioopm_list_is_empty(list) && "Listan är tom, kan ej radera från");
    assert(index < list->size && "Index out of bounds");

    link_t *rm = NULL;

    if(index == 0)
    {
        
        rm = list->head;
        list->head = rm->next;

        if(list->size == 1)
        {
            list->tail = NULL;

        }
        
    }
    else 
    {
        link_t *before = find_link_at_index(list, index-1);
        rm = before->next;
        before->next = rm->next;

        if(index == list->size - 1)
        {
            list->tail = before;
            before->next = NULL;
        }  
    }

    elem_t value = rm->value;
    free(rm);
    list->size -= 1;
    return value;
    
}

elem_t ioopm_list_get(ioopm_list_t *list, size_t index)
{
    assert(ioopm_list_is_empty(list) == false && "list is empty, no index");

    assert(list->size > index && "Index too big, not possible");

    link_t *current = find_link_at_index(list,index);

    return current->value;
}

size_t ioopm_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
    if(list->head == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}


