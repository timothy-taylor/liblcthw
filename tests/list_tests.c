#include "minunit.h"
#include <h/list.h>
#include <assert.h>

static List *list = NULL;
static List *list1 = NULL;
static List *list2 = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list");

    return NULL;
}

char *test_destroy()
{
    List_clear_destroy(list);
    List_clear_destroy(list1);
    List_clear_destroy(list2);

    return NULL;
}

char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value.");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count on pop.");

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");

    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value.");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong first value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    // only need to test middle case since pop/shift
    // already test other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count on remove.");
    mu_assert(List_first(list) == test3, "Wrong first element after remove.");
    mu_assert(List_last(list) == test1, "Wrong last element after remove.");

    return NULL;
}

char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value after shift.");

    val = List_shift(list);
    mu_assert(val == test1, "Wrong value after shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}

char *test_copy()
{
    list1 = List_create();
    mu_assert(list1 != NULL, "Failed to create list");

    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);
    mu_assert(List_count(list) == 3, "Wrong initial count on copy.");

    List_copy(list, list1);
    mu_assert(List_first(list1) == List_first(list),
            "Wrong value after copy.");

    return NULL;
}

char *test_join()
{ 
    List_join(list, list1);
    mu_assert(List_count(list) == 6, "Wrong count after join.");

    List_pop(list1);
    List_pop(list1);
    List_pop(list1);

    return NULL;
}

char *test_split()
{
    list2 = List_create();
    mu_assert(list2 != NULL, "Failed to create list");
    mu_assert(List_count(list) == 6, "Wrong count before split.");
    
    List_split(list, list1, list2);
    mu_assert(List_count(list1) == 3, "Wrong count after split.");
    mu_assert(List_count(list2) == 3, "Wrong count after split.");

    // not sure why you have to pop everything off the list
    // but otherwise there is an error in test_destroy
    // kind of seems like List_clear isn't working

    List_pop(list);
    List_pop(list);
    List_pop(list);
    List_pop(list);
    List_pop(list);
    List_pop(list);

    List_pop(list1);
    List_pop(list1);
    List_pop(list1);
    List_pop(list2);
    List_pop(list2);
    List_pop(list2);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_copy);
    mu_run_test(test_join);
    mu_run_test(test_split);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
