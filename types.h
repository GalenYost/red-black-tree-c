#ifndef TYPES_H
#define TYPES_H

#include "array.h"

#include <stdbool.h>

#define MAX_DATA_TYPE_BUF_SIZE 5
#define MAX_DATA_VALUE_BUF_SIZE 100

typedef enum {
    INT,
    BOOL,
    CHAR,
    STR,
} DataType;

typedef union {
    int i;
    bool b;
    char c;
    char *s;
} DataValue;

typedef struct {
    DataType type;
    DataValue value;
} Data;

typedef void (*fn)(void *env);

typedef struct {
    void *env;
    fn func;
} Callback;

typedef struct {
    const char *key;
    Data data;
} Pair;

Pair Pair_init(const char *key, DataType type, DataValue value);

typedef struct InputBufValue InputBufValue;

DECLARE_ARRAY_TYPE(int, INT_ARRAY)
DECLARE_ARRAY_TYPE(InputBufValue, InputBufValueMap);

#endif
