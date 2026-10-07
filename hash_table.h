#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"


/**
* @file hash_table.h
* @author Lina Holsti, Elis Lemon
* @date 14/9-26
* @brief Simple hash table that maps keys to values.
*
* Hash Tables provide a key-value mapping interface that allows keys to be searched, added, updated, and removed. 
* Both the keys and the values are of type elem_t, meaning they can store integers, strings, booleans or pointers. 
* A hash table relies on custom hash and equality functions provided at creation to handle generic keys.
* A hashtable can be empty
*
*/

typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief checks if a key exists in a hash table
/// @param ht hash table operated upon
/// @param key key to check
/// @return a boolean if the key exists or not
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief checks if a hash table is empty
/// @param ht hash table operated upon
/// @return a boolean if the hash table is empty or not
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief checks the size of a hash table
/// @param ht hash table operated upon
/// @return a positive integer of the hash table size
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief compares two element to see if they are the same
/// @param a elem 1
/// @param b elem 2
/// @return true if the elements are the same, false otherwise
bool comp_str(elem_t a, elem_t b);

/// @brief compares two element to see if they are the same
/// @param a elem 1
/// @param b elem 2
/// @return true if the elements are the same, false otherwise
bool comp_int(elem_t a, elem_t b);

/// @brief compares two element to see if they are the same
/// @param a elem 1
/// @param b elem 2
/// @return true if the elements are the same, false otherwise
bool comp_bool(elem_t a, elem_t b);

/// @brief hashes an element "key" of type string
/// @param key to hash
/// @return integer > 0 to hash to a bucket
size_t string_hash(elem_t key);

/// @brief hashes an element "key" of type int
/// @param key to hash
/// @return integer > 0 to hash to a bucket
size_t int_hash(elem_t key);

/// @brief hashes an element "key" of type bool
/// @param key to hash
/// @return integer > 0 to hash to a bucket
size_t bool_hash(elem_t key);