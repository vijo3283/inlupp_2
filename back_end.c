#include"back_end.h"


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
    ioopm_hash_table_insert(ht, string_elem(merch->name), ptr_elem(merch));
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

void replenish(hash_table_t *ht, merch_t *merch, size_t amount, char *shelf){
   if(ioopm_hash_table_has_key(ht, ptr_elem(merch->name))){
    hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
    while (!ioopm_hash_table_iterator_current_key == merch->name){
        ioopm_hash_table_iterator_advance(it);
    }
    list_t *shelf_list = merch->location;
    list_iterator_t *list_iter = ioopm_list_iterator_create(shelf_list);
    while (!ioopm_list_iterator_current == shelf){
        ioopm_list_iterator_advance(it);
    }
    

   }

    
    }
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