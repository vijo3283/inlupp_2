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
