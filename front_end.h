#pragma once
#include"back_end.h"
#include"hash_table.h"
#include"hash_table_iterator.h"
#include"linked_list.h"
#include"linked_list_iterator.h"
#include"hash_table_internal.h"
#include"linked_list_internal.h"


void print_menu(void);

bool confirmation_ui_yes(void);

bool confirmation_ui_no(void);

void add_merchandise_ui(data_base_t *db);

void list_merchandise_ui(data_base_t *db);

void list_one_merchandise_ui(data_base_t *db);

void remove_merchandise_ui(data_base_t *db);

void edit_merchandise_ui(data_base_t *db);

void show_stock_ui(data_base_t *db);

void replenish_ui(data_base_t *db);

void create_cart_ui(data_base_t *db);

void remove_cart_ui();

void add_to_cart_ui();

void remove_from_cart_ui();

void calculate_cost_ui();

void checkout_ui();

