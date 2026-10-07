#pragma once

typedef struct link link_t;
typedef struct list list_t;
typedef struct list_iterator list_iterator_t;

struct link
{
    elem_t value;
    link_t *next;   
};

struct list
{
    link_t *first;
    link_t *last;
    size_t size;
};

struct list_iterator
{
    ioopm_list_t *list;
    link_t *current_node;
    elem_t current_element;
};