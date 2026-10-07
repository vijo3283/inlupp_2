#pragma once
#include"../inlupp1/hash_table.h"
#include"../inlupp1/hash_table_iterator.h"
#include"../inlupp1/linked_list.h"
#include"../inlupp1/linked_list_iterator.h"
#include"../inlupp1/hash_table_internal.h"
#include"../inlupp1/linked_list_internal.h"


typedef struct merch merch_t;
typedef struct shopping_cart shopping_cart_t;
typedef struct shelf shelf_t;


struct merch 
{
    char *name;
    char *description;
    int price;
    size_t stock;
    list_t *location;

};

struct shopping_cart
{
    list_t *cart;
};

struct shelf 
{
    char *shelf;
    int quantity;
};


merch_t *ioopm_create_merchandise(char *name, char *description, int price);


void ioopm_add_merchandise(hash_table_t *ht, merch_t *merch);


void list_merchandise(hash_table_t *ht);


void remove_merchandise(hash_table_t *ht, merch_t merch);


void edit_merchandise(hash_table_t *ht, merch_t merch);


size_t show_stock(hash_table_t *ht, merch_t merch);


void replenish(hash_table_t *ht, merch_t merch, size_t amount);

shopping_cart_t create_cart();


void remove_cart(shopping_cart_t shopping_cart);
 

shopping_cart_t add_to_cart(shopping_cart_t shopping_cart, merch_t merch, size_t amount);


shopping_cart_t remove_from_cart(shopping_cart_t shopping_cart, merch_t merch, size_t amount);


int calculate_cost(shopping_cart_t shopping_cart);


void checkout(shopping_cart_t shopping_cart){


void quit();