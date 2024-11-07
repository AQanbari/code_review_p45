#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include "linked_list.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

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

void test_create_destroy_LL()
{
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_destroy(list);
}

void test_append_list()
{
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  CU_ASSERT_PTR_NOT_NULL(list);

  ioopm_linked_list_append(list, int_elem(10));
  ioopm_linked_list_append(list, int_elem(100));

  CU_ASSERT_PTR_NOT_NULL(list->tail);
  CU_ASSERT_EQUAL(list->tail->element.integer, 100);
  CU_ASSERT_EQUAL(list->size, 2);
  CU_ASSERT_PTR_NULL(list->tail->next);
  CU_ASSERT_EQUAL(list->head->next->element.integer, 100);

  ioopm_linked_list_destroy(list);

  ioopm_list_t *list2 = ioopm_linked_list_create(NULL);
  CU_ASSERT_PTR_NOT_NULL(list2);
  ioopm_linked_list_append(list2, int_elem(123));
  CU_ASSERT_EQUAL(list2->tail->element.integer, 123);

  ioopm_linked_list_destroy(list2);
}

void test_prepend_list(){
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  CU_ASSERT_PTR_NOT_NULL(list);

  ioopm_linked_list_prepend(list, int_elem(1));
  CU_ASSERT_EQUAL(list->head->element.integer, 1);
  CU_ASSERT_EQUAL(list->size, 1)

  ioopm_linked_list_prepend(list, int_elem(2));
  CU_ASSERT_EQUAL(list->head->element.integer, 2);
  CU_ASSERT_EQUAL(list->tail->element.integer, 1);

  ioopm_linked_list_destroy(list);
}

//head i listan har index 0
void test_list_insert(){
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_append(list, int_elem(1));
  ioopm_linked_list_append(list, int_elem(2));
  ioopm_linked_list_append(list, int_elem(3));
  ioopm_linked_list_append(list, int_elem(4));
  CU_ASSERT_EQUAL(list->size, 4);


  ioopm_linked_list_insert(list, 2, int_elem(5));
  CU_ASSERT_EQUAL(list->head->next->next->element.integer, 5);
  CU_ASSERT_EQUAL(list->size, 5);

  ioopm_linked_list_insert(list, 0, int_elem(10));
  CU_ASSERT_EQUAL(list->head->element.integer, 10);
  
  ioopm_linked_list_insert(list, 0, int_elem(55));
  CU_ASSERT_EQUAL(list->head->element.integer, 55);
  CU_ASSERT_EQUAL(list->head->next->element.integer, 10);

  ioopm_linked_list_destroy(list);

}


void test_linked_list_remove() {
    ioopm_list_t *list = ioopm_linked_list_create(NULL);
    CU_ASSERT_PTR_NOT_NULL(list);

    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(20));
    ioopm_linked_list_append(list, int_elem(30));

    elem_t removed_value = ioopm_linked_list_remove(list, 1);
    CU_ASSERT_EQUAL(removed_value.integer, 20); // ta bort det andra elementet
    CU_ASSERT_EQUAL(list->size, 2);
    CU_ASSERT_EQUAL(list->head->next->element.integer, 30); // Jämför andra elementets värde med 30

    removed_value = ioopm_linked_list_remove(list, 0); // ta bort head
    CU_ASSERT_EQUAL(removed_value.integer, 10);
    CU_ASSERT_EQUAL(list->size, 1);
    CU_ASSERT_EQUAL(list->head->element.integer, 30); // nu borde 30 vara head och det enda elementet

    removed_value = ioopm_linked_list_remove(list, 0); // ta bort sista
    CU_ASSERT_EQUAL(removed_value.integer, 30);
    CU_ASSERT_EQUAL(list->size, 0);
    CU_ASSERT_PTR_NULL(list->head); // Nu borde listan vara null

    ioopm_linked_list_destroy(list);
}

void test_get_value() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_PTR_NOT_NULL(list);

    elem_t result = ioopm_linked_list_get(list, 0);
    CU_ASSERT_EQUAL(result.integer, -1); // Assuming -1 is the error case

    ioopm_linked_list_append(list, int_elem(100));
    ioopm_linked_list_append(list, int_elem(200));
    ioopm_linked_list_append(list, int_elem(300));
    ioopm_linked_list_append(list, int_elem(400));
    CU_ASSERT_EQUAL(list->size, 4);

    result = ioopm_linked_list_get(list, 0);
    CU_ASSERT_EQUAL(result.integer, 100);

    result = ioopm_linked_list_get(list, 50);
    CU_ASSERT_EQUAL(result.integer, -1); // Assuming -1 is the error case

    ioopm_linked_list_destroy(list);
}

void test_list_contains() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_PTR_NOT_NULL(list);

    bool result = ioopm_linked_list_contains(list, int_elem(-45));
    CU_ASSERT_FALSE(result);

    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));
    ioopm_linked_list_append(list, int_elem(4));
    CU_ASSERT_EQUAL(list->size, 4);

    result = ioopm_linked_list_contains(list, int_elem(10));
    CU_ASSERT_FALSE(result);
  
    result = ioopm_linked_list_contains(list, int_elem(1));
    CU_ASSERT_TRUE(result);

    ioopm_linked_list_destroy(list);
}


void test_is_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_PTR_NOT_NULL(list);

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));

    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));
    ioopm_linked_list_append(list, int_elem(4));
    CU_ASSERT_EQUAL(list->size, 4);

    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_is_clear() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));
    ioopm_linked_list_append(list, int_elem(4));
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));

    ioopm_linked_list_clear(list);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    CU_ASSERT_EQUAL(list->size, 0);
    CU_ASSERT_PTR_NULL(list->head);
    ioopm_linked_list_destroy(list);
}


void test_is_all_even() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(4));
    ioopm_linked_list_append(list, int_elem(6));

    CU_ASSERT_FALSE(ioopm_linked_list_all(list, ioopm_is_even, NULL));
    ioopm_linked_list_remove(list, 0);
    CU_ASSERT_TRUE(ioopm_linked_list_all(list, ioopm_is_even, NULL));

    ioopm_linked_list_destroy(list);
}

void test_is_any_even() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(3));

    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
    CU_ASSERT_FALSE(ioopm_linked_list_any(list, ioopm_is_even, NULL));

    ioopm_linked_list_append(list, int_elem(2));
    CU_ASSERT_TRUE(ioopm_linked_list_any(list, ioopm_is_even, NULL));

    ioopm_linked_list_destroy(list);
}

void test_function_apply() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));

    ioopm_linked_list_apply_to_all(list, ioopm_print_elem, NULL);
    // Output: 1, 2, 3

    ioopm_linked_list_destroy(list);  
}

void test_append_prepend_string(void) {
  ioopm_list_t *list = ioopm_linked_list_create(str_eq_func);
    elem_t elem1 = { .str = "hello" };
    elem_t elem2 = { .str = "world" };

    ioopm_linked_list_prepend(list, elem1);
    ioopm_linked_list_append(list, elem2);

    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 2);
    CU_ASSERT_STRING_EQUAL(ioopm_linked_list_get(list, 0).str, "hello");
    CU_ASSERT_STRING_EQUAL(ioopm_linked_list_get(list, 1).str, "world");
}

void test_contains(void) {
   ioopm_list_t *list = ioopm_linked_list_create(eq_func);
    elem_t elem1 = { .integer = 42 };
    elem_t elem2 = { .integer = 100 };

    ioopm_linked_list_append(list, elem1);
    
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, elem1));
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, elem2));
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
      (CU_add_test(my_test_suite, "test create and destroy list", test_create_destroy_LL) == NULL) ||
      (CU_add_test(my_test_suite, "test append list", test_append_list) == NULL) ||
      (CU_add_test(my_test_suite, "test prepend list", test_prepend_list) == NULL) ||
      (CU_add_test(my_test_suite, "test insert list", test_list_insert) == NULL) ||
      (CU_add_test(my_test_suite, "test remove list", test_linked_list_remove) == NULL) ||
      (CU_add_test(my_test_suite, "test get list", test_get_value) == NULL) ||
      (CU_add_test(my_test_suite, "test list contains", test_list_contains) == NULL) ||
      (CU_add_test(my_test_suite, "test list IS EMPTY", test_is_empty) == NULL) ||
      (CU_add_test(my_test_suite, "test list IS clear", test_is_clear) == NULL) ||
      (CU_add_test(my_test_suite, "test is all null", test_is_all_even) == NULL) ||
      (CU_add_test(my_test_suite, "test is any null", test_is_any_even) == NULL) ||
      (CU_add_test(my_test_suite, "test print", test_function_apply) == NULL) ||
      (CU_add_test(my_test_suite, "test prepend append string", test_append_prepend_string) == NULL) ||
      (CU_add_test(my_test_suite, "test contains", test_contains) == NULL) ||
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