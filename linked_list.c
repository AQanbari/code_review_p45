#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include "../common.h"
#include "../hash_table/hash_table.h"



bool compare_int_elements(elem_t a, elem_t b)
{
    return b.integer == a.integer;
}

link_t *link_create(elem_t element, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->element = element;
    link->next = next;
    return link;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    if (list)
    {
        list->head = list->tail = NULL;
        list->eq_fn = func;
    }
    return list;
}

#define Free(ptr)   \
    {               \
        free(ptr);  \
        ptr = NULL; \
    }

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t element)
{
    ioopm_linked_list_insert(list, list->size, element);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t element)
{
    ioopm_linked_list_insert(list, 0, element);
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t element)
{
    if (index < 0 || index > list->size)
    {
        return;
    }

    link_t *new_link = link_create(element, NULL);

    if (index == 0)
    {
        new_link->next = list->head;
        list->head = new_link;
        if (list->size == 0)
        {
            list->tail = new_link;
        }
    }
    else
    {
        link_t *current = list->head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }
        new_link->next = current->next;
        current->next = new_link;

        if (index == list->size)
        {
            list->tail = new_link;
        }
    }
    list->size++;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
    if (index < 0 || index >= list->size)
    {
        return int_elem(-1); // Returnera -1 (felmeddelande) om negativt index används eller index för högt 
    }

    link_t *current = list->head;
    elem_t removed_value;

    if (index == 0)
    {
        removed_value = current->element;
        list->head = current->next;
        if (list->size == 1)
        {
            list->tail = NULL;
        }
        Free(current);
    }
    else
    {
        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }

        link_t *to_remove = current->next;
        removed_value = to_remove->element;
        current->next = to_remove->next;

        if (index == list->size - 1)
        {
            list->tail = current;
        }

        Free(to_remove);
    }

    list->size--;
    return removed_value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
    if (index < 0 || index >= list->size)
    {
        return int_elem(-1);
    }
    link_t *current = list->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->element;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    if (list == NULL)
    {
        return false;
    }

    link_t *curr = list->head;
    while (curr != NULL)
    {
        if (list->eq_fn(curr->element, element))
        {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    link_t *curr = list->head;
    while (curr != NULL)
    {
        link_t *tmp = curr->next;
        Free(curr);
        curr = tmp;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

bool ioopm_is_even(elem_t element, void *extra)
{
    return element.integer % 2 == 0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate *prop, void *extra)
{
    if (list == NULL || prop == NULL)
    {
        return false;
    }

    link_t *curr = list->head; 

    while (curr != NULL)
    { 
        if (!prop(curr->element, extra))
        {
            return false;
        }
        curr = curr->next;
    }

    return true;
}

void ioopm_print_elem(elem_t element, void *extra)
{
    printf("%d,", element.integer);
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate *prop, void *extra)
{
    if (list == NULL || prop == NULL)
    {
        return false;
    }

    link_t *curr = list->head; 

    while (curr != NULL)
    { 
        if (prop(curr->element, extra))
        {
            return true;
        }
        curr = curr->next;
    }

    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function *fun, void *extra)
{
    if (list == NULL)
    {
        return;
    }

    link_t *curr = list->head;

    while (curr != NULL)
    {
        fun((curr->element), extra);
        curr = curr->next;
    }
}