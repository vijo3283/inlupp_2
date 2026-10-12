#pragma once
#include"hash_table.h"
#include"hash_table_iterator.h"
#include"linked_list.h"
#include"linked_list_iterator.h"
#include"hash_table_internal.h"
#include"linked_list_internal.h"


typedef struct merch merch_t;
typedef struct shopping_cart shopping_cart_t;
typedef struct shelf shelf_t;
typedef struct data_base data_base_t;


struct merch 
{
    char *name;
    char *description;
    int price;
    size_t stock;

    //dubbel pointers array?
    ioopm_list_t *location;

};

struct shopping_cart
{
    ioopm_list_t *cart;
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
    ioopm_list_t *carts;
};

data_base_t *ioopm_create_data_base(void);

merch_t *ioopm_create_merchandise(char *name, char *description, int price);


void ioopm_add_merchandise(hash_table_t *ht, merch_t *merch);


void list_merchandise(hash_table_t *ht);


bool remove_merchandise(data_base_t *db, char *merch);


bool edit_merchandise(data_base_t *db, char *old_name, char *new_name, char *description, size_t price);


size_t show_stock(data_base_t *db, char *merch);


bool replenish(data_base_t *db, char *merch, size_t amount, char *shelf);

shopping_cart_t create_cart();


void remove_cart(shopping_cart_t shopping_cart);
 

shopping_cart_t add_to_cart(shopping_cart_t shopping_cart, merch_t merch, size_t amount);


shopping_cart_t remove_from_cart(shopping_cart_t shopping_cart, merch_t merch, size_t amount);


int calculate_cost(shopping_cart_t shopping_cart);


void checkout(shopping_cart_t shopping_cart);


void quit();