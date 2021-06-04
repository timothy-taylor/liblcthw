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
    log_info("merging...");
    List *result = List_create();
    void *val = NULL;
    int compared = 0;

    while (List_count(left) > 0 || List_count(right) > 0){
        if (List_count(left) > 0 && List_count(right) > 0 ){
            assert(left->first->value != NULL && right->first->value != NULL
                    && "Values must not be null"); 
            compared = cmp(List_first(left), List_first(right));
            log_info("compare: %d", compared);
            if (compared <= 0){
                val = List_shift(left);
            } else {
                log_info("here it is");
                val = List_shift(right);
            }
           
            log_info("val = %s", val);
            List_push(result, val);
        } else if (List_count(left) > 0){
            val = List_shift(left);
            log_info("val = %s", val);
            List_push(result, val);
        } else if (List_count(right) > 0){
            val = List_shift(right);
            log_info("val = %s", val);
            List_push(result, val);
        }
    }
    
    log_info("returning results...");
    return result;
}

List *List_merge_sort(List * list, List_compare cmp)
{
    if (List_count(list) <= 1){
        return list;
    }

    List *result = NULL;
    List *left = List_create();
    List *right = List_create();
    int mid = List_count(list) / 2;
    int i = 0;
    
    LIST_FOREACH(list, first, next, cur){
        if (i < mid){
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }
        i++;
    }

    log_info("left: %d: %s, right %d: %s", left->count, left->first->value,
            right->count, right->first->value);
    List *sort_left = List_merge_sort(left, cmp);
    List *sort_right = List_merge_sort(right, cmp);

    if (sort_left != left)
        List_destroy(left);
    if (sort_right != right)
        List_destroy(right);

    log_info("sort_left: %d: %s, sort_right %d: %s", sort_left->count, sort_left->first->value,
            sort_right->count, sort_right->first->value);
    result = List_merge(sort_left, sort_right, cmp);

    List_destroy(sort_left);
    List_destroy(sort_right);

    return result;
}
