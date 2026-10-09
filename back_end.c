#include "back_end.h"

data_base_t *ioopm_create_data_base(void){

    data_base_t *new_db;

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

bool replenish(data_base_t *db, char *merch, size_t amount, char *shelf){
    //TODO: stub

    //vi måste fixa eftersom de pekar på samma struct och kanske lägga till hylla redan i början.
    shelf_t *shelf_occupied;
    //kolla igenom shelf ht för att kolla så varan inte finns redan på en hylla upptagen av en annan vara
    if(ioopm_hash_table_lookup(db->shelf_ht, string_elem(shelf), &shelf_occupied))
    {
        //redan upptagen av en annan merch
        if(!(strcmp(shelf_occupied->merch_name->s, merch) == 0))
        {
            return false;

        }
        
    }

    merch_t *current_merch;
    
    if(ioopm_hash_table_lookup(db->merch_ht,string_elem(merch), &current_merch))
    {
        
        current_merch->stock += amount;
        ioopm_list_t *shelf_list = current_merch->location;

        

        ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(shelf_list);

        
        while(!ioopm_list_iterator_at_end(list_it))
        {
            shelf_t *current_shelf = ioopm_list_iterator_current(list_it).p;

            
            if(strcmp(current_shelf->shelf , shelf) == 0)
            {
                current_shelf->quantity += amount;
                break;       
            }

            ioopm_list_iterator_advance(list_it);
        }


        if(ioopm_list_iterator_at_end(list_it))
        {

            shelf_t *new_shelf = create_shelf(merch,amount,shelf);
            

            ioopm_list_append(shelf_list, ptr_elem(new_shelf));   
        
            ioopm_hash_table_insert(db->shelf_ht, string_elem(new_shelf->shelf) ,ptr_elem(new_shelf));

        }

        
        
    
        
        return true;

    }

    return false;

    
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