#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_internal.h"

#define No_Buckets 17

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
 entry_t *new_entry = malloc(sizeof(entry_t));

 new_entry -> key = key;
 new_entry -> value = value;
 new_entry -> next = next;

 return new_entry;
}


static void entry_destroy(entry_t *entry){
 free(entry);
}


static entry_t *find_previous_entry(ioopm_hash_table_t *ht, elem_t key){
   /* 
   * Used to more easily utilize the pointers, makes functions such as remove and insert easier to handle becuase
   * we will be pointing to said element.
   */
   size_t bucket = ht->hash_fn(key) % No_Buckets;
   entry_t *previous = &ht->buckets[bucket];
    
   // Only runs if element exists and key is of correct type and correct key.
    while (previous->next != NULL && ht->key_eq_fn(previous->next->key, key) != 0)
    {
        previous = previous -> next;
    }
 return previous;
}


ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn) {
 /// Allocate zeroed-out space for a ioopm_hash_table_t = 17 pointers to entry_t's
 ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
 ht->hash_fn = hash_fn;
 ht->key_eq_fn = key_eq_fn;
 return ht;
}



void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
 for (size_t i = 0; i < No_Buckets; i++){
    entry_t *current = ht->buckets[i].next;

   // Destroys element piece by piece as it loops
    while (current != NULL){
        entry_t *next_node = current->next;
        entry_destroy(current);
        current = next_node;
        }
    }
 free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
 // find previous entry, or the last entry if the key does not exist
 entry_t *previous = find_previous_entry(ht, key);

 // if the key exists, update the value, otherwise create a new entry
 if (previous->next != NULL)
    {
        previous->next->value = value;
    }
 else
    {
        previous->next = entry_create(key, value, NULL);
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result){
 // look for an entry with the key we want
 entry_t *previous = find_previous_entry(ht, key);

 if (previous->next != NULL)
    {
    *result = previous -> next -> value;
    return true;
    }
 return false;
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key){
    entry_t *previous = find_previous_entry(ht, key);
    entry_t *to_remove = previous -> next;

    if (to_remove != NULL)
    {
        elem_t removed_value = to_remove -> value;
        // After removed value is decided we redirect the pointer
        previous -> next = to_remove -> next;
        entry_destroy(to_remove);
        return removed_value;
    }
 return ptr_elem(NULL);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key){
 entry_t *previous = find_previous_entry(ht, key);
 if (previous -> next != NULL)
    {
        return true;
    }
 return false;
}

static size_t bucket_size(entry_t *first_entry, size_t counter){
 if (first_entry != NULL)
   {
      return bucket_size(first_entry->next, counter + 1);
   }
   return counter;
}


size_t ioopm_hash_table_size(ioopm_hash_table_t *ht){
 size_t size = 0;
 for (size_t i = 0; i < No_Buckets; i++) 
 {
    size += bucket_size(ht->buckets[i].next, 0);
 }
 return size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht){
 size_t size = ioopm_hash_table_size(ht);
 if (size == 0)
 {
    return true;
 }
 return false;
}

bool comp_str(elem_t a, elem_t b)
{
    return strcmp(a.s, b.s);
}

bool comp_int(elem_t a, elem_t b)
{
    return !(a.i == b.i);
}

bool comp_bool(elem_t a, elem_t b)
{
    return !(a.b == b.b);
}

size_t string_hash(elem_t key)
{
 size_t result = 0;
 while (*key.s != '\0')
 {
    result = result * 31 + ((unsigned char) *key.s);
    key.s++;
 }
 return result;
}

size_t int_hash(elem_t key)
{
   return (size_t)(unsigned int)key.i;   
}

size_t bool_hash(elem_t key)
{
   return (size_t)(key.b);
}




