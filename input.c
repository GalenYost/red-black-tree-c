#include "input.h"
#include "log.h"
#include "str.h"
#include "types.h"

#include <stdio.h>
#include <string.h>

DEFINE_ARRAY_TYPE(InputBufValue, InputBufValueMap);

DataValue readInputAndCastValue(DataType type) {
    char *buf = NULL;
    size_t len = 100;

    ssize_t input = getline(&buf, &len, stdin);
    DataValue data = {0};

    if (input == -1) {
        log_error("getline failed or EOF");
        data.i = 0;
        free(buf);
        return data;
    }

    if (input > 0 && buf[input - 1] == '\n') {
        buf[input - 1] = '\0';
        input--;
    }

    data = str_as_data_value(type, buf);
    free(buf);

    return data;
}

InputBuffer *InputBuffer_init(const char *inputPrompt,
                              const char *optionsPrompt) {
    InputBuffer *ib = malloc(sizeof(InputBuffer));
    if (!ib) {
        log_error("Failed to initialize InputBuffer");
        return NULL;
    }

    ib->inputPrompt = inputPrompt;
    ib->optionsPrompt = optionsPrompt;
    InputBufValueMap_init(&ib->options, 1);

    return ib;
}

void InputBuffer_destroy(InputBuffer *ib) {
    InputBufValueMap_destroy(&ib->options);
    free(ib);
}

void InputBuffer_bind(InputBuffer *ib, InputBufValue value) {
    InputBufValueMap_push(&ib->options, value);
}

void InputBuffer_displayOptions(InputBuffer *ib) {
    printf("%s", ib->optionsPrompt);
    size_t size = ib->options.size;
    for (size_t i = 0; i < size; i++) {
        InputBufValue *cur = InputBufValueMap_get(&ib->options, i);
        if (!cur) {
            log_fatal("Value doesnt exist, index = %d", i);
            exit(1);
        }

        char value_buf[MAX_DATA_VALUE_BUF_SIZE];
        data_value_as_str(&cur->value.data, value_buf, sizeof(value_buf));

        printf("%s - %s\n", cur->value.key, value_buf);
    }
}

void InputBuffer_triggerInputEvent(InputBuffer *ib) {
    printf("%s", ib->inputPrompt);
    DataValue inp = readInputAndCastValue(STR);

    if (!inp.s) {
        log_error("error reading input");
        return;
    }

    size_t size = ib->options.size;
    for (size_t i = 0; i < size; i++) {
        InputBufValue *cur = InputBufValueMap_get(&ib->options, i);
        if (!cur) {
            log_fatal("Value doesnt exist, index = %d", i);
            exit(1);
        }

        const char *input_key = inp.s;
        const char *cur_key = cur->value.key;

        if (strcmp(input_key, cur_key) == 0) {
            cur->cb.func(cur->cb.env);
            return;
        }
    }

    log_warn("Value with key %s not found", inp.s);
}
