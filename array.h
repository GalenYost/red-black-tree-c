#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#define DECLARE_ARRAY_TYPE(T, Name)                                            \
    typedef struct {                                                           \
        T *data;                                                               \
        size_t size;                                                           \
        size_t cap;                                                            \
    } Name;                                                                    \
                                                                               \
    void Name##_init(Name *arr, size_t cap);                                   \
    void Name##_expand(Name *arr);                                             \
    void Name##_push(Name *arr, T value);                                      \
    void Name##_insert(Name *arr, T value, unsigned index);                    \
    void Name##_remove(Name *arr, unsigned index);                             \
    void Name##_remove_unorder(Name *arr, unsigned index);                     \
    void Name##_destroy(Name *arr);                                            \
    T Name##_pop(Name *arr);                                                   \
    T *Name##_get(Name *arr, unsigned index);

// Note: Pointers returned by get are invalidated by push.

#define DEFINE_ARRAY_TYPE(T, Name)                                             \
    void Name##_init(Name *arr, size_t cap) {                                  \
        if (cap == 0) cap = 1;                                                 \
        arr->data = malloc(sizeof(T) * cap);                                   \
        arr->size = 0;                                                         \
        arr->cap = cap;                                                        \
    }                                                                          \
                                                                               \
    void Name##_expand(Name *arr) {                                            \
        size_t new_cap = arr->cap * 2;                                         \
        T *new_data = realloc(arr->data, sizeof(T) * new_cap);                 \
        if (!new_data) {                                                       \
            log_error("Error reallocating array (expansion operation)");       \
            abort();                                                           \
        }                                                                      \
        arr->data = new_data;                                                  \
        arr->cap = new_cap;                                                    \
    }                                                                          \
                                                                               \
    T *Name##_get(Name *arr, unsigned index) {                                 \
        if (index >= arr->size) { return NULL; }                               \
        return &arr->data[index];                                              \
    }                                                                          \
                                                                               \
    void Name##_push(Name *arr, T value) {                                     \
        if (arr->size == arr->cap) Name##_expand(arr);                         \
        arr->data[arr->size++] = value;                                        \
    }                                                                          \
                                                                               \
    void Name##_insert(Name *arr, T value, unsigned index) {                   \
        if (index > arr->size) { return; }                                     \
        if (arr->size == arr->cap) Name##_expand(arr);                         \
                                                                               \
        if (index < arr->size) {                                               \
            T *src = &arr->data[index];                                        \
            T *dest = &arr->data[index + 1];                                   \
            size_t bytes = (arr->size - index) * sizeof(T);                    \
            memmove(dest, src, bytes);                                         \
        }                                                                      \
                                                                               \
        arr->data[index] = value;                                              \
        arr->size++;                                                           \
    }                                                                          \
                                                                               \
    T Name##_pop(Name *arr) {                                                  \
        if (arr->size == 0) {                                                  \
            log_error("array is empty, aborting function call");               \
            abort();                                                           \
        }                                                                      \
        arr->size--;                                                           \
        return arr->data[arr->size];                                           \
    }                                                                          \
                                                                               \
    void Name##_remove(Name *arr, unsigned index) {                            \
        if (index >= arr->size) return;                                        \
                                                                               \
        if (index < arr->size - 1) {                                           \
            T *dest = &arr->data[index];                                       \
            T *src = &arr->data[index + 1];                                    \
            size_t bytes = (arr->size - (index + 1)) * sizeof(T);              \
            memmove(dest, src, bytes);                                         \
        }                                                                      \
                                                                               \
        arr->size--;                                                           \
    }                                                                          \
                                                                               \
    void Name##_remove_unorder(Name *arr, unsigned index) {                    \
        if (index >= arr->size) return;                                        \
        arr->data[index] = arr->data[arr->size - 1];                           \
        arr->size--;                                                           \
    }                                                                          \
                                                                               \
    void Name##_destroy(Name *arr) { free(arr->data); }

#endif
