#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include "back_end.h"

/* Input strings are heap-allocated, as with ask_question_string().
   Successful creation/edit/replenish may transfer ownership to backend. */
static char *copy_string(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static merch_t *add_merch(data_base_t *db, const char *name, const char *desc, int price)
{
    char *n = copy_string(name);
    char *d = copy_string(desc);
    if (!n || !d) { free(n); free(d); return NULL; }
    merch_t *m = ioopm_create_merchandise(db, n, d, price);
    if (!m) { free(n); free(d); return NULL; }
    if (!ioopm_add_merchandise(db, m)) {
        /* Unexpected with these unique test names; creation succeeded. */
        return NULL;
    }
    return m;
}

static merch_t *find_merch(data_base_t *db, const char *name)
{
    elem_t value;
    return ioopm_hash_table_lookup(db->merch_ht, string_elem((char *)name), &value)
           ? value.p : NULL;
}

static shelf_t *find_shelf(data_base_t *db, const char *name)
{
    elem_t value;
    return ioopm_hash_table_lookup(db->shelf_ht, string_elem((char *)name), &value)
           ? value.p : NULL;
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void test_create_and_remove_merch(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *m = add_merch(db, "Cykel", "Rod", 150);
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_PTR_EQUAL(find_merch(db, "Cykel"), m);
    CU_ASSERT_EQUAL(m->price, 150);
    CU_ASSERT_EQUAL(m->stock, 0);
    CU_ASSERT_EQUAL(m->reserved, 0);
    CU_ASSERT_TRUE(ioopm_list_one_merch(db, "Cykel"));
    CU_ASSERT_FALSE(ioopm_list_one_merch(db, "Saknas"));
    CU_ASSERT_EQUAL(ioopm_show_stock(db, "Cykel"), 0);
    CU_ASSERT_FALSE(ioopm_remove_merchandise(db, "Saknas"));
    CU_ASSERT_TRUE(ioopm_remove_merchandise(db, "Cykel"));
    CU_ASSERT_PTR_NULL(find_merch(db, "Cykel"));
    ioopm_quit(db);
}

void test_list_merchandise(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);

    add_merch(db, "Cykel", "uppsala", 150);
    add_merch(db, "Bil", "fordon", 200);
    add_merch(db, "Ananas", "frukt", 20);

    CU_ASSERT_FALSE(ioopm_list_merchandise(db, 0));

    ioopm_quit(db);
}

void test_remove_non_existant_merchandise(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);

    add_merch(db, "Cykel", "uppsala", 150);
    add_merch(db, "Bil", "fordon", 200);
    add_merch(db, "Ananas", "frukt", 20);
    
    bool been_removed = ioopm_remove_merchandise(db, "hej");

    CU_ASSERT_FALSE(been_removed);

    ioopm_quit(db);
}

void test_carts(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);

    merch_t *cykel = add_merch(db, "Cykel", "uppsala", 150);
    CU_ASSERT_PTR_NOT_NULL_FATAL(cykel);

    ioopm_replenish(db, "Cykel", 4, copy_string("A20"));

    int cart = ioopm_create_cart(db);

    // Lägg till samma vara två gånger
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, cart, "Cykel", 1));
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, cart, "Cykel", 1));
    CU_ASSERT_EQUAL(cykel->reserved, 2);

    // Ogiltiga tillägg
    CU_ASSERT_FALSE(ioopm_add_to_cart(db, 999, "Cykel", 1));
    CU_ASSERT_FALSE(ioopm_add_to_cart(db, cart, "Saknas", 1));
    CU_ASSERT_FALSE(ioopm_add_to_cart(db, cart, "Cykel", 0));

    // Beräkna kostnad
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, cart), 300);
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, 999), 0);

    // Skriv ut kundvagnar
    ioopm_print_cart(db, cart);
    ioopm_print_cart(db, 999);

    // Ogiltig borttagning
    CU_ASSERT_FALSE(ioopm_remove_from_cart(db, 999, "Cykel", 1));
    CU_ASSERT_FALSE(ioopm_remove_from_cart(db, cart, "Saknas", 1));

    // Checkout på obefintlig och tom kundvagn
    ioopm_checkout(db, 999);
    int empty = ioopm_create_cart(db);
    ioopm_print_cart(db, empty);
    ioopm_checkout(db, empty);

    ioopm_quit(db);
}


void test_edit_stock_replenish_false(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);

    add_merch(db, "Cykel", "uppsala", 150);
    add_merch(db, "Bil", "fordon", 200);
    add_merch(db, "Ananas", "frukt", 20);

    int stock = ioopm_show_stock(db, "Äpple");
    CU_ASSERT_EQUAL(stock, 0);

    bool been_replenished =
        ioopm_replenish(db, "Banan", 3, copy_string("A37"));

    CU_ASSERT_FALSE(been_replenished);

    char *new_name = copy_string("Bil");
    char *new_desc = copy_string("lördag");

    bool been_edited =
        ioopm_edit_merchandise(db, "Cykel", new_name, new_desc, 3);

    CU_ASSERT_FALSE(been_edited);

    free(new_name);
    free(new_desc);

    ioopm_quit(db);
}

void test_duplicate_merch(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    CU_ASSERT_PTR_NOT_NULL_FATAL(add_merch(db, "A", "Original", 10));
    char *n = copy_string("A");
    char *d = copy_string("Dublett");
    CU_ASSERT_PTR_NOT_NULL_FATAL(n);
    CU_ASSERT_PTR_NOT_NULL_FATAL(d);
    merch_t *duplicate = ioopm_create_merchandise(db, n, d, 20);
    CU_ASSERT_PTR_NULL(duplicate);
    if (!duplicate) { free(n); free(d); }
    CU_ASSERT_EQUAL(find_merch(db, "A")->price, 10);
    ioopm_quit(db);
}

void test_replenish_shelves(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *a = add_merch(db, "A", "Vara A", 10);
    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(add_merch(db, "B", "Vara B", 20));
    CU_ASSERT_TRUE(ioopm_replenish(db, "A", 5, copy_string("A01")));
    CU_ASSERT_TRUE(ioopm_replenish(db, "A", 3, copy_string("A02")));
    CU_ASSERT_EQUAL(a->stock, 8);
    shelf_t *s = find_shelf(db, "A01");
    CU_ASSERT_PTR_NOT_NULL_FATAL(s);
    CU_ASSERT_EQUAL(s->quantity, 5);
    /* Current replenish implementation frees an incoming duplicate shelf name. */
    CU_ASSERT_TRUE(ioopm_replenish(db, "A", 2, copy_string("A01")));
    CU_ASSERT_EQUAL(s->quantity, 7);
    CU_ASSERT_FALSE(ioopm_replenish(db, "B", 1, copy_string("A01")));
    CU_ASSERT_EQUAL(s->quantity, 7);
    ioopm_quit(db);
}

void test_remove_from_shelves(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *m = add_merch(db, "A", "Test", 10);
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_TRUE(ioopm_replenish(db, "A", 3, copy_string("A01")));
    CU_ASSERT_TRUE(ioopm_replenish(db, "A", 4, copy_string("A02")));
    ioopm_remove_from_shelf(db, m, 5);
    m->stock -= 5; /* remove_from_shelf does not modify merch->stock. */
    CU_ASSERT_PTR_NULL(find_shelf(db, "A01"));
    shelf_t *s = find_shelf(db, "A02");
    CU_ASSERT_PTR_NOT_NULL_FATAL(s);
    CU_ASSERT_EQUAL(s->quantity, 2);
    CU_ASSERT_EQUAL(m->stock, 2);
    ioopm_quit(db);
}

void test_cart_add_and_remove(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *m = add_merch(db, "Lampa", "LED", 100);
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_TRUE(ioopm_replenish(db, "Lampa", 10, copy_string("L01")));
    int id = ioopm_create_cart(db);
    CU_ASSERT_TRUE(id > 0);
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, id, "Lampa", 4));
    CU_ASSERT_EQUAL(m->reserved, 4);
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, id), 400);
    CU_ASSERT_FALSE(ioopm_add_to_cart(db, id, "Lampa", 7));
    CU_ASSERT_TRUE(ioopm_remove_from_cart(db, id, "Lampa", 2));
    CU_ASSERT_EQUAL(m->reserved, 2);
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, id), 200);
    CU_ASSERT_FALSE(ioopm_remove_from_cart(db, id, "Lampa", 3));
    CU_ASSERT_TRUE(ioopm_remove_cart(db, id));
    CU_ASSERT_EQUAL(m->reserved, 0);
    CU_ASSERT_FALSE(ioopm_remove_cart(db, id));
    ioopm_quit(db);
}

void test_checkout_multiple_shelves(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *m = add_merch(db, "Batteri", "36V", 200);
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_TRUE(ioopm_replenish(db, "Batteri", 3, copy_string("B01")));
    CU_ASSERT_TRUE(ioopm_replenish(db, "Batteri", 4, copy_string("B02")));
    int id = ioopm_create_cart(db);
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, id, "Batteri", 5));
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, id), 1000);
    ioopm_checkout(db, id);
    CU_ASSERT_EQUAL(m->stock, 2);
    CU_ASSERT_EQUAL(m->reserved, 0);
    CU_ASSERT_PTR_NULL(find_shelf(db, "B01"));
    shelf_t *s = find_shelf(db, "B02");
    CU_ASSERT_PTR_NOT_NULL_FATAL(s);
    CU_ASSERT_EQUAL(s->quantity, 2);
    elem_t result;
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(db->cart_ht, int_elem(id), &result));
    ioopm_quit(db);
}

void test_edit_name_and_description(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *m = add_merch(db, "Gammal", "Fore", 30);
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_TRUE(ioopm_replenish(db, "Gammal", 2, copy_string("G01")));
    CU_ASSERT_TRUE(ioopm_edit_merchandise(db, "Gammal", copy_string("Ny"), copy_string("Efter"), 40));
    CU_ASSERT_PTR_NULL(find_merch(db, "Gammal"));
    CU_ASSERT_PTR_EQUAL(find_merch(db, "Ny"), m);
    CU_ASSERT_STRING_EQUAL(m->description, "Efter");
    CU_ASSERT_EQUAL(m->price, 40);
    shelf_t *s = find_shelf(db, "G01");
    CU_ASSERT_PTR_NOT_NULL_FATAL(s);
    CU_ASSERT_STRING_EQUAL(s->merch_name, "Ny");
    CU_ASSERT_TRUE(ioopm_edit_merchandise(db, "Ny", copy_string("Ny"), copy_string("Beskr"), 50));
    CU_ASSERT_STRING_EQUAL(m->description, "Beskr");
    CU_ASSERT_EQUAL(m->price, 50);
    ioopm_quit(db);
}

void test_edit_missing_merch(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    char *n = copy_string("Nytt");
    char *d = copy_string("Beskrivning");
    CU_ASSERT_FALSE(ioopm_edit_merchandise(db, "Saknas", n, d, 10));
    free(n);
    free(d);
    ioopm_quit(db);
}

void test_remove_merch_from_multiple_carts(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    merch_t *a = add_merch(db, "A", "A-desc", 10);
    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(add_merch(db, "B", "B-desc", 20));
    CU_ASSERT_TRUE(ioopm_replenish(db, "A", 8, copy_string("A11")));
    CU_ASSERT_TRUE(ioopm_replenish(db, "B", 2, copy_string("B11")));
    int c1 = ioopm_create_cart(db);
    int c2 = ioopm_create_cart(db);
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, c1, "A", 2));
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, c2, "A", 3));
    CU_ASSERT_TRUE(ioopm_add_to_cart(db, c2, "B", 1));
    CU_ASSERT_EQUAL(a->reserved, 5);
    CU_ASSERT_TRUE(ioopm_remove_merchandise(db, "A"));
    CU_ASSERT_PTR_NULL(find_merch(db, "A"));
    CU_ASSERT_PTR_NULL(find_shelf(db, "A11"));
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, c1), 0);
    CU_ASSERT_EQUAL(ioopm_calculate_cost(db, c2), 20);
    ioopm_quit(db);
}

void test_quit_empty_database(void)
{
    data_base_t *db = ioopm_create_data_base();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    ioopm_quit(db);
}

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();
    CU_pSuite suite = CU_add_suite("Back End Tests", init_suite, clean_suite);
    if (!suite) { CU_cleanup_registry(); return CU_get_error(); }

    if (CU_add_test(suite, "Create and remove merchandise", test_create_and_remove_merch) == NULL ||
        CU_add_test(suite, "Duplicate merchandise", test_duplicate_merch) == NULL ||
        CU_add_test(suite, "Replenish shelves", test_replenish_shelves) == NULL ||
        CU_add_test(suite, "Remove across shelves", test_remove_from_shelves) == NULL ||
        CU_add_test(suite, "Cart add and remove", test_cart_add_and_remove) == NULL ||
        CU_add_test(suite, "Checkout across shelves", test_checkout_multiple_shelves) == NULL ||
        CU_add_test(suite, "Edit name and description", test_edit_name_and_description) == NULL ||
        CU_add_test(suite, "Edit missing merchandise", test_edit_missing_merch) == NULL ||
        CU_add_test(suite, "Remove merchandise from carts", test_remove_merch_from_multiple_carts) == NULL ||
        CU_add_test(suite, "Quit empty database", test_quit_empty_database) == NULL ||
        CU_add_test(suite, "list merchandise", test_list_merchandise) == NULL ||
        CU_add_test(suite, "remove non existant", test_remove_non_existant_merchandise) == NULL ||
        CU_add_test(suite, "edit, show stock, replensish, not right", test_edit_stock_replenish_false) == NULL ||
        CU_add_test(suite, "test on carts", test_carts) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    return failures ? 1 : 0;
}
