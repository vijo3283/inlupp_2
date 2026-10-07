#pragma once
#define No_Buckets 17
#include <string.h>

typedef struct entry entry_t;
typedef struct hash_table hash_table_t;
typedef struct hash_table_iterator hash_table_iterator_t;

struct entry
{
  elem_t key;
  elem_t value;
  entry_t *next;
};

struct hash_table
{
  entry_t buckets[No_Buckets];
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *key_eq_fn;
};

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  size_t current_bucket;
  entry_t *current_entry;
};