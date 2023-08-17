#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <vector>

// #define debug printf("(%s,%d)", __FILE__, __LINE__); printf
#define debug if (false) printf

typedef int Key;
typedef int Value;
typedef int Element;

#define NOT_FOUND -9999
class Bucket {
  public:  
    std::vector<Key> keys;
    std::vector<Value> values;

  public:  
    void init() {
        
    }

    bool put(Key key, Value value) {
        for (int i = 0; i < this->keys.size(); i++) {
            if (this->keys[i] == key) {
                this->values[i] = value;
                return false;
            }
        }
        this->keys.push_back(key);
        this->values.push_back(value);
        return true;
    }

    Value get(Key key) {
        for (int i = 0; i < this->keys.size(); i++) {
            if (this->keys[i] == key) {
                return this->values[i];
            }
        }
        return NOT_FOUND;
    }

    bool del(Key key) {
        for (int i = 0; i < this->keys.size(); i++) {
            if (this->keys[i] == key) {
                for (int j = i; j < this->keys.size() - 1; j++) {
                    this->keys[j] = this->keys[j + 1];
                    this->values[j] = this->values[j + 1];
                }
                return true;
            }
        }
        return false;
    }    
};

class HashMap {
  public:  
    class Iterator {
      public:  
        HashMap* table;
        int cur_bucket;
        int cur_key;
        Key key;
        Value value;

      public:  
        Iterator* next() {
            HashMap* table = this->table;
            assert(table != NULL);
            while (this->cur_bucket < table->num_buckets) {
                Bucket* bucket = table->buckets[this->cur_bucket];
                assert(bucket != NULL);
                if (this->cur_key < bucket->keys.size()) {
                    this->key = bucket->keys[this->cur_key];
                    this->value = bucket->values[this->cur_key];
                    this->cur_key += 1;
                    return this;
                } else {
                    this->cur_key = 0;
                }
                this->cur_bucket += 1;
            }
            return NULL;
        }    
    };
  
    Iterator* iterator() {
        Iterator* iter = new Iterator();
        iter->table = this;
        iter->cur_bucket = 0;
        iter->cur_key = 0;
        this->iter = iter;
        return iter;
    }      
  
  public:  
    std::vector<Bucket*> buckets;
    int count; // HashMap이 담고 있는 (key, value)의 개수
    int num_buckets; // bucket 개수 8, 16, ......
    Iterator* iter;

  public:  
    HashMap() {
        this->count = 0;
        this->num_buckets = 8;
        for (int i = 0; i < this->num_buckets; i++) {
            this->buckets.push_back(new Bucket());
            this->buckets[i]->init();
        }
        this->iter = NULL;
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

    void resize() {
        debug("-\n");
        if (this->count <= (int) ((float) this->num_buckets * 0.75)) {
            debug("count = %d, num_buckets = %d\n", this->count, this->num_buckets);
            return;
        }
        debug("-\n");
        // 새로운 buckets을 할당한다
        int new_count = 0;
        int new_num_buckets = this->num_buckets * 2;
        std::vector<Bucket*> new_buckets;
        for (int i = 0; i < new_num_buckets; i++) {
            new_buckets.push_back(new Bucket());
            new_buckets[i]->init();
        }
        debug("-\n");
        // 새로운 buckets에 기존 값들을 모두 옮겨 담는다
        Iterator* iter = this->iterator();
        while (true) {
            iter = iter->next();
            if (iter == NULL) {
                debug("-\n");
                break;
            }
            debug("moving... key = %d, value = %d\n", iter->key, iter->value);
            int h = hash(iter->key);
            int which_bucket = h % new_num_buckets;
            debug("which_bucket = %d, new_num_buckets = %d\n", which_bucket, new_num_buckets);
            bool inserted = new_buckets[which_bucket]->put(iter->key, iter->value);
            if (inserted == true) {
                debug("-\n");
                new_count += 1;
            }
        }
        debug("-\n");
        // 기존의 This에 들어 있는 buckets를 모두 free한다
        for (int i = 0; i < this->num_buckets; i++) {
            delete this->buckets[i];
        }
        debug("-\n");
        // 새로운 buckets를 This에 할당
        this->count = new_count;
        this->num_buckets = new_num_buckets;
        this->buckets = new_buckets;
    }

    ~HashMap() {
        // 기존의 This에 들어 있는 buckets를 모두 free한다
        for (int i = 0; i < this->num_buckets; i++) {
            delete this->buckets[i];
        }
    }

    bool put(Key key, Value value) {
        int h = hash(key);
        int which_bucket = h % this->num_buckets;
        debug("hash = %d, key = %d, which_bucket = %d, num_buckets = %d\n", h, key, which_bucket, this->num_buckets);
        bool inserted = this->buckets[which_bucket]->put(key, value); 
        if (inserted) {
            this->count += 1;
            this->resize();
            debug("-\n");
            return true;
        }
        debug("-\n");
        return false;
    }

    Value get(Key key) {
        int h = hash(key);
        int which_bucket = h % this->num_buckets;
        Bucket* bucket = this->buckets[which_bucket];
        return bucket->get(key);    
    }

    void del(Key key) {
        int h = hash(key);
        int which_bucket = h % this->num_buckets;
        Bucket* bucket = this->buckets[which_bucket];
        if (bucket->del(key)) this->count -= 1; 
    }

    void print() {
        Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            // iter = Iterator_next(iter);
            // if (iter == NULL) break;
            printf("(%d, %d)\n", iter->key, iter->value);
        }
    }

    std::vector<Key>* keys() {
        std::vector<Key>* key_s = new std::vector<Key>();
        Iterator* iter = this->iterator();
        
        while (iter->next() != NULL) {
            key_s->push_back(iter->key);
        }
        return key_s;   
    }

    std::vector<Value>* values() {
        std::vector<Value>* value_s = new std::vector<Value>();
        Iterator* iter = this->iterator();
        
        while (iter->next() != NULL) {
            value_s->push_back(iter->value);
        }
        return value_s;
    }

    bool contains(Key key) {
        return (this->get(key) != NOT_FOUND);
    }

    int get_count() {
        return this->count;
    }
};

class Set: public HashMap {
  public:  
    Set() {}
    
    ~Set() {}

    int count() {
        return this->get_count();
    }

    bool has(Element element) {
        return this->contains(element);
    }

    void add(Element element) {
        this->put(element, 1);
    }

    void remove(Element element) {
        this->del(element);
    }
    
    // 교집합을 생성해서 return함, return되는 Set은 delete해줘야 함
    Set* intersection(Set* set2) {
        Set* set3 = new Set();
        HashMap::Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            if (set2->has(iter->key)) {
                set3->add(iter->key);
            }
        }
        return set3;
    }

    Set* union_set(Set* set2) {
        Set* set3 = new Set();
        HashMap::Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            set3->add(iter->key);
        }
        iter = set2->iterator();
        while (iter->next() != NULL) {
            set3->add(iter->key);
        }
        return set3;
    }

    Set* difference(Set* set2) {
        Set* set3 = new Set();
        HashMap::Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            if (! set2->has(iter->key)) {
                set3->add(iter->key);
            }
        }
        return set3;
    }

    Set* symmetric_difference(Set* set2) {
        Set* set3 = new Set();
        HashMap::Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            if (! set2->has(iter->key)) {
                set3->add(iter->key);
            }
        }
        iter = set2->iterator();
        while (iter->next() != NULL) {
            if (! this->has(iter->key)) {
                set3->add(iter->key);
            }
        }
        return set3;
    }

    void print() {
        printf("count: %d {", this->count());
        HashMap::Iterator* iter = this->iterator();
        for (int i = 0; iter->next() != NULL; i++) {
            if (i == this->count() - 1) {
                printf("%d", iter->key);
            } else {
                printf("%d, ", iter->key);
            }
        }
        printf("}\n");
    }
};

int compare(int a, int b) {
    return b - a;
}

void qsort(std::vector<Key>& array, int left, int right, int (*compf)(int a, int b)) {
    if (right <= left) return;
    
    int pivot = array[right];
    int pivotIdx = left;
    for (int i = left; i < right; i++) {
        if (compf(array[i], pivot) > 0) {
            int temp = array[i];
            array[i] = array[pivotIdx];
            array[pivotIdx] = temp;
            pivotIdx += 1;
        }
    }
    int temp = array[right];
    array[right] = array[pivotIdx];
    array[pivotIdx] = temp;
    
    qsort(array, left, pivotIdx - 1, compf);
    qsort(array, pivotIdx + 1, right, compf);
}

int main() {
    HashMap* table = new HashMap();
    table->put(5, 500);
    table->put(7, 700);
    table->put(1, 100);
    table->put(3, 300);
    table->put(2, 200);
    table->put(15, 1500);
    table->put(17, 1700);
    table->put(11, 1100);
    table->put(13, 1300);
    table->put(12, 1200);
    table->print();
    Value val = table->get(3);
    printf("val : %d\n", val);
    Value val2 = table->get(9);
    printf("val : %d\n", val2);
    table->del(17);
    table->print();
    
    printf("----------\n");
    std::vector<Key>* keys = table->keys();
    qsort(*keys, 0, keys->size() - 1, compare);
    for (auto key : (*keys)) {
        printf("key: %d, value: %d\n", key, table->get(key));
    }
    printf("----------\n");
    delete table;

    Set* setx = new Set();
    setx->add(23);
    setx->add(37);
    setx->add(41);
    setx->add(23);
    setx->add(59);
    setx->add(73);
    setx->add(89);
    setx->add(97);

    setx->print();
    Set* sety = new Set();
    sety->add(37);
    sety->add(67);
    sety->add(83);
    sety->add(41);
    sety->print();
    Set* setz = setx->intersection(sety);
    setz->print();
    Set* setw = setx->union_set(sety);
    setw->print();
    Set* setu = setx->difference(sety);
    setu->print();
    Set* setk = sety->difference(setx);
    setk->print();
    Set* setl = setx->symmetric_difference(sety);
    setl->print();
    delete setx;
    delete sety;
    delete setz;
    delete setw;
    delete setu;
    delete setk;
    delete setl;

    return 0;
}