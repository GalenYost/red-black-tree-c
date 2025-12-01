#ifndef INPUT_H
#define INPUT_H

#include "array.h"
#include "types.h"

struct InputBufValue {
    Pair value;
    Callback cb;
};

typedef struct {
    const char *inputPrompt;
    const char *optionsPrompt;
    InputBufValueMap options;
} InputBuffer;

DataValue readInputAndCastValue(DataType type);

InputBuffer *InputBuffer_init(const char *inputPrompt,
                              const char *optionsPrompt);
void InputBuffer_destroy(InputBuffer *ib);
void InputBuffer_bind(InputBuffer *ib, InputBufValue value);
void InputBuffer_displayOptions(InputBuffer *ib);
void InputBuffer_triggerInputEvent(InputBuffer *ib);

#endif
