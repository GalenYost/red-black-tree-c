#include "types.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static char *DATA_TYPES[] = {"INT", "CHAR", "BOOL", "STR"};

void to_lower_str(char *s) {
    for (; *s; s++) { *s = tolower((unsigned char)*s); }
}

void to_upper_str(char *s) {
    for (; *s; s++) { *s = toupper((unsigned char)*s); }
}

int str_no_case_cmp(const char *a, const char *b) {
    for (; *a && *b; a++, b++) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

const char *data_type_as_str(DataType type) {
    if (type >= INT && type <= STR) return DATA_TYPES[type];
    return "NONE";
}

void data_value_as_str(Data *data, char *buf, size_t buf_len) {
    switch (data->type) {
    case INT:
        snprintf(buf, buf_len, "%d", data->value.i);
        break;
    case CHAR:
        snprintf(buf, buf_len, "%c", data->value.c);
        break;
    case BOOL:
        snprintf(buf, buf_len, "%d", data->value.b);
        break;
    case STR:
        snprintf(buf, buf_len, "%s", data->value.s);
        break;
    }
}

DataValue str_as_data_value(DataType type, const char *str) {
    DataValue val = {0};
    switch (type) {
    case INT:
        val.i = atoi(str);
        break;
    case BOOL:
        val.b = str_no_case_cmp(str, "true") == 0 ? true : false;
        break;
    case CHAR:
        val.c = str[0];
        break;
    case STR:
        val.s = strdup(str);
        break;
    }
    return val;
}
