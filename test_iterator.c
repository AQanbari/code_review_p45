#include <stdbool.h>
#include <stdio.h>
#include "iterator.h"
#include <stdlib.h>
#include <assert.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../linked_list/linked_list.h"
#include "../linked_list/linked_list.c"
#include "../common.h"

void test_create_destory()
{
    ioopm_list_t *list = ioopm_linked_list_create(NULL);
    iter_t *iter = ioopm_list_iterator(list);
    CU_ASSERT_PTR_NOT_NULL(iter);
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iterator_has_next()
{
    ioopm_list_t *list = ioopm_linked_list_create(NULL);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));
    iter_t *iter = ioopm_list_iterator(list);

    // vi börjar på huvudet
    CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
    ioopm_iterator_next(iter);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
    ioopm_iterator_next(iter);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iterator_current()
{
    ioopm_list_t *list = ioopm_linked_list_create(NULL);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    iter_t *iter = ioopm_list_iterator(list);

    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).integer, 1);
    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).integer, 2);

    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).integer, -1);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iterator_reset()
{
    ioopm_list_t *list = ioopm_linked_list_create(NULL);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));
    iter_t *iter = ioopm_list_iterator(list);

    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).integer, 2);

    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).integer, 3);

    ioopm_iterator_reset(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).integer, 1);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iterator_remove()
{
    ioopm_list_t *list = ioopm_linked_list_create(NULL);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    ioopm_linked_list_append(list, int_elem(3));
    iter_t *iter = ioopm_list_iterator(list);

    CU_ASSERT_EQUAL(ioopm_iterator_remove(iter).integer, 2);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter).integer, 3);
    CU_ASSERT_EQUAL(ioopm_iterator_remove(iter).integer, -1);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
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
        (CU_add_test(my_test_suite, "test create and destroy table", test_create_destory) == NULL) ||
        (CU_add_test(my_test_suite, "test has next", test_iterator_has_next) == NULL) ||
        (CU_add_test(my_test_suite, "test current", test_iterator_current) == NULL) ||
        (CU_add_test(my_test_suite, "test reset", test_iterator_reset) == NULL) ||
        (CU_add_test(my_test_suite, "test remove", test_iterator_remove) == NULL) ||
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
