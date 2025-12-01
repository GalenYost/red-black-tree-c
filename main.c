#include "input.h"
#include "tree.h"
#include "types.h"

#ifdef TEST_MODE
#include "tests.h"
#endif

void exit_fn(void *env) { exit(0); }
void insert_fn(void *env) {
    RedBlackTree *tree = (RedBlackTree *)env;

    printf("Value to insert: ");
    DataValue val = readInputAndCastValue(INT);
    if (!val.i) {
        log_error("Reading input failed");
        return;
    }

    RedBlackTree_insert(tree, val.i);
}
void remove_fn(void *env) {
    RedBlackTree *tree = (RedBlackTree *)env;

    printf("Value to search for: ");
    DataValue val = readInputAndCastValue(INT);
    if (!val.i) {
        log_error("Reading input failed");
        return;
    }

    RedBlackTree_remove(tree, val.i);
}
void display_fn(void *env) {
    RedBlackTree *tree = (RedBlackTree *)env;
    RedBlackTree_display(tree);
}

int main(void) {
#ifdef TEST_MODE
    run_tests();
#endif
#ifndef TEST_MODE
    RedBlackTree *tree = RedBlackTree_init();

    Callback exit_cb = {.func = exit_fn};
    Callback insert_cb = {.func = insert_fn, .env = tree};
    Callback remove_cb = {.func = remove_fn, .env = tree};
    Callback display_cb = {.func = display_fn, .env = tree};

    Pair exit_pair = Pair_init("e", STR, (DataValue){.s = "exit"});
    Pair insert_pair = Pair_init("i", STR, (DataValue){.s = "insert"});
    Pair remove_pair = Pair_init("r", STR, (DataValue){.s = "remove"});
    Pair display_pair = Pair_init("d", STR, (DataValue){.s = "display"});

    InputBufValue exit_val = {.value = exit_pair, .cb = exit_cb};
    InputBufValue insert_val = {.value = insert_pair, .cb = insert_cb};
    InputBufValue remove_val = {.value = remove_pair, .cb = remove_cb};
    InputBufValue display_val = {.value = display_pair, .cb = display_cb};

    InputBuffer *ib = InputBuffer_init("Choice: ", "Options:\n");
    InputBuffer_bind(ib, exit_val);
    InputBuffer_bind(ib, insert_val);
    InputBuffer_bind(ib, remove_val);
    InputBuffer_bind(ib, display_val);

    while (true) {
        InputBuffer_displayOptions(ib);
        InputBuffer_triggerInputEvent(ib);
    }
#endif
    return 0;
}
