#ifndef __list_h__
#define __list_h__

// Generic ArrayList in C
// Copyright@ jinhoesperas@gamil.com
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
#include <string.h>

typedef struct List { // Generic ArrayList
    char* array; // container of the elements
    size_t T_size; // size of element type T
    char* T_desc; // type description, "i", "c", "s", 
                  // TODO: needs to handle other types
                   // "I", "l", "L", "d","p"
                  // , "[i3]", "{icsp[i3]i{ic}}"
    size_t count; // number of elements in the array
    int capacity; // capacity of array
} List;

List* List_create(size_t T_size, const char* T_desc);
void* List_get(List* This, size_t index);
void List_retrieve(List* This, size_t index, void* p_elem_result);
void List_remove(List* This, size_t index, void* p_elem_removed);
void List_insert(List* This, size_t index, const void* p_elem);
void List_delete(List* This, size_t index);
void List_destroy(List* This);
int List_find(List* This, const void* p_elem);
void List_print(List* This, void (*print_elem_f)(const void* p_elem));
void List_sort(List* This, int (*compf)(const void* p_a, const void* p_b));
void List_append(List* This, const void* p_elem);
void List_shift(List* This, const void* p_elem);
void* List_dequeue(List* This);
void* List_pop(List* This);
bool List_contains(List* This, const void* p_elem);
size_t List_count(List* This);

int compare_int(const void* p_a, const void* p_b);
int compare_char(const void* p_a, const void* p_b);
int compare_str(const void* p_a, const void* p_b);

#define List_has(list, p_elem) List_contains((list), (p_elem))
#define List_push_back(list, p_elem) List_append((list), (p_elem))
#define List_add(list, p_elem) List_push((list), (p_elem))
#define List_push(list, p_elem) List_append((list), (p_elem))
#define List_enqueue(list, p_elem) List_append((list), (p_elem))
#define List_unshift(list) List_dequeue(list)

// Generic API for users
#define L_CREATE(size, desc) List_create((size), (desc))
#define L_DESTROY(list) List_destroy(list)
#define L_ADD(list, elem) List_append((list), (const void*) &(elem))
#define L_FIND(list, elem) List_find((list), (const void*) &(elem))
#define L_REMOVE(list, idx, p_elem_removed) List_remove((list), (idx), (void*) (p_elem_removed))
#define L_RETR(list, idx, p_elem_result) List_retrieve((list), (idx), (void*) ((p_elem_result)))
#define L_INSERT(list, idx, elem) List_insert((list), (idx), (const void*) &(elem))
#define L_HAS(list, elem) List_contains((list), (const void*) &(elem))
#define L_DELETE(list, idx) List_delete((list), (idx))
#define L_COUNT(list) List_count(list)
#define L_SORT(list, compare_f) List_sort((list), (int (*)(const void*, const void*))(compare_f)) 
#define L_PRINT(list, print_f) List_print((list), (void (*)(const void*))(print_f))
#define STREQ(str1, str2) (strcmp((str1), (str2)) == 0)

#endif // __list_h__