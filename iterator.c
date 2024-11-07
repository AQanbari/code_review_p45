#include <stdbool.h>
#include <stdio.h>
#include "iterator.h"
#include <stdlib.h>
//#include "../linked_list/linked_list.h"
#include "../common.h"

iter_t *ioopm_list_iterator(ioopm_list_t *list)
{
    iter_t *result = calloc(1, sizeof(struct iter));

    result->current = list->head;
    result->list = list; // iterator remeber where it came from

    return result;
}

elem_t ioopm_iterator_peek(iter_t *iter)
{
    if (iter == NULL || iter->current == NULL || iter->current->next == NULL)
    {
        iter->current = NULL; // efter vi gått igenom sista elementet s'tter vi till NULL
        return int_elem(-1);
    }

    // printf("vidare till nästa");
    return iter->current->element;
}

bool ioopm_iterator_has_next(iter_t *iter)
{
    if (ioopm_iterator_peek(iter).integer == -1)
    {
        return false;
    }

    return iter->current->next != NULL;
}

bool ioopm_iterator_current_valid(iter_t *iter)
{
    return iter->current != NULL;
}

elem_t ioopm_iterator_next(iter_t *iter)
{
    if (iter == NULL || iter->current == NULL || iter->current->next == NULL)
    {
        iter->current = NULL; // efter vi gått igenom sista elementet s'tter vi till NULL
        return int_elem(-1);
    }

    iter->current = iter->current->next;
    // printf("vidare till nästa");
    return iter->current->element;
}


void ioopm_iterator_destroy(iter_t *iter)
{
    free(iter);
}

elem_t ioopm_iterator_current(iter_t *iter)
{
    if (iter->current == NULL)
    {
        return int_elem(-1);
    }

    return iter->current->element;
    ;
}

void ioopm_iterator_reset(iter_t *iter)
{
    iter->current = iter->list->head;
}

elem_t ioopm_iterator_remove(iter_t *iter)
{
    if (iter == NULL || iter->list == NULL || iter->current->next == NULL)
    {
        return int_elem(-1);
    }

    link_t *to_remove = iter->current->next;
    elem_t result = to_remove->element;
    iter->current->next = to_remove->next;

    free(to_remove);
    return result;
}
