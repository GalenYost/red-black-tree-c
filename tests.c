#include "tests.h"
#include "array.h"
#include "input.h"
#include "types.h"

#include <string.h>

DEFINE_ARRAY_TYPE(int, INT_ARRAY)

static void array_test() {
    INT_ARRAY arr;
    INT_ARRAY_init(&arr, 1);

    INT_ARRAY_push(&arr, 10);

    assert_inline(arr.size == 1, "Array length is not 1", "Array length is 1");
    assert_inline(INT_ARRAY_get(&arr, 0) != NULL, "First element is NULL",
                  "First element is 10");
    assert_inline(INT_ARRAY_get(&arr, 1) == NULL, "Second element is not NULL",
                  "Second element is NULL");

    INT_ARRAY_destroy(&arr);
}

static void inputbuf_test() {
    InputBufValueMap map;
    InputBufValueMap_init(&map, 1);

    InputBuffer *ib = InputBuffer_init("test", "test");
    if (!ib) {
        log_error("Test panic: input buffer failed to initialize");
        return;
    }

    assert_inline(strcmp(ib->inputPrompt, "test") == 0,
                  "Input buffer caught wrong input prompt",
                  "Input prompt is valid");
}

void run_tests() {
    array_test();
    inputbuf_test();
}
