#pragma once
#include <stdbool.h>
#include "common.h"


struct link
{
    elem_t value;
    link_t *next;
};

struct list 
{
    link_t *head;
    link_t *tail;
    size_t size;
};

