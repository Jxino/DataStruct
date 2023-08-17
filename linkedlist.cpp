#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <assert.h>
#include <vector>
    
#define Debug printf("(%s,%d)", __FILE__, __LINE__); printf

class Data {
  public:
    int age;
    std::string name;
    
    Data(int age, const char* name) {
        this->age = age;
        this->name = name;
    }

    ~Data() {

    }

    bool equals(Data* data) { // this == data
        if (this->age != data->age) return false; 
        if (this->name != data->name) return false;
        return true;
    }

    void print() {
        printf("(%d, %s)", this->age, this->name.c_str());
        // std::cout << "(name:" << this->name << ", age:" << this-age << ")";
    }
};

class Node {
  private:
  public:
    Data* data;
    Node* prev;
    Node* next;
    
    Node(Data* data_) { // 생성자(constructor)
        this->data = data_;
        this->prev = NULL;
        this->next = NULL;
    }

    void print() {
        this->data->print();
    }
    
    ~Node() { // 소멸자(destructor)

    }
};

class List {
  public:
    class Iterator {
      public:    
        List* list;
        Node* node;
        Node* next_node;
        Data* data;
    
      public:  
        Iterator* next() {
            if (this->next_node == NULL) {
                return NULL;
            }
            this->node = this->next_node;
            this->data = this->node->data;
            this->next_node = this->node->next;
            return this;
        }
    };
  
  private:
    int count_; // list에 들어 있는 node 개수를 기억한다
    Node* head;
    Iterator* iter;
  
  public:
    List() {
        this->head = NULL;
        this->count_ = 0;
        this->iter = NULL;
    }

    void prepend_item(Data* data) {
        // Node* node = new Node(data); // data를 이용해서 node를 하나 생성한다
        
        // if (this->head == NULL) { // list가 비어 있으면 
        //     this->head = node;    // node를 list->head에 바로 할당
        // } else { 
        //     Node* prev_head = this->head; // 기존에 head에 있는 node를 temp에 피신시켜두고
        //     this->head = node;       // list->head에 새로 할당한 node를 할당
        //     node->next = prev_head;  // 피신시켜놓은 기존 head를 새로 할당한 node 꽁무니에 연결해준다
        //     prev_head->prev = node;  // 피신시켜놓은 prev_head->prev를 새로 들어온 node에 연결한다
        // }

        // count_ += 1;
        this->insert_at(0, data);
    }

    void print() {
        Node* cur_node = this->head;
        
        printf("count_ : %d ", count_);
        while (cur_node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            cur_node->data->print();
            cur_node = cur_node->next;
        }
        printf("\n");
    }

    int count() {
        return count_;
    }

    void append_item(Data* data) {
        // Node* node = new Node(data);
        // Node* cur_node = this->head;

        // count_ += 1;
        
        // if (this->head == NULL) { // list가 비어있으면
        //     this->head = node;    // list->head에 node를 연결해주고 끝낸다
        //     return;
        // } 

        // while (cur_node != NULL) { // 현재 node가 NULL이 아닌 동안에 
        //     if (cur_node->next == NULL) { // 마지막 node면
        //         cur_node->next = node;    // node를 연결해주고 끝낸다
        //         node->prev = cur_node;
        //         return;
        //     }
        //     cur_node = cur_node->next; // 다음 node로 넘어간다
        // } 
        this->insert_at(this->count(), data);
    }

    Iterator* iterator() {
        Iterator* iter = new Iterator();
        iter->list = this;
        iter->node = NULL;
        iter->next_node = this->head;
        iter->data = NULL;
        this->iter = iter;
        return iter;
    }    

    Data* get_item(int index) {
        int i = 0;
        if (index < 0) {
            return NULL;
        }
        
        Node* cur_node = this->head;
        while (cur_node != NULL) {
            if (index == i) {
                return cur_node->data;
            }
            cur_node = cur_node->next;
            i += 1;
        }
        return NULL;
    }

    void remove_item(Data* data) {
        Node* cur_node = this->head;
        Node* prev_node = NULL; // 최초에는 이전 node가 없다
        while (cur_node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            Node* next_node = cur_node->next;
            if (cur_node->data->equals(data)) { // 지워야 할 node가 식별됨
                delete cur_node;
                count_ -= 1; 
                if (prev_node != NULL) { 
                    if (next_node == NULL) {
                        prev_node->next = NULL;
                    } else {
                        prev_node->next = next_node; // 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                        next_node->prev = prev_node;
                    }
                } else { // 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                    if (next_node == NULL) {
                         this->head = next_node;
                    } else {
                        this->head = next_node; //list->head를 다음 node로 연결해 준다
                        next_node->prev = NULL;
                    }
                }
            } 
            prev_node = cur_node; // 이전 노드를 기억
            cur_node = next_node; // 다음 노드로 이동
        }
    }

    Data* remove_at(int index) {
        Node* cur_node = this->head;
        Node* prev_node = NULL;
        int i = 0;
        if (index < 0) return NULL;
        
        Data* cur_data = NULL;
        while (cur_node != NULL) {
            Node* next_node = cur_node->next;
            if (index == i) {
                cur_data = cur_node->data; // 현재 node의 data를 피신시켜두고
                cur_node->data = NULL; // 현재 node의 data link를 끊고
                delete cur_node; // node를 삭제한다
                count_ -= 1;
                if (prev_node != NULL) { 
                    if (next_node == NULL) {
                        prev_node->next = NULL;
                    } else {
                        prev_node->next = next_node; // 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                        next_node->prev = prev_node;
                    }
                    
                } else { // 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                    if (next_node == NULL) {
                        this->head = next_node;
                    } else {
                        this->head = next_node; //list->head를 다음 node로 연결해 준다
                        next_node->prev = NULL;
                    }
                }
            }
            prev_node = cur_node; // 이전 노드를 기억
            cur_node = next_node; // 다음 노드로 이동
            i += 1;
        }
        return cur_data; // 피신시켜놓은 현재 node의 data를 리턴한다
    }

    ~List() {
        Node* cur_node = this->head;
        while (cur_node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 delete한다
            Node* next_node = cur_node->next;
            delete cur_node;
            cur_node = next_node;
        }
        if (this->iter != NULL) {
            delete this->iter;        
        }
    }

    int find(Data* data) {
        Node* cur_node = this->head;
        int i = 0;
        
        while (cur_node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            if (cur_node->data->equals(data)) {
                return i;
            }
            cur_node = cur_node->next;
            i += 1;
        }
        return -1;
    }

    bool insert_at(int index, Data* data) {
        if (index < 0 || index > this->count()) // index 범위를 벗어나면 리턴 false 
            return false;
        Node* new_node = new Node(data); // 새로 추가할 node 생성
        Node* cur_node = this->head;
        Node* prev_node = NULL;
        int i = 0;
        while (cur_node != NULL) { // 현재 node가 null인지 여부 검사
            if (index == i) { // 추가할 index 위치를 찾으면 그곳에 끼워 넣는다
                if (prev_node == NULL) { // 이전 node가 없으면 뒤로만 연결하면 됨
                    new_node->next = cur_node; // 뒤로 연결
                    cur_node->prev = new_node; 
                    this->head = new_node; // 새로운 node가 list의 head가 됨
                } else { // 앞뒤로 다 연결
                    new_node->next = cur_node; // 뒤로 연결 
                    cur_node->prev = new_node; 
                    prev_node->next = new_node; // 앞으로 연결
                    new_node->prev = prev_node;
                }
                this->count_ += 1; 
                return true; // 리턴
            }
            prev_node = cur_node;
            cur_node = cur_node->next; // 뒤로 이동 
            i += 1;
        }
        assert(index == this->count());
        if (prev_node == NULL) {
            assert(index == 0 && this->count() == 0);
            this->head = new_node;
        } else {
            prev_node->next = new_node;
            new_node->prev = prev_node;
        }
        this->count_ += 1;
        return true;
    }

    void sort() {
        std::vector<Node*> array;
        Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            array.push_back(iter->node);
        }
        qsort(array, 0, this->count() - 1, compare);
    }

    static int compare(Node* a, Node* b) {
        int diff = b->data->age - a->data->age;
        if (diff != 0) return diff;
        return strcmp(b->data->name.c_str(), a->data->name.c_str());
    }
    
    static void qsort(std::vector<Node*>& array, int left, int right, int (*compf)(Node* a, Node* b)) {
        if (right <= left) return;

        Node* pivot = array[right];
        int pivotIdx = left;
        for (int i = left; i < right; i++) {
            if (compf(array[i], pivot) > 0) {
                Data* temp = array[i]->data;
                array[i]->data = array[pivotIdx]->data;
                array[pivotIdx]->data = temp;
                pivotIdx += 1; 
            }
        }
        Data* temp = array[right]->data;
        array[right]->data = array[pivotIdx]->data;
        array[pivotIdx]->data = temp;

        qsort(array, left, pivotIdx - 1, compf);
        qsort(array, pivotIdx + 1, right, compf);
    }    
};

class Stack: public List {
  public:
    Stack() {
        
    }

    void push(Data* data) {
        this->append_item(data);
    }

    Data* pop() {
        return this->remove_at(this->count() - 1);
    }

    void print() {
        List::print();
    }

    ~Stack() {
        
    }
};

class Que: public List {
  public:
    Que() {
              
    }

    void enqueue(Data* data) {
        this->append_item(data);
    }

    Data* dequeue() {
        return this->remove_at(0);
    }

    void print() {
        List::print();
    }

    ~Que() {
        
    }
};

int main() {
    List* list = new List();
    Data* data = new Data(20, "Jinho");
    list->prepend_item(data);
    Data* data_2 = new Data(52, "Hiongun");
    list->prepend_item(data_2);
    Data* data_3 = new Data(50, "Sujung");
    list->prepend_item(data_3);
    Data* data_4 = new Data(20, "Junho");
    list->prepend_item(data_4);
    Data* data_5 = new Data(20, "Junhyuk");
    list->prepend_item(data_5);
    list->print();
    list->insert_at(1, new Data(20, "Yongjun"));
    list->print();
    Data* yongju = new Data(20, "Yongju");
    int idx = list->find(yongju);
    printf("idx = %d\n", idx);        
    delete yongju;
    Data* data_6 = new Data(20, "Min");
    list->append_item(data_6);
    printf("%d\n", list->count());
    list->remove_item(data_4);
    printf("%d\n", list->count());
    list->print();

    list->sort();
    printf("-----\n");
    List::Iterator* iter = list->iterator();
    while (iter->next() != NULL) {
        Data* data = iter->data;
        data->print();
    }
    printf("\n-----\n");    
    delete list;
    Debug("-\n");
    
    Stack* s1 = new Stack();
    Data* sdata = new Data(20, "김유찬");
    s1->push(sdata);
    Data* sdata_2 = new Data(20, "김준");
    s1->push(sdata_2);
    s1->print();
    Debug("-\n");    
    Data* sdata_x = s1->pop();
    if (sdata_x != NULL){
        sdata_x->print(); printf("\n");
        delete sdata_x;
    }
    sdata_x = s1->pop();
    if (sdata_x != NULL){
        sdata_x->print(); printf("\n");
        delete sdata_x;
    }
    Debug("-\n");
    sdata_x = s1->pop();
    if (sdata_x != NULL) {
        sdata_x->print(); printf("\n");
        delete sdata_x;        
    }
    Debug("-\n");    
    Data* sdata_3 = new Data(20, "안재형");
    s1->push(sdata_3);
    s1->print();
    delete s1;
    Debug("-\n");
    
    Que* q = new Que();
    Data* qdata = new Data(20, "김유찬");
    q->enqueue(qdata);
    Data* qdata_2 = new Data(20, "김태환");
    q->enqueue(qdata_2);
    Data* qdata_3 = new Data(20, "이호준");
    q->enqueue(qdata_3);
    Data* qdata_4 = q->dequeue();
    if (qdata_4 != NULL) {
        qdata_4->print(); printf("\n");
        delete qdata_4;
    }
    q->print();
    delete q;
    return 0;
}