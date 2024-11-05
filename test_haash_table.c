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
#include <CUnit/CUnit.h>
#include "../linked_list/linked_list.h"
#include "../common.h"

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef struct option option_t;


void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}
// Vårt stora fel här var att vi insåg att vi försökte Asserta mot null när vi hade löst lookup genom en return som inte använde null.
void test_insert_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, NULL);
  elem_t key;
  elem_t key2;
  key.integer = 2;
  char *value = "hej";
  key2.integer = 100;

  option_t *result = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_TRUE(Unsuccessful(result));
  free(result);

  ioopm_int_str_ht_insert(ht, int_elem(key.integer), str_elem(value));
  option_t *result2 = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_TRUE(Successful(result2));
  CU_ASSERT_STRING_EQUAL(result2->value.p, value);
  free(result2);

  option_t *result3 = ioopm_hash_table_lookup(ht, key2);
  CU_ASSERT_TRUE(Unsuccessful(result3));
  CU_ASSERT_FALSE(Successful(result3))
  free(result3);
  
  ioopm_hash_table_destroy(ht);
}

void test_insert_duplicate_keys() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, NULL);
    CU_ASSERT_PTR_NOT_NULL(ht);

    elem_t key = int_elem(1);
    ioopm_int_str_ht_insert(ht, key, str_elem("first_value"));
    
    option_t *result = ioopm_hash_table_lookup(ht, key);
    CU_ASSERT_TRUE(result->success.boolean);
    CU_ASSERT_STRING_EQUAL(result->value.str, "first_value");
    free(result);

    // Insert again with the same key but different value
    ioopm_int_str_ht_insert(ht, key, str_elem("updated_value"));

    result = ioopm_hash_table_lookup(ht, key);
    CU_ASSERT_TRUE(result->success.boolean);
    CU_ASSERT_STRING_EQUAL(result->value.str, "updated_value");
    free(result);

    ioopm_hash_table_destroy(ht);
}


void test_insert_twice()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, str_eq_func, NULL);
  elem_t key;
  key.str = "two";
  char *value = "hej";
  option_t *result = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_FALSE(result->success.boolean);
  free(result);
  ioopm_int_str_ht_insert(ht, str_elem(key.str), str_elem(value));
  option_t *result2 = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_TRUE(result2->success.boolean);
  free(result2);
  ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, NULL);
  for (int i = 0; i < 18; ++i) /// 18 is a bit magical
  {
    elem_t key;
    key.integer = i;
    option_t *result = ioopm_hash_table_lookup(ht, key);
    CU_ASSERT_FALSE(result->success.boolean);
    free(result);
  }
  elem_t key2;
  key2.integer = -1;
  option_t *result2 = ioopm_hash_table_lookup(ht, key2);
  CU_ASSERT_FALSE(result2->success.boolean);
  free(result2);
  ioopm_hash_table_destroy(ht);
}

void test_remove()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, NULL);
  elem_t key;
  key.integer = 2;
  ioopm_int_str_ht_insert(ht, int_elem(key.integer), str_elem("hej"));
  ioopm_hash_table_remove(ht, key);
  option_t *result = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_FALSE(result->success.boolean);
  free(result);
  ioopm_hash_table_destroy(ht);
}

void test_size()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, NULL);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  elem_t key = int_elem(2);
  elem_t value = str_elem("hej");
  ioopm_int_str_ht_insert(ht, key, value);

  elem_t key2 = int_elem(3);
  elem_t value2 = str_elem("stroke");
  ioopm_int_str_ht_insert(ht, key2, value2);

  int result = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(2, result);
  ioopm_hash_table_destroy(ht);
}

void test_clear()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, NULL);
  elem_t key = int_elem(2);
  elem_t value = str_elem("hej");
  ioopm_int_str_ht_insert(ht, key, value);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_clear(ht);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}



void test_find_keys_values()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, str_eq_func);
  elem_t keys[5] = {int_elem(3), int_elem(10), int_elem(42), int_elem(0), int_elem(99)};
  elem_t value = str_elem(strdup("hej"));
  for (int i = 0; i < 5; i++)
  {
    ioopm_int_str_ht_insert(ht, keys[i], value);
  }
  ioopm_list_t *found_keys = ioopm_hash_table_keys(ht);
  CU_ASSERT_PTR_NOT_NULL(found_keys);

  bool key_found = false;
  for (int i = 0; i < 17; i++)
  { // Loop för ändra sanningsvärdet till true om vår valda key finns i arrayen från search key funktionen.
    if (found_keys->head->element.integer == keys[i].integer)
    {
      key_found = true;
      break;
    }
  }

  option_t *result2 = ioopm_hash_table_lookup(ht, int_elem(3)); // Dubbelkollar att rätt key finns enl lookup också.
  CU_ASSERT_TRUE(result2->success.boolean);
  free(result2);

  option_t *result = ioopm_hash_table_lookup(ht, int_elem(10)); // Dubbelkollar att en felaktig key inte fungerar.
  CU_ASSERT_TRUE(result->success.boolean);
  free(result);

  option_t *result3 = ioopm_hash_table_lookup(ht, int_elem(4)); // Dubbelkollar att en felaktig key inte fungerar.
  CU_ASSERT_FALSE(result3->success.boolean);
  free(result3);

  CU_ASSERT_TRUE(key_found); // Kollar om key found enl hash_table_key är sann.
  ioopm_linked_list_destroy(found_keys);

  // Here's the start of find values
  ioopm_list_t *found_values = ioopm_hash_table_values(ht);
  CU_ASSERT_PTR_NOT_NULL(found_values);

  for (int i = 0; i < 5; i++)
  {
    CU_ASSERT_STRING_EQUAL(found_values->head->element.str, value.str);
  }

  for (int i = 0; i < 5; i++)
  {
    link_t *current = found_values->head;
    while (current != NULL)
    {
      CU_ASSERT_STRING_EQUAL(current->element.str, value.str);
      current = current->next;
    }
  }
  ioopm_linked_list_destroy(found_values);
  free(value.str);
  ioopm_hash_table_destroy(ht);
}

void test_has_spec_key_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, str_eq_func);
  CU_ASSERT_PTR_NOT_NULL(ht);
  elem_t value_3 = str_elem("hello");
  elem_t string_copy = str_elem(strdup(value_3.str));

  ioopm_int_str_ht_insert(ht, int_elem(1), str_elem("hej"));
  ioopm_int_str_ht_insert(ht, int_elem(2), str_elem("tja"));
  ioopm_int_str_ht_insert(ht, int_elem(3), value_3);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(1)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(2)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(3)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(4)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(5)));

  bool debug = ioopm_hash_table_has_value(ht, str_elem("hej"));
  CU_ASSERT_TRUE(debug);
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, str_elem("hello")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, str_elem(string_copy.str)));
  free(string_copy.str);
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, str_elem("TJOTJABBATJENA")));
  

  ioopm_hash_table_destroy(ht);
}

void test_is_val_null()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, str_eq_func);
  CU_ASSERT_PTR_NOT_NULL(ht);
  elem_t value_3 = str_elem("hello");

  ioopm_int_str_ht_insert(ht, int_elem(1), str_elem("hej"));
  ioopm_int_str_ht_insert(ht, int_elem(2), str_elem("tja"));
  ioopm_int_str_ht_insert(ht, int_elem(3), value_3);
  ioopm_int_str_ht_insert(ht, int_elem(4), ptr_elem(NULL));

  CU_ASSERT_FALSE(ioopm_hash_table_is_null(ht));

  ioopm_hash_table_clear(ht);
  CU_ASSERT_TRUE(ioopm_hash_table_is_null(ht));

  ioopm_hash_table_destroy(ht);
}

void test_change_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, str_eq_func);
  CU_ASSERT_PTR_NOT_NULL(ht);

  ioopm_int_str_ht_insert(ht, int_elem(1), str_elem("hej"));
  ioopm_int_str_ht_insert(ht, int_elem(2), str_elem("tja"));

  int size = ioopm_hash_table_size(ht);

  for (int i = 1; i <= size; i++)
  {
    ioopm_int_str_ht_insert(ht, int_elem(i), str_elem("123test"));
    bool result = ioopm_hash_table_has_value(ht, str_elem("123test"));
    CU_ASSERT_TRUE(result);
  }

  ioopm_hash_table_clear(ht);
  ioopm_hash_table_destroy(ht);
}

void test_insert_key_string()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_str, str_eq_func, eq_func);

  ioopm_hash_table_insert(ht, str_elem("bombaclat"), int_elem(2));
  option_t *result = ioopm_hash_table_lookup(ht, str_elem("bombaclat"));
  CU_ASSERT_TRUE(result->success.boolean);
  free(result);
  ioopm_hash_table_destroy(ht);
}

void test_boolean_key_integer_value() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, eq_func);
    CU_ASSERT_PTR_NOT_NULL(ht);

    elem_t key_true = bool_elem(true);
    elem_t key_false = bool_elem(false);
    elem_t value = int_elem(42);

    ioopm_int_str_ht_insert(ht, key_true, value);
    option_t *result_true = ioopm_hash_table_lookup(ht, key_true);
    CU_ASSERT_TRUE(result_true->success.boolean);
    CU_ASSERT_EQUAL(result_true->value.integer, 42);

    option_t *result_false = ioopm_hash_table_lookup(ht, key_false);
    CU_ASSERT_FALSE(result_false->success.boolean);  // False key should not be found

    free(result_true);
    free(result_false);
    ioopm_hash_table_destroy(ht);
}

void test_string_key_boolean_value() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_str, str_eq_func, eq_func);
    CU_ASSERT_PTR_NOT_NULL(ht);

    elem_t key = str_elem("string_key");
    elem_t value_true = bool_elem(true);
    elem_t value_false = bool_elem(false);

    ioopm_int_str_ht_insert(ht, key, value_true);
    option_t *result = ioopm_hash_table_lookup(ht, key);
    CU_ASSERT_TRUE(Successful(result));
    CU_ASSERT_TRUE(result->value.boolean);
    free(result);


    ioopm_int_str_ht_insert(ht, key, value_false);
    result = ioopm_hash_table_lookup(ht, key);
    CU_ASSERT_TRUE(result->success.boolean);
    CU_ASSERT_FALSE(result->value.boolean);

    free(result);
    ioopm_hash_table_destroy(ht);
}


void test_apply_func() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_func, str_eq_func);
    CU_ASSERT_PTR_NOT_NULL(ht);

    // Insert test data into the hash table
    ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
    ioopm_hash_table_insert(ht, int_elem(2), int_elem(200));
    ioopm_hash_table_insert(ht, int_elem(3), int_elem(300));
    ioopm_hash_table_insert(ht, int_elem(4), int_elem(400));


    ioopm_increment_values(ht); // Increment factor is set in the function, in this case it's set to 5.

    // Verify the keys
    ioopm_list_t *values_list = ioopm_hash_table_values(ht); 
    size_t expected_values[] = {105, 205, 305, 405};  // Expected keys after incrementing by 5

    // Check the keys against expected values
    for (size_t i = 0; i < ioopm_linked_list_size(values_list); i++) { 
        elem_t value = ioopm_linked_list_get(values_list, i); 
        printf("Key %zu: %d\n", i, value.integer);

        // Check if the current key matches the expected key
        CU_ASSERT_EQUAL(value.integer, expected_values[i]);  
    }

    ioopm_linked_list_destroy(values_list); 
    ioopm_hash_table_destroy(ht);  
}



int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL)
  {
    // If the test suite could not be added, tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
      (CU_add_test(my_test_suite, "test create and destroy table", test_create_destroy) == NULL) ||
      (CU_add_test(my_test_suite, "test insert once", test_insert_once) == NULL) ||
      (CU_add_test(my_test_suite, "test lookup empty table", test_lookup_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Test remove bucket", test_remove) == NULL) ||
      (CU_add_test(my_test_suite, "Test the size of a HT", test_size) == NULL) ||
      (CU_add_test(my_test_suite, "Test to clear the HT", test_clear) == NULL) ||
      (CU_add_test(my_test_suite, "Test to find key or value", test_find_keys_values) == NULL) ||
      (CU_add_test(my_test_suite, "Test to find specific key and value", test_has_spec_key_value) == NULL) ||
      (CU_add_test(my_test_suite, "Test to check NULL value in all keys", test_is_val_null) == NULL) ||
      (CU_add_test(my_test_suite, "Test to change value", test_change_value) == NULL) ||
      (CU_add_test(my_test_suite, "Test insert string key", test_insert_key_string) == NULL) ||
      (CU_add_test(my_test_suite, "Test insert twice", test_insert_twice) == NULL) ||
      (CU_add_test(my_test_suite, "Test insert duplicate keys", test_insert_duplicate_keys) == NULL) ||
      (CU_add_test(my_test_suite, "Test string key boolean value", test_string_key_boolean_value) == NULL) ||
      (CU_add_test(my_test_suite, "Test boolean key integer value", test_boolean_key_integer_value) == NULL) ||
      (CU_add_test(my_test_suite, "Test apply function", test_apply_func) == NULL) ||
      0)
  {
    // If adding any of the tests fails, we tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
