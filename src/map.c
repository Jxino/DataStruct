// Generic Map in C
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "list.h"
#include "map.h"

// #define debug printf("%s,%d: ", __FILE__, __LINE__); printf
#define debug if (false) printf

Bucket* Bucket_create(size_t K_size, const char* K_desc, size_t V_size, const char* V_desc) {
    Bucket* This = (Bucket*) malloc(sizeof(Bucket));
    This->keys = List_create(K_size, K_desc);
    This->values = List_create(V_size, V_desc);
    return This;
}

bool Bucket_insert(Bucket* This, void* p_key, void* p_value) {
    if (List_contains(This->keys, p_key)) {
        return false;
    }
    List_append(This->keys, p_key);
    List_append(This->values, p_value);
    return true;
}

bool Bucket_delete(Bucket* This, void* p_key) {
    int index = List_find(This->keys, p_key);
    if (index >= 0) {
        List_delete(This->keys, index);
        return true;
    }
    return false;
}

void Bucket_destroy(Bucket* This) {
    List_destroy(This->keys);
    List_destroy(This->values);
    free(This);
}

// Bucket* Buckets_get(List* map_buckets, int idx) {
//     Bucket** p_bucket = (Bucket**) List_get(map_buckets, idx);
//     return *p_bucket;
// }

// void Buckets_append(List* map_buckets, Bucket* bucket) {
//     List_append(This->buckets, &bucket);    
// }

Map* Map_create(size_t K_size, const char* K_desc, size_t V_size, const char* V_desc) {
    Map* This = (Map*) malloc(sizeof(Map));
    This->count = 0;
    This->n_buckets = 4;
    This->K_size = K_size;
    This->K_desc = (char*) malloc(strlen(K_desc) + 1);
    strcpy(This->K_desc, K_desc);
    This->V_size = V_size;
    This->V_desc = (char*) malloc(strlen(V_desc) + 1);
    strcpy(This->V_desc, V_desc);
    This->buckets = (Bucket**) malloc(sizeof(Bucket*) * This->n_buckets);
    for (size_t i = 0; i < This->n_buckets; i++) {
        Bucket* bucket = Bucket_create(K_size, K_desc, V_size, V_desc);
        This->buckets[i] = bucket;
    }
    This->iter = NULL;
    return This;
}

void Map_destroy(Map* This) {
    for (size_t i = 0; i < This->n_buckets; i++) {
        Bucket* bucket = This->buckets[i];
        Bucket_destroy(bucket);
    }
    free(This->buckets);
    free(This->K_desc);
    free(This->V_desc);
    free(This);
}

size_t hash(void* p_key, size_t K_size, const char* K_desc) {
    size_t hash = 0;
    for (size_t i = 0; i < K_size; i++) {
        if (STREQ(K_desc, "i")) {
            hash += *((size_t*) p_key) + 40;
        } else if (STREQ(K_desc, "c")) {
            hash += (size_t) (*((char*) p_key)) + 40;
        } else if (STREQ(K_desc, "s")) {
            char* str = *((char**) p_key);
            size_t len = strlen(str);
            for (size_t j = 0; j < len; j++) {
                hash += (size_t) str[j] + 40;
            }
        } else { // "p"
            hash += (size_t) (*((void**) p_key)) + 40;   
        }
    }
    debug("hash() returns %d\n", hash);
    return hash;
}

void Map_resize(Map* This) {
    if (This->count <= (int) (((float) This->n_buckets) * 0.75)) return;
    size_t new_n_buckets = This->n_buckets * 2;
    Bucket** new_buckets = (Bucket**) malloc(sizeof(Bucket*) * new_n_buckets);
    for (size_t i = 0; i < new_n_buckets; i++) {
        Bucket* bucket = Bucket_create(This->K_size, This->K_desc, This->V_size, This->V_desc);
        new_buckets[i] = bucket;
    }
    for (size_t i = 0; i < This->n_buckets; i++) {
        Bucket* bucket = This->buckets[i];
        for (size_t j = 0; j < List_count(bucket->keys); j++) {
            void* p_key = List_get(bucket->keys, j);
            void* p_value = List_get(bucket->values, j);
            size_t h = hash(p_key, This->K_size, This->K_desc);
            size_t bucket_idx = h % new_n_buckets;
            Bucket* new_bucket = new_buckets[bucket_idx];
            Bucket_insert(new_bucket, p_key, p_value);
        }
    }
    for (size_t i = 0; i < This->n_buckets; i++) {
        Bucket* bucket = This->buckets[i];
        Bucket_destroy(bucket);
    }
    free(This->buckets);
    This->buckets = new_buckets;
    This->n_buckets = new_n_buckets;
}

bool Map_put(Map* This, void* p_key, void* p_value) {
    debug("Map_put()\n");
    debug("p_key = %p, This = %p, This->K_size=%d, This->K_desc=%s\n", p_key, This, This->K_size, This->K_desc);
    size_t h = hash(p_key, This->K_size, This->K_desc);
    // debug("h=%d\n", h);
    size_t bucket_idx = h % This->n_buckets;
    Bucket* bucket = This->buckets[bucket_idx];
    bool inserted = Bucket_insert(bucket, p_key, p_value);
    if (inserted) {   
        This->count += 1;
        debug("Map_resize()\n");
        Map_resize(This);
        debug("Map_resize() done\n");
    }
    debug("Map_put()return inserted=%d\n", inserted);
    return inserted;
}

void* Map_get(Map* This, void* p_key) {
    size_t h = hash(p_key, This->K_size, This->K_desc);
    size_t bucket_idx = h % This->n_buckets;    
    Bucket* bucket = This->buckets[bucket_idx];
    int idx = List_find(bucket->keys, p_key);
    if (idx < 0) return NULL;
    void* p_value = List_get(bucket->values, idx);
    return p_value;
}

bool Map_retrieve(Map* This, void* p_key, void* p_value) {
    size_t h = hash(p_key, This->K_size, This->K_desc);
    size_t bucket_idx = h % This->n_buckets;    
    Bucket* bucket = This->buckets[bucket_idx];
    int idx = List_find(bucket->keys, p_key);
    if (idx < 0) return false;
    List_retrieve(bucket->values, idx, p_value);
    return true;
}

bool Map_contains(Map* This, void* p_key) {
    debug("Map_contains()\n");
    size_t h = hash(p_key, This->K_size, This->K_desc);
    size_t bucket_idx = h % This->n_buckets;    
    Bucket* bucket = This->buckets[bucket_idx];
    int idx = List_find(bucket->keys, p_key);
    if (idx >= 0) {
        debug("Map_contains() return true\n");
        return true;
    }
    debug("Map_contains() return false\n");
    return false;
}

Iterator* Iterator_next(Iterator* iter) {
    assert(iter != NULL);
    Map* map = iter->map;
    assert(map != NULL);
    while (iter->cur_bucket < map->n_buckets) {
        Bucket* bucket = map->buckets[iter->cur_bucket];
        assert(bucket != NULL);
        // debug("bucket=%d, count=%d\n", iter->cur_bucket, List_count(bucket->keys));
        if (iter->cur_key < List_count(bucket->keys)) {
            iter->p_key = List_get(bucket->keys, iter->cur_key);
            iter->p_value = List_get(bucket->values, iter->cur_key);
            iter->cur_key += 1;
            return iter;
        } else {
            iter->cur_key = 0;
        }
        iter->cur_bucket += 1;
    }
    return NULL;
}

Iterator* Map_iterator(Map* This) {
    Iterator* iter = (Iterator*) malloc(sizeof(Iterator));
    iter->map = This;
    iter->cur_bucket = 0;
    iter->cur_key = 0;
    This->iter = iter;
    return iter;
}

void Map_print(Map* This) {
    Iterator* iter = Map_iterator(This);
    printf("Map K_size: %d, K_desc: \"%s\"\n", This->K_size, This->K_desc);
    printf("Map V_size: %d, V_desc: \"%s\"\n", This->V_size, This->V_desc);
    printf("Map count: %d, n_buckets: %d\n", This->count, This->n_buckets);
    while (Iterator_next(iter) != NULL) {
        if (STREQ(This->K_desc, "i")) {
            printf("    key=%d,", *((int*) iter->p_key));
        } else if (STREQ(This->K_desc, "c")) {
            printf("    key=%c,", *((char*) iter->p_key));
        } else if ((STREQ(This->K_desc, "s"))) {
            printf("    key=%s,", *((char**) iter->p_key));
        } else {
            printf("    key=%p,", *((void**) iter->p_key));
        }
        
        if (STREQ(This->V_desc, "i")) {
            printf("value=%d\n", *((int*) iter->p_value));
        } else if (STREQ(This->V_desc, "c")) {
            printf("value=%c\n", *((char*) iter->p_value));
        } else if ((STREQ(This->V_desc, "s"))) {
            printf("value=%s\n", *((char**) iter->p_value));
        } else {
            printf("value=%p\n", *((void**) iter->p_value));
        }
    }
}

bool Map_delete(Map* This, void* p_key) {
    size_t h = hash(p_key, This->K_size, This->K_desc);
    size_t bucket_idx = h % This->n_buckets;    
    Bucket* bucket = This->buckets[bucket_idx];
    int idx = List_find(bucket->keys, p_key);
    if (idx < 0) return false;
    List_delete(bucket->keys, idx);
    List_delete(bucket->values, idx);
    return true;
}

size_t Map_count(Map* This) {
    return This->count;
}

List* Map_keys(Map* This) {
    List* keys = List_create(This->K_size, This->K_desc);
    Iterator* iter = Map_iterator(This);
    while (Iterator_next(iter) != NULL) {
        List_append(keys, iter->p_key);
    }
    return keys;
}

List* Map_values(Map* This) {
    List* values = List_create(This->V_size, This->V_desc);
    Iterator* iter = Map_iterator(This);
    while (Iterator_next(iter) != NULL) {
        List_append(values, iter->p_value);
    }
    return values;
}