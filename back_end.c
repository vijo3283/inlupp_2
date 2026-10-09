#include "back_end.h"


merch_t *ioopm_create_merchandise(char *name, char *description, int price){
    merch_t *merch = malloc(sizeof(merch_t));
    merch->name = name;
    merch->description = description;
    merch->price = price;
    merch->stock = 0;
    merch->location = ioopm_list_create();
    return merch;
}


void ioopm_add_merchandise(hash_table_t *ht, merch_t *merch){
    //TODO: Stub
}

void list_merchandise(hash_table_t *ht){
    //TODO; stub
}

void remove_merchandise(hash_table_t *ht, merch_t merch){
    //TODO; stub
}

void edit_merchandise(hash_table_t *ht, merch_t merch){
    //TODO; stub
}

size_t show_stock(hash_table_t *ht, merch_t merch){
    //TODO; stub
}

void replenish(hash_table_t *ht, merch_t merch, size_t amount){
    //TODO; stub
}

shopping_cart_t create_cart(){
    //TODO; stub
}

void remove_cart(shopping_cart_t shopping_cart){
    //TODO; stub
}

shopping_cart_t add_to_cart(shopping_cart_t shopping_cart, merch_t merch, size_t amount){
    //TODO: stub
}

shopping_cart_t remove_from_cart(shopping_cart_t shopping_cart, merch_t merch, size_t amount){
    //TODO: stub
}

int calculate_cost(shopping_cart_t shopping_cart){
    //TODO: stub
}


void checkout(shopping_cart_t shopping_cart){
    //TODO: stub
}

void quit(){
    //TODO: stub
}