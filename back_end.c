#include "back_end.h"

data_base_t *ioopm_create_data_base(void){

    data_base_t *new_db = malloc(sizeof(data_base_t));

    new_db->merch_ht = ioopm_hash_table_create(string_hash, comp_str);
    new_db->shelf_ht = ioopm_hash_table_create(string_hash, comp_str);
    new_db->carts = ioopm_list_create();

    return new_db;
};


merch_t *ioopm_create_merchandise(char *name, char *description, int price){
    merch_t *merch = malloc(sizeof(merch_t));
    merch->name = name;
    merch->description = description;
    merch->price = price;
    merch->stock = 0;
    merch->location = ioopm_list_create();
    return merch;
}


static shelf_t *create_shelf(char *name, size_t quantity, char *shelf)
{
    shelf_t *new_shelf = malloc(sizeof(shelf_t));
    new_shelf->shelf = shelf;
    new_shelf->quantity = quantity;
    new_shelf->merch_name = name;

    return new_shelf;
}

void ioopm_add_merchandise(hash_table_t *ht, merch_t *merch){
    ioopm_hash_table_insert(ht, string_elem(merch->name), ptr_elem(merch));
}

void list_merchandise(data_base_t *db, size_t index){
    ioopm_list_t *list = ioopm_list_create();
    hash_table_iterator_t *merch_it = ioopm_hash_table_iterator_create(db->merch_ht);

    while (!ioopm_hash_table_iterator_at_end(merch_it))
    {
        char *current_key = ioopm_hash_table_iterator_current_key(merch_it).s;
        ioopm_list_append(list, string_elem(current_key));
        ioopm_hash_table_iterator_advance(merch_it);
    }

    for (size_t i = index; i < index + 20; i++)
    {
        if (i > ioopm_list_size(list))
        {
            break;
        }
        else
        {
        printf("%d. %s \n", i + 1, list[i]);
        }
    }
}

bool remove_merchandise(data_base_t *db, char *merch){
    merch_t *merch_to_remove;
    bool merch_lookup = ioopm_hash_table_lookup(db->merch_ht, string_elem(merch), &merch_to_remove);

    if(!merch_lookup)
    {
        return false;
    }
    else 
    {   
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch_to_remove->location);

        while (!ioopm_list_iterator_at_end(it))
        {
            shelf_t *current_shelf = ioopm_list_iterator_current(it).p;
            char *shelf_name = current_shelf->shelf;
            ioopm_hash_table_remove(db->shelf_ht, string_elem(shelf_name));

            ioopm_list_iterator_advance(it);

            free(current_shelf);
            
        }
        ioopm_list_iterator_destroy(it);
        ioopm_list_destroy(merch_to_remove->location);
        ioopm_hash_table_remove(db->merch_ht, string_elem(merch));
    }
}

bool edit_merchandise(data_base_t *db, char *old_name, char *new_name, char *description, size_t price){
    merch_t *old_merch;
    bool merch_lookup = ioopm_hash_table_lookup(db->merch_ht, string_elem(old_name), &old_merch);

    if(!merch_lookup)
    {
        return false;
    }

    if (strcmp(old_merch->name, new_name) == 0)
    {
        old_merch->description = description;
        old_merch->price = price;
        return true;
    }
    else
    {
        merch_t *result;
        if(ioopm_hash_table_lookup(db->merch_ht, string_elem(new_name), &result))
        {
            return false;
        }
        ioopm_hash_table_remove(db->merch_ht, string_elem(old_name));
        old_merch->name = new_name;
        old_merch->description = description;
        old_merch->price = price;
        ioopm_list_t *old_list = old_merch->location;

        ioopm_list_iterator_t *it = ioopm_list_iterator_create(old_list);

        while(!ioopm_list_iterator_at_end(it)){
            shelf_t *current_shelf = ioopm_list_iterator_current(it).p;
            current_shelf->merch_name = new_name;
            ioopm_list_iterator_advance(it);
        }
        ioopm_list_iterator_destroy(it);
        ioopm_hash_table_insert(db->merch_ht, string_elem(new_name), ptr_elem(old_merch));
        return true;
        
    }
}

size_t show_stock(data_base_t *db, char *merch){
     merch_t *current_merch;
     bool merch_lookup = ioopm_hash_table_lookup(db->merch_ht, string_elem(merch), &current_merch);

     if (merch_lookup) {
        return current_merch->stock;
     }
     else 
     {
        return 0;
     }
}

bool replenish(data_base_t *db, char *merch, size_t amount, char *shelf){
    //TODO: stub
    //vi måste fixa eftersom de pekar på samma struct och kanske lägga till hylla redan i början.
    shelf_t *shelf_occupied;
    bool shelf_exist = ioopm_hash_table_lookup(db->shelf_ht, string_elem(shelf), &shelf_occupied);

     merch_t *current_merch;
     bool merch_lookup = ioopm_hash_table_lookup(db->merch_ht,string_elem(merch), &current_merch);

     ioopm_list_t *shelf_list = current_merch->location;
    //kolla igenom shelf ht för att kolla så varan inte finns redan på en hylla upptagen av en annan vara

    if (!merch_lookup){
        return false;
    }

    if(shelf_exist)
    {
        //redan upptagen av en annan merch
        if(!(strcmp(shelf_occupied->merch_name, merch) == 0))
        {
            return false;

        }
        else 
        {
             current_merch->stock += amount;
             shelf_occupied->quantity += amount;
        } 
    }
    else 
    {
        shelf_t *new_shelf = create_shelf(merch, amount, shelf);
        current_merch->stock += amount;
        ioopm_hash_table_insert(db->shelf_ht, string_elem(new_shelf->shelf), ptr_elem(new_shelf));
        ioopm_list_append(shelf_list, ptr_elem(new_shelf));

    }
    
        return true;
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