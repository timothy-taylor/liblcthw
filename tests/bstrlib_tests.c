#include <string.h>
#include "minunit.h"
#include <h/bstrlib.h>
#include <h/dbg.h>

static char *string = "Hello";
static char *string2 = "World";

char *test_bfromcstr()
{
    bstring b = bfromcstr(string);
    mu_assert(b, "!b, bfromcstr failed");
    mu_assert(strcmp((char*) b->data,string) == 0, 
            "b does not equal string, bfromcstr failed");

    bdestroy(b);
    return NULL;
}

char *test_blk2bstr()
{
    int size = sizeof(string);
    bstring b = blk2bstr(string, size);
    mu_assert(b, "!b, blk2bstr failed");
    mu_assert(strcmp((char*) b->data, string) == 0,
            "b does not equal string, blk2bstr failed");
    log_info("slen = %d, mlen = %d, size = %d", b->slen, b->mlen, size);    
    mu_assert(b->slen == size, 
            "mlen does not equal size, bkl2bstr failed");

    bdestroy(b);
    return NULL;
}

char *test_bstrcpy()
{
    bstring b = bfromcstr(string);
    mu_assert(b, "bfromcstr failed");
    bstring copy = bstrcpy(b);
    mu_assert(copy, "bstrcpy failed");
    mu_assert(strcmp((char*) b->data,(char*) copy->data) == 0,
            "b does not equal copy, bstrcpy failed");

    bdestroy(b);
    bdestroy(copy);
    return NULL;
}

char *test_bassign()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    bstring b2 = bfromcstr(string2);
    mu_assert(b, "bassign !b");
    mu_assert(b2, "bassign !b2");

    rc = bassign(b, b2);
    mu_assert(rc == 0, "bassign failed");
    mu_assert(strcmp((char*) b->data,(char*) b2->data) == 0, 
            "b does not equal b2, bassign failed");

    bdestroy(b);
    bdestroy(b2);
    return NULL;
}

char *test_bassigncstr()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    mu_assert(b, "bassigncstr !b");

    rc = bassigncstr(b, string2);
    mu_assert(rc == 0, "bassigncstr failed");
    mu_assert(strcmp((char*) b->data, string2) == 0,
            "b->data does not equal string2, bassigncstr failed");

    bdestroy(b);
    return NULL;
}

char *test_bassignblk()
{
    int rc = -1;
    int size = sizeof(string2);
    bstring b = bfromcstr(string);
    mu_assert(b, "bassignblk !b");

    rc = bassignblk(b, string2, size);
    mu_assert(rc == 0, "bassignblk failed");
    mu_assert(strcmp((char*) b->data, string2) == 0,
            "b->data does not equal string2, bassignblk failed");
    mu_assert(b->slen == size, "slen does not equal size, bassignblk failed");

    bdestroy(b);
    return NULL;
}

char *test_bdestroy()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    mu_assert(b, "bdestroy !b");

    rc = bdestroy(b);
    mu_assert(rc == 0, "bdestroy failed");

    return NULL;
}

char *test_bconcat()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    bstring b2 = bfromcstr(string2);
    mu_assert(b, "!b bconcat");
    mu_assert(b2, "!b2 bconcat");

    rc = bconcat(b, b2);
    mu_assert(rc == 0, "bconcat failed");
    mu_assert(strcmp((char*) b->data, "HelloWorld") == 0, 
            "Incorrect string, bconcat failed");

    bdestroy(b);
    bdestroy(b2);
    return NULL;
}

char *test_bstricmp()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    bstring b2 = bfromcstr(string);
    mu_assert(b, "!b bstricmp");
    mu_assert(b2, "!b2 bstricmp");
    
    rc = bstricmp(b, b2);
    mu_assert(rc == 0, "bstricmp failed");

    rc = bassigncstr(b2, string2);
    mu_assert(rc == 0, "reassigning b2 failed");

    rc = bstricmp(b, b2);
    mu_assert(rc < 0, "bstricmp failed");
    
    bdestroy(b);
    bdestroy(b2);
    return NULL;
}

char *test_biseq()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    bstring b2 = bfromcstr(string);
    mu_assert(b, "!b biseq");
    mu_assert(b2, "!b2 biseq");
    
    rc = biseq(b, b2);
    mu_assert(rc == 1, "bseq failed");

    rc = bassigncstr(b2, string2);
    mu_assert(rc == 0, "reassigning b2 failed");

    rc = biseq(b, b2);
    mu_assert(rc == 0, "bseq failed");

    bdestroy(b);
    bdestroy(b2);
    return NULL;
}

char *test_binstr()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    bstring b2 = bfromcstr("llo");
    mu_assert(b, "!b binstr");
    mu_assert(b2, "!b2 binstr");

    rc = binstr(b, 0, b2);
    mu_assert(rc == 2, "binstr failed");

    bdestroy(b);
    bdestroy(b2);
    return NULL;
}

char *test_bfindreplace()
{
    bstring b = bfromcstr(string);
    bstring find = bfromcstr("llo");
    bstring b2 = bfromcstr("moo");
    mu_assert(b && find && b2, "!b, !find, !b2");

    bfindreplace(b, find, b2, 0);
    mu_assert(strcmp((char*) b->data, "Hemoo") == 0, 
            "bfindreplace failed");

    bdestroy(b);
    bdestroy(find);
    bdestroy(b2);

    return NULL;
}

char *test_bsplit()
{
    int rc = -1;
    bstring b = bfromcstr(string2);
    mu_assert(b, "!b bsplit");
    unsigned char split = 'r';
    struct bstrList* list = bsplit(b, split);
    mu_assert(list, "list failed to create");

    bdestroy(b);
    rc = bstrListDestroy(list);
    mu_assert(rc == 0, "bstrListDestroy failed in bsplit");
    
    return NULL;
}

char *test_bformat()
{
    bstring b = bfromcstr(string2);
    bstring b2 = bformat("Hello %s", b->data);
    mu_assert(strcmp((char*) b2->data, "Hello World") == 0,
            "bformat failed");

    bdestroy(b);
    bdestroy(b2);

    return NULL;
}

char *test_blength()
{
    int rc = -1;
    bstring b = bfromcstr(string);
    rc = blength(b);
    mu_assert(rc == 5, "blength failed");

    bdestroy(b);
    return NULL;
}

char *test_bdata()
{
    bstring b = bfromcstr(string);
    char *data = bdata(b);
    mu_assert(strcmp(data, string) == 0,
            "bdata failed.");

    bdestroy(b);
    return NULL;
}

char *test_bchar()
{
    bstring b = bfromcstr(string);
    char c = bchar(b, 2);
    mu_assert(c == 'l',
            "bchar failed");

    bdestroy(b);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_bfromcstr);
    mu_run_test(test_blk2bstr);
    mu_run_test(test_bstrcpy);
    mu_run_test(test_bassign);
    mu_run_test(test_bassigncstr);
    mu_run_test(test_bassignblk);
    mu_run_test(test_bdestroy);
    mu_run_test(test_bconcat);
    mu_run_test(test_bstricmp);
    mu_run_test(test_binstr);
    mu_run_test(test_bfindreplace);
    mu_run_test(test_bsplit);
    mu_run_test(test_bformat);
    mu_run_test(test_blength);
    mu_run_test(test_bdata);
    mu_run_test(test_bchar);

    return NULL;
}

RUN_TESTS(all_tests);
