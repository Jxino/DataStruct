#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// #define debug printf("(%s,%d)", __FILE__, __LINE__); printf

typedef int Key;
typedef int Value;
typedef int Element;

#define MAX_SLOT 5
#define NOT_FOUND -9999
typedef struct Bucket {
    Key keys[MAX_SLOT];
    Value values[MAX_SLOT];
    int count;
} Bucket;

void Bucket_init(Bucket* bucket) {
    bucket->count = 0;
}

bool Bucket_put(Bucket* bucket, Key key, Value value) {
    assert(bucket != NULL);
    if (bucket->count == MAX_SLOT) {
        return false;
    }
    for (int i = 0; i < bucket->count; i++) {
        if (bucket->keys[i] == key) {
            bucket->values[i] = value;
            return false;
        }
    }
    bucket->keys[bucket->count] = key;
    bucket->values[bucket->count] = value;
    bucket->count += 1;
    return true;
}

Value Bucket_get(Bucket* bucket, Key key) {
    assert(bucket != NULL);
    for (int i = 0; i < bucket->count; i++) {
        if (bucket->keys[i] == key) {
            return bucket->values[i];
        }
    }
    return NOT_FOUND;
}

bool Bucket_del(Bucket* bucket, Key key) {
    assert(bucket != NULL);
    for (int i = 0; i < bucket->count; i++) {
        if (bucket->keys[i] == key) {
            for (int j = i; j < bucket->count - 1; j++) {
                bucket->keys[j] = bucket->keys[j + 1];
                bucket->values[j] = bucket->values[j + 1];
            }
            bucket->count -= 1;
            return true;
        }
    }
    return false;
}

typedef struct HashMap {
    Bucket** buckets;
    int count; // HashMap이 담고 있는 (key, value)의 개수
    int num_buckets; // bucket 개수 8, 16, ......
    struct Iterator* iter;
} HashMap;

HashMap* HashMap_new() {
    HashMap* This = (HashMap*) malloc(sizeof(HashMap));
    This->count = 0;
    This->num_buckets = 8;
    This->buckets = (Bucket**) malloc(sizeof(Bucket*) * This->num_buckets);
    for (int i = 0; i < This->num_buckets; i++) {
        This->buckets[i] = (Bucket*) malloc(sizeof(Bucket));
        Bucket_init(This->buckets[i]);
    }
    This->iter = NULL;
    return This;
}

int hash(Key key) { // Simplest hash function
    // int h = 0;
    // int size = sizeof(Key);
    // char* p = (char*) &key;
    // char prime[8] = {2, 3, 7, 11, 13, 17, 19, 23};
    // for (int i = 0; i < size; i++) {
    //     h += p[i] ^ prime[i]; // bitwise XOR
    //     h <<= 8;
    // }
    // return h;
    return key + 42;
}

typedef struct Iterator {
    HashMap* table;
    int cur_bucket;
    int cur_key;
    Key key;
    Value value;
} Iterator;

Iterator* Iterator_next(Iterator* iter) {
    assert(iter != NULL);
    HashMap* table = iter->table;
    assert(table != NULL);
    while (iter->cur_bucket < table->num_buckets) {
        Bucket* bucket = table->buckets[iter->cur_bucket];
        assert(bucket != NULL);
        if (iter->cur_key < bucket->count) {
            iter->key = bucket->keys[iter->cur_key];
            iter->value = bucket->values[iter->cur_key];
            iter->cur_key += 1;
            return iter;
        } else {
            iter->cur_key = 0;
        }
        iter->cur_bucket += 1;
    }
    return NULL;
}

Iterator* HashMap_iterator(HashMap* This) {
    Iterator* iter = (Iterator*) malloc(sizeof(Iterator));
    iter->table = This;
    iter->cur_bucket = 0;
    iter->cur_key = 0;
    This->iter = iter;
    return iter;
}

void HashMap_resize(HashMap* This) {
    // debug("-\n");
    if (This->count <= (int) ((float) This->num_buckets * 0.75)) {
        // debug("count = %d, num_buckets = %d\n", This->count, This->num_buckets);
        return;
    }
    // debug("-\n");
    // 새로운 buckets을 할당한다
    int new_count = 0;
    int new_num_buckets = This->num_buckets * 2;
    Bucket** new_buckets = (Bucket**) malloc(sizeof(Bucket*) * new_num_buckets);
    for (int i = 0; i < new_num_buckets; i++) {
        new_buckets[i] = (Bucket*) malloc(sizeof(Bucket));
        Bucket_init(new_buckets[i]);
    }
    // debug("-\n");
    // 새로운 buckets에 기존 값들을 모두 옮겨 담는다
    Iterator* iter = HashMap_iterator(This);
    while (true) {
        iter = Iterator_next(iter);
        if (iter == NULL) {
            // debug("-\n");
            break;
        }
        // debug("moving... key = %d, value = %d\n", iter->key, iter->value);
        int h = hash(iter->key);
        int which_bucket = h % new_num_buckets;
        // debug("which_bucket = %d, new_num_buckets = %d\n", which_bucket, new_num_buckets);
        bool inserted = Bucket_put(new_buckets[which_bucket], iter->key, iter->value);
        if (inserted == true) {
            // debug("-\n");
            new_count += 1;
        }
    }
    // debug("-\n");
    // 기존의 This에 들어 있는 buckets를 모두 free한다
    for (int i = 0; i < This->num_buckets; i++) {
        free(This->buckets[i]);
    }
    free(This->buckets);
    // debug("-\n");
    // 새로운 buckets를 This에 할당
    This->count = new_count;
    This->num_buckets = new_num_buckets;
    This->buckets = new_buckets;
}

void HashMap_delete(HashMap* This) {
    // 기존의 This에 들어 있는 buckets를 모두 free한다
    for (int i = 0; i < This->num_buckets; i++) {
        free(This->buckets[i]);
    }
    free(This->buckets);
}

bool HashMap_put(HashMap* This, Key key, Value value) {
    int h = hash(key);
    int which_bucket = h % This->num_buckets;
    // debug("hash = %d, key = %d, which_bucket = %d, num_buckets = %d\n", h, key, which_bucket, This->num_buckets);
    bool inserted = Bucket_put(This->buckets[which_bucket], key, value); 
    if (inserted == true) {
        This->count += 1;
        HashMap_resize(This);
        // debug("-\n");
        return true;
    }
    // debug("-\n");
    return false;
}

Value HashMap_get(HashMap* This, Key key) {
    int h = hash(key);
    int which_bucket = h % This->num_buckets;
    Bucket* bucket = This->buckets[which_bucket];
    return Bucket_get(bucket, key);    
}

void HashMap_del(HashMap* This, Key key) {
    int h = hash(key);
    int which_bucket = h % This->num_buckets;
    Bucket* bucket = This->buckets[which_bucket];
    // if (HashMap_contains(This, key)) This->count -= 1;
    if (Bucket_del(bucket, key)) This->count -= 1;
}

void HashMap_print(HashMap* This) {
    Iterator* iter = HashMap_iterator(This);
    while (Iterator_next(iter) != NULL) {
        // iter = Iterator_next(iter);
        // if (iter == NULL) break;
        printf("(%d, %d)\n", iter->key, iter->value);
    }
}

Key* HashMap_keys(HashMap* This) {
    Key* key_s = (Key*) malloc(sizeof(Key) * This->count);
    Iterator* iter = HashMap_iterator(This);
    // for (int i = 0; Iterator_next(iter) != NULL; i++) {
    //     key_s[i] = iter->key;
    // }
    int i = 0;
    while (Iterator_next(iter) != NULL) {
       key_s[i] = iter->key;
       i += 1;
    }
    return key_s;
}

Value* HashMap_values(HashMap* This) {
    Value* value_s = (Value*) malloc(sizeof(Value) * This->count);
    Iterator* iter = HashMap_iterator(This);
    // for (int i = 0; Iterator_next(iter) != NULL; i++) {
    //     value_s[i] = iter->value;
    // }
    int i = 0;
    while (Iterator_next(iter) != NULL) {
       value_s[i] = iter->value;
       i += 1;
    }
    return value_s;
}

int HashMap_get_count(HashMap* This) {
    return This->count;
}

bool HashMap_contains(HashMap* This, Key key) {
    return (HashMap_get(This, key) != NOT_FOUND);
}

int compare(int a, int b) {
    return b - a;
}

void qsort(int array[], int left, int right, int (*compare)(int a, int b)) {
    if (right <= left) return;

    int pivot = array[right];
    int pivotIdx = left;
    for (int i = left; i < right; i++) {
        if (compare(array[i], pivot) > 0) {
            int temp = array[i];
            array[i] = array[pivotIdx];
            array[pivotIdx] = temp;
            pivotIdx += 1;
        }
    }
    int temp = array[right];
    array[right] = array[pivotIdx];
    array[pivotIdx] = temp;

    qsort(array, left, pivotIdx - 1, compare);
    qsort(array, pivotIdx + 1, right, compare);
}

typedef struct Set {
    HashMap* map;
} Set;

Set* Set_create() {
    Set* This = (Set*) malloc(sizeof(Set));
    This->map = HashMap_new();
    return This;
}

int Set_count(Set* This) {
    return HashMap_get_count(This->map);
}

bool Set_has(Set* This, Element element) {
    return HashMap_contains(This->map, element);
}

void Set_add(Set* This, Element element) {
    HashMap_put(This->map, element, 1);
}

void Set_remove(Set* This, Element element) {
    HashMap_del(This->map, element);
}

Set* Set_intersection(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = HashMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        if (Set_has(set2, iter->key)) {
            Set_add(set3, iter->key);
        }
    }
    return set3;
}

Set* Set_union(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = HashMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        Set_add(set3, iter->key);
    }
    iter = HashMap_iterator(set2->map);
    while (Iterator_next(iter) != NULL) {
        Set_add(set3, iter->key);
    }
    return set3;
}

Set* Set_difference(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = HashMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        if (! Set_has(set2, iter->key)) {
            Set_add(set3, iter->key);
        }   
    }
    return set3;
}

Set* Set_symmetric_difference(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = HashMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        if (! Set_has(set2, iter->key)) {
            Set_add(set3, iter->key);
        }   
    }
    iter = HashMap_iterator(set2->map);
    while (Iterator_next(iter) != NULL) {
        if (! Set_has(This, iter->key)) {
            Set_add(set3, iter->key);
        }   
    }
    return set3;
}

void Set_print(Set* This) {
    printf("count: %d {", Set_count(This));
    Iterator* iter = HashMap_iterator(This->map);
    for (int i = 0; Iterator_next(iter) != NULL; i++) {
        if (i == Set_count(This) - 1) {
            printf("%d", iter->key);
        } else {
            printf("%d, ", iter->key);
        }
    }
    printf("}\n");
}

void Set_delete(Set* This) {
    HashMap_delete(This->map);
    free(This);
}

int main() {
    HashMap* table = HashMap_new();
    HashMap_put(table, 5, 500);
    HashMap_put(table, 7, 700);
    HashMap_put(table, 1, 100);
    HashMap_put(table, 3, 300);
    HashMap_put(table, 2, 200);
    HashMap_put(table, 15, 1500);
    HashMap_put(table, 17, 1700);
    HashMap_put(table, 11, 1100);
    HashMap_put(table, 13, 1300);
    HashMap_put(table, 12, 1200);
    HashMap_print(table);
    Value val = HashMap_get(table, 3);
    printf("val : %d\n", val);
    Value val2 = HashMap_get(table, 9);
    printf("val : %d\n", val2);
    HashMap_del(table, 17);
    HashMap_print(table);

    printf("----------\n");
    int* keys = HashMap_keys(table);
    qsort(keys, 0, HashMap_get_count(table) - 1, compare);
    for (int i = 0; i < HashMap_get_count(table); i++) {
        printf("key: %d, value: %d\n", keys[i], HashMap_get(table, keys[i]));
    }
    free(keys);
    printf("----------\n");
    HashMap_delete(table);

    Set* setx = Set_create();
    Set_add(setx, 23);
    Set_add(setx, 37);
    Set_add(setx, 41);
    Set_add(setx, 23);
    Set_add(setx, 59);
    Set_add(setx, 73);
    Set_add(setx, 89);
    Set_add(setx, 97);

    Set_print(setx);
    Set* sety = Set_create();
    Set_add(sety, 37);
    Set_add(sety, 67);
    Set_add(sety, 83);
    Set_add(sety, 41);
    Set_print(sety);
    Set* setz = Set_intersection(setx, sety);
    Set_print(setz);
    Set* setw = Set_union(setx, sety);
    Set_print(setw);
    Set* setu = Set_difference(setx, sety);
    Set_print(setu);
    Set* setk = Set_difference(sety, setx);
    Set_print(setk);
    Set* setl = Set_symmetric_difference(setx, sety);
    Set_print(setl);
    Set_delete(setx);
    Set_delete(sety);
    Set_delete(setz);
    Set_delete(setw);
    Set_delete(setu);
    Set_delete(setk);
    Set_delete(setl);

    return 0;    
}