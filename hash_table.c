#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include "hash_table.h"
#include <CUnit/Basic.h>
#include <string.h>
#include "linked_list.h"
#include "common.h"

bool eq_func(elem_t a, elem_t b)
{
  return a.integer == b.integer;
}

bool str_eq_func(elem_t a, elem_t b)
{
  // Kolla att båda är strängar
  if (a.str == NULL && b.str == NULL)
  {
    return true; // jämför strängar
  }
  if (a.str == NULL || b.str == NULL)
  {
    return false;
  }
  return strcmp(a.str, b.str) == 0;
}

int hash_function_str(elem_t key)
{
  char *str = key.str;
  int hash = 0;
  while (*str)
  {
    hash = hash * 31 + *str;
    str++;
  }
  return hash;
}

typedef struct option option_t;

int extract_int_hash_key(elem_t key)
{
  return key.integer;
}

char *extract_char_hash_key(elem_t key)
{
  return key.str;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function func, ioopm_eq_function eq_key, ioopm_eq_function eq_value)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  if (ht)
  {
    ht->func = func;
    ht->eq_key = eq_key;
    ht->eq_value = eq_value;
  }
  return ht;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  // if (ht == NULL) return;

  for (int i = 0; i < no_buckets; i++)
  {
    // ht har buckets 17 st vi vill gå igenom deras entry som heter next
    entry_t *entry_bucket = ht->buckets[i].next;

    while (entry_bucket != NULL)
    {
      entry_t *next_entry = entry_bucket->next;
      free(entry_bucket->value);
      free(entry_bucket);        // frigör entry bucket
      entry_bucket = next_entry; // vidare till nsäta
    }
  }
  free(ht);
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *new_ent = calloc(1, sizeof(entry_t));
  new_ent->key = key;
  new_ent->value = calloc(1, sizeof(elem_t));
  *new_ent->value = value;
  new_ent->next = next;
  return new_ent;
}

static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t key)
{
  entry_t *next = entry->next;

  while (next != NULL && !ht->eq_key(next->key, key))
  {
    entry = next;
    next = entry->next;
  }
  return entry;
}

option_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  // Hitta index för keyen
  int index = (ht->func(key) % no_buckets + no_buckets) % no_buckets;

  // Hitta den tidigare entryn för keyen vi vill titta på
  entry_t *tmp = find_previous_entry_for_key(ht, &ht->buckets[index], key);
  entry_t *current = tmp ? tmp->next : NULL;

  option_t *result = calloc(1, sizeof(option_t));
  if (!result)
  {
    return NULL;
  }
  if (current && ht->eq_key(current->key, key))
  {

    *result = Success(*current->value);
  }
  else
  {

    *result = Failure();
  }

  return result; // Returnera pekaren till option_t structen
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  int bucket_index = (ht->func(key) % no_buckets + no_buckets) % no_buckets;
  if (bucket_index < 0)
  {
    bucket_index += no_buckets;
  }
  entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket_index], key);
  entry_t *next = entry->next;

  /// Kolla om nästa entry ska uppdaters eller inte
  if (next && ht->eq_key(next->key, key))
  {
    *(next->value) = value; // Uppdaterar värdet om key redan fanns
  }
  else
  {
    entry->next = entry_create(key, value, next);
  }
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = (ht->func(key) % no_buckets + no_buckets) % no_buckets;
  entry_t *tmp = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next_adj;
  elem_t value;
  while (tmp != NULL && tmp->next != NULL)
  {
    next_adj = tmp->next->next; // ger oss den vi tar bort pekar på
    // next_adj är den vi vill länka till tmp
    // next_adj är den som vi tar borts neighbour
    value = *(tmp->next->value);
    free(tmp->next->value);
    free(tmp->next);
    tmp->next = next_adj;
  }
  return value;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t counter = 0;
  for (int i = 0; i < no_buckets; i++)
  {
    entry_t *entry_bucket = ht->buckets[i].next;
    while (entry_bucket != NULL)
    {
      counter++;
      entry_bucket = entry_bucket->next;
    }
  }
  return counter;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < no_buckets; i++)
  {
    // ht har buckets 17 st vi vill gå igenom deras entry som heter next
    entry_t *entry_bucket = ht->buckets[i].next;

    while (entry_bucket != NULL)
    {
      entry_t *next_entry = entry_bucket->next;
      free(entry_bucket->value);
      free(entry_bucket);
      entry_bucket = next_entry;
    }
    ht->buckets[i].next = NULL;
  }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *key_list = ioopm_linked_list_create(eq_func); // Skapa en lista för keysen
  for (size_t i = 0; i < no_buckets; i++)
  {
    entry_t *entry_bucket = ht->buckets[i].next;
    while (entry_bucket)
    {
      ioopm_linked_list_append(key_list, entry_bucket->key); // Lägg in keysen i listan
      entry_bucket = entry_bucket->next;                     // Iterera vidare i listan
    }
  }
  return key_list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *values_list = ioopm_linked_list_create(eq_func);

  for (size_t i = 0; i < no_buckets; i++)
  {
    entry_t *entry = ht->buckets[i].next;
    while (entry != NULL)
    {
      ioopm_linked_list_append(values_list, *entry->value);
      entry = entry->next;
    }
  }
  return values_list;
}

bool key_equiv(ioopm_hash_table_t *ht, elem_t key, elem_t value_ignored, void *x)
{
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return ht->eq_key(key, other_key);
}

bool val_equiv(ioopm_hash_table_t *ht, elem_t key_ignored, elem_t value, void *x)
{
  elem_t *other_value = x;
  return ht->eq_value(value, *other_value);
}

static bool val_null_check(ioopm_hash_table_t *ht, elem_t key_ignored, elem_t value, void *x_ignored)
{
  if (value.str)
  {
    return false;
  }
  return true;
}

static void change_val(elem_t key_ignored, elem_t *value, void *arg_ignored)
{
  value->str = "123test";
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  option_t *tmp = ioopm_hash_table_lookup(ht, key);
  bool result = tmp->success.boolean;
  free(tmp);
  return result;
}

void ioopm_hash_table_change_val(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_apply_to_all(ht, change_val, NULL);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, val_equiv, &value);
}

bool ioopm_hash_table_is_null(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_all(ht, val_null_check, NULL);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg)
{
  entry_t *entry;
  for (int i = 0; i < no_buckets; i++)
  {
    entry = &ht->buckets[i];
    while (entry->next != NULL)
    {
      entry = entry->next;
      if (pred(ht, entry->key, *entry->value, arg))
      {
        return true;
      }
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg)
{
  if (ht == NULL || pred == NULL)
  {
    return false;
  }

  size_t size = ioopm_hash_table_size(ht);

  entry_t *entry;

  for (int i = 0; i < size; i++)
  {
    entry = &ht->buckets[i];
    while (entry->next != NULL)
    {
      entry = entry->next;
      if (!pred(ht, entry->key, *entry->value, arg))
      {
        return false;
      }
    }
  }

  return true;
}

// void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg)
// {
//   size_t size = ioopm_hash_table_size(ht);
//   entry_t *entry;
//   for (int i = 0; i <= size; i++)
//   {
//     entry = &ht->buckets[i];
//     while (entry->next != NULL)
//     {

//       apply_fun(entry->key, entry->value, arg);
//       entry = entry->next;
//     }
//   }
// }
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  for (int i = 0; i < no_buckets; ++i)
  {
    entry_t *current_entry = &ht->buckets[i];

    while (current_entry != NULL)
    {
      apply_fun(current_entry->key, current_entry->value, arg);

      current_entry = current_entry->next;
    }
  }
}
