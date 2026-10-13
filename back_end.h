#pragma once
#include"hash_table.h"
#include"hash_table_iterator.h"
#include"linked_list.h"
#include"linked_list_iterator.h"
#include"hash_table_internal.h"
#include"linked_list_internal.h"


typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct data_base data_base_t;
typedef struct cart_item cart_item_t;




struct merch 
{
    char *name;
    char *description;
    int price;
    size_t stock;

    int reserved;

    //dubbel pointers array?
    ioopm_list_t *location;

};


struct shelf 
{
    char *shelf;
    size_t quantity;
    char *merch_name;
};


struct data_base
{
    hash_table_t *merch_ht;
    hash_table_t *shelf_ht;
    hash_table_t *cart_ht;
    int next_cart_id;
};

struct cart_item
{
    merch_t *merch;
    int quantity;
};

bool is_positive_number(int number);


void ioopm_list_sort(ioopm_list_t *list, char *value);


data_base_t *ioopm_create_data_base(void);

merch_t *ioopm_create_merchandise(char *name, char *description, int price);


void ioopm_add_merchandise(hash_table_t *ht, merch_t *merch);

//true if there is more items to list
bool list_merchandise(data_base_t *db, size_t index);

bool list_one_merch(data_base_t *db, char *merch);



bool remove_merchandise(data_base_t *db, char *merch);


bool edit_merchandise(data_base_t *db, char *old_name, char *new_name, char *description, size_t price);


size_t show_stock(data_base_t *db, char *merch);


bool replenish(data_base_t *db, char *merch, size_t amount, char *shelf);

int create_cart(data_base_t *db);

cart_item_t *create_cart_item(merch_t *merch, int amount);


void remove_cart(void);
 

bool add_to_cart(data_base_t *db, int user_id, char *merch, int amount);


void remove_from_cart(merch_t merch, size_t amount);


int calculate_cost(data_base_t *db, int user_id);


void checkout(void);


