#pragma once
#include "back_end.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "linked_list_iterator.h"
#include "hash_table_internal.h"
#include "linked_list_internal.h"

/// @brief Prints the available merchandise, inventory, and shopping-cart commands.
void print_menu(void);

/// @brief Prompts the user to confirm an action.
/// @return true if the user's response begins with 'Y' or 'y'; otherwise false.
bool confirmation_ui_yes(void);

/// @brief Prompts the user to decline or stop an action.
/// @return true if the user's response begins with 'N' or 'n'; otherwise false.
bool confirmation_ui_no(void);

/// @brief Prompts for merchandise details and adds the merchandise to the database.
/// @param db The initialized database to which the merchandise is added.
void add_merchandise_ui(data_base_t *db);

/// @brief Displays merchandise in pages, prompting before displaying each next page.
/// @param db The initialized database containing the merchandise.
void list_merchandise_ui(data_base_t *db);

/// @brief Prompts for a merchandise name and displays its details.
/// @param db The initialized database in which to look up the merchandise.
void list_one_merchandise_ui(data_base_t *db);

/// @brief Prompts for a merchandise name and asks for confirmation before removing it.
/// @param db The initialized database from which the merchandise may be removed.
void remove_merchandise_ui(data_base_t *db);

/// @brief Prompts for a merchandise item and its replacement name, description, and price.
/// @details Applies the changes only if the user confirms and the backend edit succeeds.
/// @param db The initialized database containing the merchandise to edit.
void edit_merchandise_ui(data_base_t *db);

/// @brief Prompts for a merchandise name and displays its current stock quantity.
/// @param db The initialized database containing the merchandise.
void show_stock_ui(data_base_t *db);

/// @brief Prompts for a shelf, merchandise name, and positive quantity to replenish.
/// @param db The initialized database whose inventory is replenished.
void replenish_ui(data_base_t *db);

/// @brief Creates a shopping cart and displays its assigned ID.
/// @param db The initialized database in which to create the cart.
void create_cart_ui(data_base_t *db);

/// @brief Prompts for a cart ID and displays the contents of that cart.
/// @param db The initialized database containing the cart.
void print_cart_ui(data_base_t *db);

/// @brief Prompts for a cart ID and asks for confirmation before removing that cart.
/// @param db The initialized database containing the cart.
void remove_cart_ui(data_base_t *db);

/// @brief Prompts for a cart ID, merchandise name, and positive quantity to add.
/// @param db The initialized database containing the cart and merchandise.
void add_to_cart_ui(data_base_t *db);

/// @brief Prompts for a cart ID, merchandise name, and positive quantity to remove.
/// @param db The initialized database containing the cart and merchandise.
void remove_from_cart_ui(data_base_t *db);

/// @brief Prompts for a cart ID and displays the total cost of its contents.
/// @param db The initialized database containing the cart.
void calculate_cost_ui(data_base_t *db);

/// @brief Prompts for a cart ID and checks out that cart.
/// @param db The initialized database containing the cart.
void checkout_ui(data_base_t *db);

/// @brief Asks the user to confirm quitting and frees the database if confirmed.
/// @param db The initialized database to free if the user confirms.
/// @return true if quitting was confirmed; otherwise false.
bool quit_ui(data_base_t *db);
