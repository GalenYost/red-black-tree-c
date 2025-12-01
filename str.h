#ifndef STR_H
#define STR_H

#include "types.h"

const char *data_type_as_str(DataType type);
void data_value_as_str(Data *data, char *buf, size_t buf_len);

DataValue str_as_data_value(DataType type, const char *str);

#endif
