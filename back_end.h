#pragma once
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "linked_list_iterator.h"
#include "hash_table_internal.h"
#include "linked_list_internal.h"
#include <stdio.h>

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct data_base data_base_t;
typedef struct cart_item cart_item_t;

struct merch
{
    char *name;
    char *description;
    int price;
    int stock;
    int reserved;

    ioopm_list_t *location;
};

struct shelf
{
    char *shelf;
    int quantity;
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

/// @brief Checks whether an integer is positive.
/// @param number The integer to check.
/// @return true if number is greater than zero; otherwise false.
bool ioopm_is_positive_number(int number);

/// @brief Creates an empty database and its merchandise, shelf, and cart tables.
/// To remove a data base: use ioopm_quit
/// @return A pointer to the newly allocated database.
data_base_t *ioopm_create_data_base(void);

/// @brief Creates a merchandise item unless an item with the same name already exists.
/// @pre db must point to an initialized database; name and description must be valid strings.
/// @param db The database in which to check for an existing item.
/// @param name The merchandise name.
/// @param description The merchandise description.
/// @param price The merchandise price.
/// @return A pointer to the newly allocated item, or NULL if the name is already in use.
/// the merch type owns its name and despcription.
merch_t *ioopm_create_merchandise(data_base_t *db, char *name, char *description, int price);

/// @brief Inserts a non-NULL merchandise item into the database, keyed by its name.
/// @pre db must point to an initialized database.
/// @param db The database in which to store the item.
/// @param merch The merchandise item to insert.
/// @return true if merch was non-NULL and inserted; otherwise false.
bool ioopm_add_merchandise(data_base_t *db, merch_t *merch);

/// @brief Prints up to 20 merchandise names in alphabetical order, starting at index.
/// @pre db must point to an initialized database.
/// @param db The database containing the merchandise.
/// @param index The zero-based index of the first name to print.
/// @return true if more names remain after this page; otherwise false.
bool ioopm_list_merchandise(data_base_t *db, size_t index);

/// @brief Prints the details of a merchandise item identified by name.
/// @pre db must point to an initialized database; merch must be a valid string.
/// @param db The database in which to search.
/// @param merch The name of the item to display.
/// @return true if the item exists and was displayed; otherwise false.
bool ioopm_list_one_merch(data_base_t *db, char *merch);

/// @brief Removes a merchandise item, its shelf entries, and its references in carts.
/// @pre db must point to an initialized database; merch must be a valid string.
/// @param db The database from which to remove the item.
/// @param merch The name of the item to remove.
/// @return true if the item existed and was removed; otherwise false.
bool ioopm_remove_merchandise(data_base_t *db, char *merch);

/// @brief Changes a merchandise item's name, description, and price.
/// @details The new name must not belong to another item. When the name changes,
///          associated shelf entries are updated as well.
/// @pre db must point to an initialized database; all names and description must be valid strings.
/// @param db The database containing the item.
/// @param old_name The item's current name.
/// @param new_name The replacement name.
/// @param description The replacement description.
/// @param price The replacement price.
/// @return true if the item was found and updated; otherwise false.
bool ioopm_edit_merchandise(data_base_t *db, char *old_name, char *new_name, char *description, int price);

/// @brief Gets the stock quantity for a named merchandise item.
/// @pre db must point to an initialized database; merch must be a valid string.
/// @param db The database in which to search.
/// @param merch The name of the item.
/// @return The item's stock quantity, or zero if the item does not exist.
int ioopm_show_stock(data_base_t *db, char *merch);

/// @brief Adds stock to a merchandise item at the specified shelf.
/// @details A shelf already assigned to a different item cannot be used.
/// @pre db must point to an initialized database; merch and shelf must be valid strings.
/// @param db The database whose inventory is updated.
/// @param merch The name of the item to replenish.
/// @param amount The quantity to add.
/// @param shelf The shelf identifier at which to store the stock.
/// @return true if stock was added; otherwise false, including when the item does not exist
///         or the shelf is assigned to another item.
bool ioopm_replenish(data_base_t *db, char *merch, int amount, char *shelf);

/// @brief Creates an empty cart and assigns it a new ID.
/// @pre db must point to an initialized database.
/// @param db The database in which to create the cart.
/// @return The ID assigned to the new cart.
size_t ioopm_create_cart(data_base_t *db);

/// @brief Prints the merchandise and quantities in a cart.
/// @pre db must point to an initialized database.
/// @param db The database containing the cart.
/// @param user_id The ID of the cart to print.
/// @return Nothing. Prints a message if the cart does not exist or is empty.
void ioopm_print_cart(data_base_t *db, int user_id);

/// @brief Allocates a cart item referring to a merchandise item and quantity.
/// @param merch The merchandise item referenced by the cart item.
/// @param amount The quantity stored in the cart item.
/// @return A pointer to the newly allocated cart item.
/// The type cart_item_t does not own the merch, it only points to the struct merch.
cart_item_t *ioopm_create_cart_item(merch_t *merch, int amount);

/// @brief Removes a quantity of stock from the merchandise item's shelves, starting at its first shelf.
/// @pre db must point to an initialized database; merch must have sufficient stock on its shelves.
/// @param db The database containing the shelf records.
/// @param merch The merchandise whose shelf quantities are reduced.
/// @param to_remove The quantity to remove.
/// @return Nothing.
void ioopm_remove_from_shelf(data_base_t *db, merch_t *merch, int to_remove);

/// @brief Removes a cart and releases the stock reserved by its items.
/// @pre db must point to an initialized database.
/// @param db The database containing the cart.
/// @param user_id The ID of the cart to remove.
/// @return true if the cart existed and was removed; otherwise false.
bool ioopm_remove_cart(data_base_t *db, int user_id);

/// @brief Adds a quantity of merchandise to a cart and reserves the corresponding stock.
/// @pre db must point to an initialized database; merch must be a valid string and amount positive.
/// @param db The database containing the cart and merchandise.
/// @param user_id The ID of the cart to update.
/// @param merch The name of the merchandise to add.
/// @param amount The positive quantity to add.
/// @return true if the item was added; otherwise false if the item or cart does not exist,
///         or there is not enough unreserved stock.
bool ioopm_add_to_cart(data_base_t *db, int user_id, char *merch, int amount);

/// @brief Removes a quantity of merchandise from a cart and releases its reserved stock.
/// @pre db must point to an initialized database; merch must be a valid string and amount positive.
/// @param db The database containing the cart and merchandise.
/// @param user_id The ID of the cart to update.
/// @param merch The name of the merchandise to remove.
/// @param amount The positive quantity to remove.
/// @return true if the quantity was removed; otherwise false if the item or cart does not exist,
///         the quantity is invalid, or the cart does not contain enough.
bool ioopm_remove_from_cart(data_base_t *db, int user_id, char *merch, int amount);

/// @brief Calculates the total price of all items in a cart.
/// @pre db must point to an initialized database.
/// @param db The database containing the cart.
/// @param user_id The ID of the cart to total.
/// @return The total price, or zero if the cart does not exist or is empty.
int ioopm_calculate_cost(data_base_t *db, int user_id);

/// @brief Completes checkout by deducting cart quantities from stock and removing the cart.
/// @pre db must point to an initialized database.
/// @param db The database containing the cart.
/// @param user_id The ID of the cart to check out.
/// @return Nothing. Prints a message if the cart does not exist; an empty cart is removed.
void ioopm_checkout(data_base_t *db, int user_id);

/// @brief Frees the database, its tables, and the merchandise and cart data they own.
/// @pre db must point to a database created by ioopm_create_data_base and must not be used afterward.
/// @param db The database to destroy.
/// @return Nothing.
void ioopm_quit(data_base_t *db);
