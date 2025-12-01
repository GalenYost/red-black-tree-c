#include "types.h"

Pair Pair_init(const char *key, DataType type, DataValue value) {
    Pair pair = {0};
    pair.key = key;
    pair.data.type = type;
    pair.data.value = value;
    return pair;
}
