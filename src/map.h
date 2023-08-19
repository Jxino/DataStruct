#ifndef __map_h__
#define __map_h__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "list.h"

typedef struct Bucket {
    List* keys;
    List* values;
} Bucket;

struct Map;
typedef struct Iterator {
    struct Map* map;
    size_t bucket_i;
    size_t bucket_i_j;
    void* p_key;
    void* p_value;
} Iterator;

typedef struct Map { // Generic HashMap
    Bucket** buckets; // array of buckets
    size_t count; // number of entries (key-value pairs) in this HashMap
    size_t n_buckets; // number of buckets in this hashMap
    size_t K_size; // sizeof Key type
    char* K_desc; // Key type description, "i", "c", "s"
    size_t V_size; // sizeof Value type
    char* V_desc; // Value type description, "i", "c", "s"
    Iterator* iter;
} Map;

Bucket* Bucket_create(size_t K_size, const char* K_desc, size_t V_size, const char* V_desc);
bool Bucket_insert(Bucket* This, void* p_key, void* p_value);
bool Bucket_delete(Bucket* This, void* p_key);
void Bucket_destroy(Bucket* This);

Map* Map_create(size_t K_size, const char* K_desc, size_t V_size, const char* V_desc);
void Map_destroy(Map* This);
bool Map_put(Map* This, void* p_key, void* p_value);
void* Map_get(Map* This, void* p_key);
bool Map_retrieve(Map* This, void* p_key, void* p_value);
bool Map_contains(Map* This, void* p_key);
Iterator* Iterator_next(Iterator* iter);
void Iterator_value(Iterator* iter, void* p_value);
void Iterator_key(Iterator* iter, void* p_key);
Iterator* Map_iterator(Map* This);
void Map_print(Map* This);
bool Map_delete(Map* This, void* p_key);
size_t Map_count(Map* This);
List* Map_keys(Map* This);
List* Map_values(Map* This);

// API for users
#define M_CREATE(K_size, K_desc, V_size, V_desc) Map_create((K_size), (K_desc), (V_size), (V_desc))
#define M_DESTROY(map) Map_destroy(map)
#define M_PUT(map, key, value) Map_put((map), (void*) &(key), (void*) &(value))
#define M_RETR(map, key, p_result) Map_retrieve((map), (void*) &(key), (void*) (p_result))
#define M_HAS(map, key) Map_contains((map), (void*) &(key))
#define M_ITER(map) Map_iterator(map)
#define I_NEXT(iter) Iterator_next(iter)
#define I_VALUE(iter, p_value) Iterator_value((iter), (void*) (p_value))
#define I_KEY(iter, p_key) Iterator_key((iter), (void*) (p_key))
#define M_DEL(map, key) Map_delete((map), (void*) &(key))
#define M_COUNT(map) Map_count(map)
#define M_KEYS(map) Map_keys(map)
#endif // __map_h__