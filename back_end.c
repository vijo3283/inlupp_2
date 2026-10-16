#include "back_end.h"
#include <stdio.h>

bool ioopm_is_positive_number(int number)
{
    return (number > 0);
}

static int enough_stock(merch_t *merch)
{
    int stock_left = (merch->stock) - (merch->reserved);
    return stock_left;
}

static void ioopm_list_sort(ioopm_list_t *list, char *value)
{
    size_t i = 0;
    while (i < list->size)
    {
        char *current = (ioopm_list_get(list, i)).s;

        if (strcmp(value, current) < 0)
        {
            break;
        }
        i++;
    }
    ioopm_list_insert(list, i, string_elem(value));
}

static bool find_merch_shelf(hash_table_t *ht, char *key, elem_t *result)
{
    return ioopm_hash_table_lookup(ht, string_elem(key), result);
}

static bool find_cart(hash_table_t *ht, int key, elem_t *result)
{
    return ioopm_hash_table_lookup(ht, int_elem(key), result);
}

data_base_t *ioopm_create_data_base(void)
{

    data_base_t *new_db = malloc(sizeof(data_base_t));

    new_db->merch_ht = ioopm_hash_table_create(string_hash, comp_str);
    new_db->shelf_ht = ioopm_hash_table_create(string_hash, comp_str);
    new_db->cart_ht = ioopm_hash_table_create(int_hash, comp_int);
    new_db->next_cart_id = 1;

    return new_db;
};

merch_t *ioopm_create_merchandise(data_base_t *db, char *name, char *description, int price)
{

    elem_t result;
    bool merch_exist = ioopm_hash_table_lookup(db->merch_ht, string_elem(name), &result);

    if (merch_exist)
    {
        printf("Can not create merch");
        return NULL;
    }

    merch_t *merch = malloc(sizeof(merch_t));
    merch->name = name;
    merch->description = description;
    merch->price = price;
    merch->stock = 0;
    merch->location = ioopm_list_create();
    merch->reserved = 0;
    return merch;
}

static shelf_t *create_shelf(char *name, int quantity, char *shelf)
{
    shelf_t *new_shelf = malloc(sizeof(shelf_t));
    new_shelf->shelf = shelf;
    new_shelf->quantity = quantity;
    new_shelf->merch_name = name;

    return new_shelf;
}

bool ioopm_add_merchandise(data_base_t *db, merch_t *merch)
{
    if (!(merch == NULL))
    {
        ioopm_hash_table_insert(db->merch_ht, string_elem(merch->name), ptr_elem(merch));
        return true;
    }
    return false;
}

bool ioopm_list_merchandise(data_base_t *db, size_t index)
{
    // om listan är tom
    ioopm_list_t *list = ioopm_list_create();
    hash_table_iterator_t *merch_it = ioopm_hash_table_iterator_create(db->merch_ht);

    while (!ioopm_hash_table_iterator_at_end(merch_it))
    {
        char *current_key = ioopm_hash_table_iterator_current_key(merch_it).s;
        ioopm_list_sort(list, current_key);
        ioopm_hash_table_iterator_advance(merch_it);
    }
    ioopm_hash_table_iterator_destroy(merch_it);

    for (size_t i = index; i < index + 20; i++)
    {

        if (i >= ioopm_list_size(list))
        {
            break;
        }
        else
        {
            printf("%zu. %s \n", i + 1, ioopm_list_get(list, i).s);
        }
    }

    bool continue_list = list->size > index + 20;
    ioopm_list_destroy(list);

    return continue_list;
}

bool ioopm_list_one_merch(data_base_t *db, char *merch)
{
    elem_t result;

    if (find_merch_shelf(db->merch_ht, merch, &result))
    {
        merch_t *merch_print = result.p;

        printf("Name: %s\n", merch_print->name);
        printf("Description: %s\n", merch_print->description);
        printf("Price: %d\n", merch_print->price);
        printf("Stock: %d\n", merch_print->stock);
        printf("Reserved: %d\n", merch_print->reserved);
        return true;
    }
    return false;
}

void ioopm_remove_shelf(data_base_t *db, shelf_t *shelf)
{

    ioopm_hash_table_remove(db->shelf_ht, string_elem(shelf->shelf));
    free(shelf->shelf);
    free(shelf);
}

void ioopm_remove_from_shelf(data_base_t *db, merch_t *merch, int to_remove)
{
    ioopm_list_t *location_list = merch->location;
    if (to_remove == 0)
    {
        return;
    }

    shelf_t *shelf = ioopm_list_head(location_list).p;
    if (shelf->quantity > to_remove)
    {
        shelf->quantity -= to_remove;
    }
    else if (shelf->quantity == to_remove)
    {
        ioopm_remove_shelf(db, shelf);
        ioopm_list_remove(location_list, 0);
    }
    else if (shelf->quantity < to_remove)
    {
        to_remove -= shelf->quantity;
        ioopm_remove_shelf(db, shelf);
        ioopm_list_remove(location_list, 0);
        ioopm_remove_from_shelf(db, merch, to_remove);
    }
}

bool ioopm_remove_merchandise(data_base_t *db, char *merch)
{
    elem_t result;

    if (find_merch_shelf(db->merch_ht, merch, &result))
    {
        merch_t *merch_to_remove = result.p;

        hash_table_iterator_t *cart_it = ioopm_hash_table_iterator_create(db->cart_ht);

        while (!ioopm_hash_table_iterator_at_end(cart_it))
        {
            int key = ioopm_hash_table_iterator_current_key(cart_it).i;
            ioopm_list_t *value = ioopm_hash_table_iterator_current_value(cart_it).p;

            size_t index = 0;

            while (index < ioopm_list_size(value))
            {
                cart_item_t *item = ioopm_list_get(value, index).p;

                if (item->merch == merch_to_remove)
                {
                    int quantity = item->quantity;

                    ioopm_remove_from_cart(db, key, merch, quantity);
                    break;
                }
                index++;
            }
            ioopm_hash_table_iterator_advance(cart_it);
        }
        ioopm_hash_table_iterator_destroy(cart_it);

        ioopm_remove_from_shelf(db, merch_to_remove, merch_to_remove->stock);

        ioopm_list_destroy(merch_to_remove->location);

        ioopm_hash_table_remove(db->merch_ht, string_elem(merch_to_remove->name));

        free(merch_to_remove->name);
        free(merch_to_remove->description);
        free(merch_to_remove);
        return true;
    }
    return false;
}

bool ioopm_edit_merchandise(data_base_t *db, char *old_name, char *new_name, char *description, int price)
{

    elem_t result;

    if (find_merch_shelf(db->merch_ht, old_name, &result))
    {

        merch_t *old_merch = result.p;

        char *old_merch_name = old_merch->name;
        char *old_merch_description = old_merch->description;

        if (strcmp(old_merch->name, new_name) == 0)
        {
            old_merch->description = description;
            old_merch->price = price;

            free(old_merch_description);
            free(new_name);
            return true;
        }
        else
        {
            elem_t result;
            if (ioopm_hash_table_lookup(db->merch_ht, string_elem(new_name), &result))
            {
                return false;
            }

            ioopm_hash_table_remove(db->merch_ht, string_elem(old_name));
            old_merch->name = new_name;
            old_merch->description = description;
            old_merch->price = price;
            ioopm_list_t *old_list = old_merch->location;

            if (!ioopm_list_is_empty(old_merch->location))
            {
                ioopm_list_iterator_t *it = ioopm_list_iterator_create(old_list);

                while (ioopm_list_iterator_at_end(it))
                {
                    shelf_t *current_shelf = ioopm_list_iterator_current(it).p;
                    current_shelf->merch_name = new_name;
                    ioopm_list_iterator_advance(it);
                }
                ioopm_list_iterator_destroy(it);
            }

            ioopm_hash_table_insert(db->merch_ht, string_elem(new_name), ptr_elem(old_merch));

            free(old_merch_description);
            free(old_merch_name);

            return true;
        }
    }
    return false;
}

int ioopm_show_stock(data_base_t *db, char *merch)
{

    elem_t result;

    if (find_merch_shelf(db->merch_ht, merch, &result))
    {
        merch_t *current_merch = result.p;
        return current_merch->stock;
    }
    return 0;
}

bool ioopm_replenish(data_base_t *db, char *merch, int amount, char *shelf)
{
    // TODO: stub
    // vi måste fixa eftersom de pekar på samma struct och kanske lägga till hylla redan i början.
    elem_t result;
    if (find_merch_shelf(db->merch_ht, merch, &result))
    {
        merch_t *current_merch = result.p;

        ioopm_list_t *shelf_list = current_merch->location;

        elem_t result_2;
        if (find_merch_shelf(db->shelf_ht, shelf, &result_2))
        {
            shelf_t *shelf_occupied = result_2.p;

            // redan upptagen av en annan merch
            if (!(strcmp(shelf_occupied->merch_name, merch) == 0))
            {
                free(shelf);
                return false;
            }
            else
            {
                free(shelf);
                current_merch->stock += amount;
                shelf_occupied->quantity += amount;
            }
        }
        else
        {
            shelf_t *new_shelf = create_shelf(current_merch->name, amount, shelf);
            current_merch->stock += amount;
            ioopm_hash_table_insert(db->shelf_ht, string_elem(new_shelf->shelf), ptr_elem(new_shelf));
            ioopm_list_append(shelf_list, ptr_elem(new_shelf));
        }
        return true;
    }
    free(shelf);
    return false;
}

int ioopm_create_cart(data_base_t *db)
{
    int user_id = db->next_cart_id;
    ioopm_list_t *cart_list = ioopm_list_create();
    ioopm_hash_table_insert(db->cart_ht, int_elem(db->next_cart_id), ptr_elem(cart_list));
    db->next_cart_id += 1;

    return user_id;
}

void ioopm_print_cart(data_base_t *db, int user_id)
{
    elem_t result_cart;

    if (find_cart(db->cart_ht, user_id, &result_cart))
    {
        ioopm_list_t *cart_list = result_cart.p;

        if (ioopm_list_is_empty(cart_list))
        {
            printf("cart is empty");
        }
        ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(cart_list);
        size_t index = 1;
        while (ioopm_list_iterator_at_end(list_it))
        {
            elem_t current_item = ioopm_list_iterator_current(list_it);
            cart_item_t *current_cart_item = current_item.p;

            printf("%ld. Item: %s, Amount: %d \n", index, current_cart_item->merch->name, current_cart_item->quantity);
            ioopm_list_iterator_advance(list_it);
            index++;
        }
        ioopm_list_iterator_destroy(list_it);
        return;
    }
    printf("no cart was found with that id");
    return;
}

cart_item_t *ioopm_create_cart_item(merch_t *merch, int amount)
{
    cart_item_t *new_item = malloc(sizeof(cart_item_t));
    new_item->merch = merch;
    new_item->quantity = amount;

    return new_item;
}

bool ioopm_remove_cart(data_base_t *db, int user_id)
{
    elem_t result_cart;
    if (find_cart(db->cart_ht, user_id, &result_cart))
    {

        ioopm_list_t *user_list = result_cart.p;

        if (!ioopm_list_is_empty(user_list))
        {
            ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(user_list);
            while (ioopm_list_iterator_at_end(list_it))
            {
                elem_t current_item = ioopm_list_iterator_current(list_it);
                cart_item_t *current_cart_item = current_item.p;

                int quantity_to_remove = current_cart_item->quantity;
                current_cart_item->merch->reserved -= quantity_to_remove;

                ioopm_list_iterator_advance(list_it);
                free(current_cart_item);
            }
            ioopm_list_iterator_destroy(list_it);
        }
        ioopm_list_destroy(user_list);
        ioopm_hash_table_remove(db->cart_ht, int_elem(user_id));
        return true;
    }
    return false;
}

bool ioopm_add_to_cart(data_base_t *db, int user_id, char *merch, int amount)
{

    elem_t result;
    elem_t result_cart;

    if (!find_merch_shelf(db->merch_ht, merch, &result))
    {
        return false;
    }

    if (!find_cart(db->cart_ht, user_id, &result_cart))
    {
        return false;
    }

    merch_t *merch_add = result.p;

    if (amount <= 0 || amount > enough_stock(merch_add))
    {
        printf("Invalid amount or not enough stock\n");
        return false;
    }

    ioopm_list_t *user_list = result_cart.p;

    ioopm_list_iterator_t *cart_it = ioopm_list_iterator_create(user_list);
    while (ioopm_list_iterator_at_end(cart_it))
    {
        cart_item_t *current_cart_item = ioopm_list_iterator_current(cart_it).p;

        if (current_cart_item->merch == merch_add)
        {
            current_cart_item->quantity += amount;
            merch_add->reserved += amount;

            ioopm_list_iterator_destroy(cart_it);
            return true;
        }
        ioopm_list_iterator_advance(cart_it);
    }

    ioopm_list_iterator_destroy(cart_it);
    cart_item_t *new_item = ioopm_create_cart_item(merch_add, amount);

    ioopm_list_append(user_list, ptr_elem(new_item));
    merch_add->reserved += amount;

    return true;
}

bool ioopm_remove_from_cart(data_base_t *db, int user_id, char *merch, int amount)
{
    elem_t result;
    elem_t result_cart;
    if (!find_merch_shelf(db->merch_ht, merch, &result) || amount <= 0)
    {
        printf("Merch does not exist, or amount is invalid\n");
        return false;
    }

    if (!find_cart(db->cart_ht, user_id, &result_cart))
    {
        printf("No cart was found with the id\n");
        return false;
    }

    merch_t *merch_to_remove = result.p;
    ioopm_list_t *user_list = result_cart.p;

    size_t i = 0;
    while (i < ioopm_list_size(user_list))
    {
        cart_item_t *item = ioopm_list_get(user_list, i).p;

        if (item->merch == merch_to_remove)
        {
            if (item->quantity < amount)
            {
                printf("Not that many in cart\n");
                return false;
            }
            merch_to_remove->reserved -= amount;
            item->quantity -= amount;

            if (item->quantity == 0)
            {
                cart_item_t *removed_item =
                    ioopm_list_remove(user_list, i).p;
                free(removed_item);
            }
            return true;
        }
        i++;
    }
    return false;
}

int ioopm_calculate_cost(data_base_t *db, int user_id)
{
    elem_t result_cart;
    if (!find_cart(db->cart_ht, user_id, &result_cart))
    {
        printf("No cart was found with the id\n");
        return 0;
    }
    ioopm_list_t *user_list = result_cart.p;

    int total_cost = 0;
    if (ioopm_list_is_empty(user_list))
    {
        return total_cost;
    }
    ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(user_list);
    while (ioopm_list_iterator_at_end(list_it))
    {
        elem_t current_item = ioopm_list_iterator_current(list_it);
        cart_item_t *current_cart_item = current_item.p;
        total_cost += current_cart_item->merch->price * current_cart_item->quantity;
        ioopm_list_iterator_advance(list_it);
    }
    ioopm_list_iterator_destroy(list_it);
    return total_cost;
}

void ioopm_checkout(data_base_t *db, int user_id)
{
    elem_t result_cart;
    if (!find_cart(db->cart_ht, user_id, &result_cart))
    {
        printf("No cart was found with the id\n");
        return;
    }
    ioopm_list_t *user_list = result_cart.p;

    if (ioopm_list_is_empty(user_list))
    {
        ioopm_remove_cart(db, user_id);
        printf("Cart is empty");
        return;
    }
    int total_price = ioopm_calculate_cost(db, user_id);
    ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(user_list);
    while (ioopm_list_iterator_at_end(list_it))
    {
        elem_t current_item = ioopm_list_iterator_current(list_it);
        cart_item_t *current_cart_item = current_item.p;

        current_cart_item->merch->stock -= current_cart_item->quantity;

        ioopm_remove_from_shelf(db, current_cart_item->merch, current_cart_item->quantity);

        ioopm_list_iterator_advance(list_it);
    }

    ioopm_list_iterator_destroy(list_it);
    ioopm_remove_cart(db, user_id);
    printf("You are checked out! The total was %d", total_price);
}

void ioopm_cart_ht_remove(data_base_t *db)
{
    ioopm_hash_table_iterator_t *cart_it = ioopm_hash_table_iterator_create(db->cart_ht);

    while (!ioopm_hash_table_iterator_at_end(cart_it))
    {
        int key = ioopm_hash_table_iterator_current_key(cart_it).i;

        ioopm_hash_table_iterator_advance(cart_it);

        ioopm_remove_cart(db, key);
    }

    ioopm_hash_table_iterator_destroy(cart_it);
}

void ioopm_merch_ht_remove(data_base_t *db)
{
    ioopm_hash_table_iterator_t *merch_it = ioopm_hash_table_iterator_create(db->merch_ht);

    while (!ioopm_hash_table_iterator_at_end(merch_it))
    {
        char *key = ioopm_hash_table_iterator_current_key(merch_it).s;

        ioopm_hash_table_iterator_advance(merch_it);

        ioopm_remove_merchandise(db, key);
    }

    ioopm_hash_table_iterator_destroy(merch_it);
}

void ioopm_quit(data_base_t *db)
{

    ioopm_merch_ht_remove(db);
    ioopm_cart_ht_remove(db);

    ioopm_hash_table_destroy(db->cart_ht);
    ioopm_hash_table_destroy(db->shelf_ht);
    ioopm_hash_table_destroy(db->merch_ht);

    free(db);
}