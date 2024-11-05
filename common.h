#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include "linked_list/linked_list.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdbool.h>
#include "iterator/iterator.h"
#ifndef COMMON_H
#define COMMON_H



#define no_buckets 17

typedef union elem elem_t;
typedef struct link link_t;
typedef struct list ioopm_list_t;
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef struct option option_t;
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);
typedef int (*ioopm_hash_function)(elem_t key);
bool eq_func(elem_t a, elem_t b);
bool str_eq_func(elem_t a, elem_t b);

typedef bool ioopm_predicate(ioopm_hash_table_t *ht, elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);

union elem
{
    int integer;
    bool boolean;
    float floater;
    unsigned int unint;
    void *p;
    char *str;
};

struct list
{
    link_t *head; // pekare till första linken
    link_t *tail; // pekare till sista noden
    size_t size;  // antalet noder
    ioopm_eq_function eq_fn;
};

typedef struct link
{
    elem_t element;    // vad som finns i noden
    struct link *next; // pekare till nästa nod
} link_t;

/// @brief
/// @param a
/// @param b
/// @return
bool compare_int_elements(elem_t a, elem_t b);

#define ioopm_int_str_ht_insert(ht, i, s) \
    ioopm_hash_table_insert(ht, i, s)


int extract_int_hash_key(elem_t key);
char *extract_char_hash_key(elem_t key);

#define int_elem(x) \
    (elem_t) { .integer = (x) }
#define ptr_elem(x) \
    (elem_t) { .p = (x) }
#define uint_elem(x) \
    (elem_t) { .unint = (x) }
#define bool_elem(x) \
    (elem_t) { .boolean = (x) }
#define float_elem(x) \
    (elem_t) { .floater = (x) }
#define str_elem(x) \
    (elem_t) { .str = (x) }

#define ptr_elem(x) \
    (elem_t) { .p = (x) }


#endif // COMMON_H
