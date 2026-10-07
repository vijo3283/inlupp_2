#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hash_table_iterator.h"
#include "hash_table_internal.h"


static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
  // advances to the next entry
  it->current_entry = it->current_entry->next;

  // if it was null advance to the next bucket
  if (it->current_entry == NULL)
  {
    it->current_bucket += 1;

    // if the next bucket existed, update the current entry
    if (it->current_bucket != No_Buckets)
    {
      it->current_entry = &it->ht->buckets[it->current_bucket];
    }
    
  }
}

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it)
{
  while (it->current_bucket != No_Buckets &&
        // Checks if we are on a sentinel node, if so we advance.  
         it->current_entry == &it->ht->buckets[it->current_bucket])
  {
    advance_iterator_state(it);
  }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_iterator_t *it = calloc(1, sizeof(ioopm_hash_table_iterator_t));
  it->ht = ht;
  it->current_bucket = 0;
  it->current_entry = &ht->buckets[0];
  // Hash table creates sentinel node at start, hence the skip so that it is not utilized here
  skip_sentinel_nodes(it);
  return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it){
  free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it){
  return it->current_bucket == No_Buckets;
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it){
  if (!ioopm_hash_table_iterator_at_end(it))
  {
  advance_iterator_state(it);
  skip_sentinel_nodes(it);
  }
}

elem_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it){
  return it->current_entry->key;
}

elem_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it){
  return it->current_entry->value;
}