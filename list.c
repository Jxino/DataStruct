#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include "list.h"

#define debug printf("%s,%d: ", __FILE__, __LINE__); printf

List* List_create(size_t T_size, const char* T_desc) {
    List* This = (List*) malloc(sizeof(List));
    This->T_size = T_size;
    This->T_desc = (char*) malloc(strlen(T_desc) + 1);
    strcpy(This->T_desc, T_desc);
    This->capacity = 4; // double capacity everytime it exceeds
    This->array = (char*) malloc(This->T_size * This->capacity);
    This->count = 0;
    return This;
}

void* List_get(List* This, size_t index) {
    return This->array + (This->T_size * index);
}

void List_retrieve(List* This, size_t index, void* p_elem_result) {
    memcpy(p_elem_result, This->array + (This->T_size * index), This->T_size);
}

void List_remove(List* This, size_t index, void* p_elem_removed) {
    memcpy(p_elem_removed, This->array + (This->T_size * index), This->T_size);
    List_delete(This, index);
}

void List_insert(List* This, size_t index, const void* p_elem) {
    assert(This != NULL);
    assert(index >= 0 && index <= This->count);
    assert(p_elem != NULL);
    if (This->count >= This->capacity) {
        This->capacity *= 2;
        This->array = (char*) realloc(This->array, This->T_size * This->capacity);
    }
    char* idx_ptr = (char*) List_get(This, index); // byte level pointer arithmetic
    size_t tailing_bytes = (This->count - index) * This->T_size;
    memmove(idx_ptr + This->T_size, idx_ptr, tailing_bytes);
    if (STREQ(This->T_desc, "s")) {
        char* new_str = (char*) malloc(strlen(*((char**)p_elem)) + 1);
        strcpy(new_str, *((char**)p_elem));
        memcpy(idx_ptr, &new_str, This->T_size);
    } else {
        memcpy(idx_ptr, p_elem, This->T_size);
    }
    This->count += 1;
}

void List_delete(List* This, size_t index) {
    assert(This != NULL);
    assert(index >= 0 && index < This->count);
    char* idx_ptr = (char*) List_get(This, index); // byte level pointer arithmetic
    size_t tailing_bytes = (This->count - index - 1) * This->T_size;
    if (STREQ(This->T_desc, "s")) {
        free(*((char**)idx_ptr));
    }
    memmove(idx_ptr, idx_ptr + This->T_size, tailing_bytes);
    This->count -= 1;
}

void List_destroy(List* This) {
    if (STREQ(This->T_desc, "s")) {
        for (size_t i = 0; i < This->count; i++) {
            char** idx_ptr = (char**) List_get(This, i);
            free(*idx_ptr);
        }
    }
    free(This->array);
    free(This->T_desc);
    free(This);
}

int List_find(List* This, const void* p_elem) {
    assert(This != NULL);
    assert(p_elem != NULL);
    for (size_t i = 0; i < This->count; i++) {
        char* idx_ptr = (char*) List_get(This, i);
        if (STREQ(This->T_desc, "s")) {
            if (STREQ(*((char**) idx_ptr), *((char**) p_elem))) {
                return i;
            }
        }
        if (memcmp(idx_ptr, p_elem, This->T_size) == 0)
            return i;
    }
    return -1;
}

void List_print(List* This, void (*print_elem)(const void* elem)) {
    printf("count=%d, capacity=%d, T_desc=\"%s\", T_size=%d: ", This->count, This->capacity, This->T_desc, This->T_size);
    for (size_t i = 0; i < This->count; i++) {
        char* idx_ptr = (char*) List_get(This, i);
        if (print_elem == NULL) {
            if (STREQ(This->T_desc, "i")) {
                printf("%d ", *((size_t*) idx_ptr));
            } else if (STREQ(This->T_desc, "c")) {
                printf("%c ", *((char*) idx_ptr));
            } else if (STREQ(This->T_desc, "s")) {
                printf("%s ", *((char**) idx_ptr));
            }
            continue;
        }
        print_elem(idx_ptr);
    }
    printf("\n");
}

int compare_int(const void* p_a, const void* p_b) {
    return *((int*)p_a) - *((int*)p_b);
}

int compare_char(const void* p_a, const void* p_b) {
    return *((char*)p_a) - *((char*)p_b);
}

int compare_str(const void* p_a, const void* p_b) {
    return strcmp(*((char**) p_a), *((char**) p_b));
}

void List_sort(List* This, int (*compf)(const void* p_a, const void* p_b)) {
    if (compf == NULL) {
        if (STREQ(This->T_desc, "i")) {
            compf = compare_int;
        } else if (STREQ(This->T_desc, "c")) {
            compf = compare_char;
        } else if (STREQ(This->T_desc, "s")) {
            compf = compare_str;
        } else {
            assert(false);
        }
    }
    qsort(This->array, This->count, This->T_size, compf);
}

void List_append(List* This, const void* p_elem) {
    List_insert(This, This->count, p_elem);
}

void List_shift(List* This, const void* p_elem) {
    List_insert(This, 0, p_elem);
}

void* List_dequeue(List* This) {
    if (This->count == 0) return NULL;
    void* p_elem = (void*) malloc(This->T_size);
    memcpy(p_elem, This->array, This->T_size);
    // if (STREQ(This->T_desc, "s")) {
    //     *((char**) p_elem) = (char*) malloc(strlen((char*) This->array) + 1);
    //     strcpy(*((char**) p_elem), (char*) This->array);
    // }
    List_delete(This, 0);
    return p_elem;
}

void* List_pop(List* This) {
    if (This->count == 0) return NULL;
    void* p_elem = (void*) malloc(This->T_size);
    memcpy(p_elem, List_get(This, This->count - 1), This->T_size);
    List_delete(This, This->count - 1);
    return p_elem;
}

bool List_contains(List* This, const void* p_elem) {
    int idx = List_find(This, p_elem);
    if (idx < 0) return false;
    return true;
}

size_t List_count(List* This) {
    return This->count;
}