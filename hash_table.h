#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include "linked_list.h"
#include "common.h"

/**
 * @file hash_table.h
 * @author Felicia Fälldin, William Ewerblad
 * @date Sep 2024
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

bool val_equiv(ioopm_hash_table_t *ht, elem_t key_ignored, elem_t value, void *x);
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef bool ioopm_predicate(ioopm_hash_table_t *ht, elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);
bool eq_func(elem_t a, elem_t b);
bool str_eq_func(elem_t a, elem_t b);
int hash_function_str(elem_t key);

#define Success(v) \
  (option_t) { .success.boolean = true, .value = v }
#define Failure() \
  (option_t) { .success = false }

#define Successful(o) ((o)->success.boolean == true)

#define Unsuccessful(o) ((o)->success.boolean == false)

typedef struct entry
{
  elem_t key;         // Key for the hash table entry
  elem_t *value;      // Value for the hash table entry
  struct entry *next; // Pointer to the next entry (linked list)
} entry_t;

struct hash_table
{
  entry_t buckets[no_buckets]; // Buckets blir en array av bucket pointers
  ioopm_hash_function func;
  ioopm_eq_function eq_key;
  ioopm_eq_function eq_value;
};

typedef struct option
{
  elem_t success;
  elem_t value;
} option_t;

/// @brief Jämför två strängar och kollar ekvivalens
/// @param a Sträng 1
/// @param b Sträng 2
/// @return boolean value, true om equal annars false.
bool str_eq_func(elem_t a, elem_t b);

/// @brief Jämför två integers och kollar ekvivalens
/// @param a int 1
/// @param b int 2
/// @return  boolean value, true om equal annars false
bool eq_func(elem_t a, elem_t b);

/// @brief Gör key variabeln i vår ht till en integer
/// @param key
/// @return returns key as type integer
int extract_int_hash_key(elem_t key);

char *extract_char_hash_key(elem_t key);

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function func, ioopm_eq_function eq_key, ioopm_eq_function eq_value);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

// We chose option 1 therefore using option_t *. By allocating space and
// assigning this space with a temp containing the value, it was done using the same signature of the function.
/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key (FIXME: incomplete)
option_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key (FIXME: incomplete)
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return an array of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

///@brief Null check for all values in ht.
///@param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_is_null(ioopm_hash_table_t *ht);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg);

/// @brief Changes the value in the current bucket
/// @param ht
void ioopm_hash_table_change_val(ioopm_hash_table_t *ht);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg);

///@brief Prints out keys and values in a hash table
///@param ht hash table operated upon
void ioopm_hash_table_print(ioopm_hash_table_t *ht);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg);

///@brief compares 2 keys
///@param key
///@param value_ignored
///@param x
///@return boolean value. If keys are equal, true otherwise false
bool key_equiv(ioopm_hash_table_t *ht, elem_t key, elem_t value_ignored, void *x);
