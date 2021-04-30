#include "h/list.h"
#include "h/dbg.h"
#include <assert.h>

List *List_create()
{
    return calloc(1, sizeof(List));
}

void List_destroy(List * list)
{
    assert(list != NULL && "The list must not be NULL");

    LIST_FOREACH(list, first, next, cur) {
        if (cur->prev){
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List * list)
{
    assert(list != NULL && "The list must not be NULL");

    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }
}

void List_clear_destroy(List * list)
{
    assert(list != NULL && "The list must not be NULL");

    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
        if (cur->prev){
            free(cur->prev);
        }
    }
    free(list->last);
    free(list);

    //List_clear(list);
    //List_destroy(list);
}

void List_push(List * list, void *value)
{
    assert(list != NULL && "The list must not be NULL");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->last == NULL){
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

    assert(list->count > 0 && list->first != NULL && "Invalid list, list count is zero or the first node is NULL");
error:
    return;
}

void *List_pop(List * list)
{
    assert(list != NULL && "The list must not be NULL");

    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List * list, void *value)
{
    assert(list != NULL && "The list must not be NULL");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

    assert(list->count > 0 && list->first != NULL && "Invalid list, list count is zero or the first node is NULL");
error:
    return;
}

void *List_shift(List * list)
{
    assert(list != NULL && "The list must not be NULL");

    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List * list, ListNode * node)
{
    assert(list != NULL && "The list must not be NULL");

    void *result = NULL;

    check(list->first && list->last, "List is empty");
    check(node, "Node can't be NULL");

    if (node == list->first && node == list->last){
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first){
        list->first = node->next;
        check(list->first != NULL, "Invalid list, got a first that is NULL");
        list->first->prev = NULL;
    } else if (node == list->last){
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, got a last that is NULL");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}

void List_copy(List * src, List * dst)
{
    assert(src != NULL && dst != NULL && "Both lists must not be NULL");
    assert(dst->first == NULL && dst->last == NULL && 
            "The destination list must be empty");

    LIST_FOREACH(src, first, next, cur){
        List_push(dst, cur->value);
    }
}

void List_join(List * list_one, List * list_two)
{
    assert(list_one != NULL && list_two != NULL && "Both lists must not be NULL");
    assert(list_one->last != NULL && list_two->first != NULL && 
            "The nodes at beginning and end of the two lists must not be NULL");
   
    LIST_FOREACH(list_two, first, next, cur){
        List_push(list_one, cur->value);
    }
}

void List_split(List * orig, List * list_one, List * list_two)
{
    assert(orig != NULL && list_one != NULL && list_two != NULL && 
            "All lists must not exist");
    assert(orig->count > 0 && "The initial list must contain nodes");
    assert(list_one->count == 0 && list_two->count == 0 &&
            "The destination lists must be empty");

    int splitAt = orig->count / 2;
    int i = 0;

    LIST_FOREACH(orig, first, next, cur){
        List_push( ( (i++ >= splitAt) ? list_one : list_two ), cur->value);
    }
}
