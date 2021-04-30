#include <h/list_algos.h>
#include <h/dbg.h>
#include <assert.h>

static inline void ListNode_swap(ListNode * a, ListNode * b)
{
    assert(a != NULL && b != NULL && "values cannot be null");
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List * list, List_compare cmp)
{
    assert(list != NULL && "The list cannot be null.");
        
    if (List_count(list) <= 1){
        return 0;
    }

    int i = 0;
    int n = List_count(list) - 1;

    for (i = 0; i < n; i++){
        LIST_FOREACH(list, first, next, cur){
            if (cur->next){
                if (cur->next->value && cmp(cur->value, cur->next->value) > 0){
                    check(cur->value != NULL && cur->next->value != NULL,
                            "Neither the current value or next value can be NULL.");
                    ListNode_swap(cur, cur->next);
                }
                
            }
        }
    }

    return 0;
error:
    return -1;
}

static inline List *List_merge(List * left, List * right, List_compare cmp)
{
    assert(left != NULL && right != NULL && "Lists cannot be null");
    List *result = List_create();
    void *val = NULL;

    log_info("Left count: %d, right count: %d", List_count(left), List_count(right));
    while (List_count(left) > 0 || List_count(right) > 0){
        if (List_count(left) > 0 && List_count(right) > 0 ){
            log_info("merge compare running");
            log_info("left: %s, right: %s", List_first(left), List_first(right));
            if (cmp(List_first(left), List_first(right)) <= 0){
                val = List_shift(left);
            } else {
                val = List_shift(right);
            }
            
            log_info("pushing");
            List_push(result, val);
        } else if (List_count(left) > 0){
            log_info("right is 0");
            val = List_shift(left);
            List_push(result, val);
        } else if (List_count(right) > 0){
            log_info("left is 0");
            val = List_shift(right);
            List_push(result, val);
        }
    }

    return result;
}

List *List_merge_sort(List * list, List_compare cmp)
{
    List *result = NULL;

    if (List_count(list) <= 1){
        return list;
    }

    List *left = List_create();
    List *right = List_create();
    int middle = List_count(list) / 2;

    LIST_FOREACH(list, first, next, cur){
        if (middle > 0){
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }

        middle--;
    }

    List *sort_left = List_merge_sort(left, cmp);
    List *sort_right = List_merge_sort(right, cmp);

    if (sort_left != left)
        List_destroy(left);
    if (sort_right != right)
        List_destroy(right);

    result = List_merge(sort_left, sort_right, cmp);

    List_destroy(sort_left);
    List_destroy(sort_right);

    return result;
}
