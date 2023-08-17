#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <vector>
#include <assert.h>

#define Debug printf("(%s,%d)", __FILE__, __LINE__); printf

// 저장할 Data의 구조체를 선언
// typedef int Data;
class Data {
  public:
    int age;
    std::string name;
    
    Data(int age, const char* name) {
        this->age = age;
        this->name = name;
    }

    void print() {
        printf("(%d, %s)", this->age, this->name.c_str());
    }

    bool equals(Data* data) { // this == data
        if (this->age != data->age) return false; 
        if (this->name != data->name) return false;
        return true;
    }    

    ~Data() {
        
    }
};

// Node 구조체를 선언한다.
// data필드는 저장할 데이터를 담고 있고 next는 다음 Node를 가리키는 포인터다.
class Node {

  public:
    Data* data;
    
    Node(Data* data) { // data를 포함하는 Node 객체 하나를 초기화해서 리턴한다
        this->data = data;
    }

    void print() {
        this->data->print();
    }    
    
    ~Node() {
        delete this->data;
    }
};

// List는 Node를 가리키는 head포인터를 가지고 있다. 
class List {
  public:  
    class Iterator {
      public:
        List* list;
        int idx;
        int next_idx;
        Data* data;

      public:  
        Iterator* next() {
            if (this->next_idx >= this->list->get_count()) {
                return NULL;
            }
            this->idx = this->next_idx;
            this->data = this->list->array[this->idx]->data;
            this->next_idx = this->idx + 1;
            return this;
        }
    };
  
    Iterator* iterator() {
        Iterator* iter = new Iterator();
        iter->list = this;
        iter->idx = 0;
        iter->next_idx = 0;
        iter->data = NULL;
        this->iter = iter;
        return iter;
    }
  
  public:
    std::vector<Node*> array; // node포인터들을 담고 있는 array
    List::Iterator* iter;

  public:
    
    // List를 생성하는 함수 
    List() { 
        this->iter = NULL;
    }

    void prepend_item(Data* data) { // List에 data를 추가하는 함수
        // Node* node = new Node(data); // data를 이용해서 node를 하나 생성한다
        // this->array.push_back(node);
        this->insert_at(0, data);
    }

    void print() { // 
        for(int i = 0; i < this->array.size(); i++) {
            this->array[i]->print();                                    
        }
        printf("\n");
    }

    void remove_item(Data* data) { // list에서 data 삭제
        // for (int i = 0; i < this->array.size(); i++) { 
        //     if (this->array[i]->data->equals(data)) { // 지울 data가 들어 있는 node를 찾으면 
        //         delete this->array[i];
        //         this->array.erase(this->array.begin() + i); // 해당 node를 지운다
        //     }                                        
        // }
        Data* removed = this->remove_at(this->find(data));
        if (removed != NULL)
            delete removed;
    }

    int get_count() {
        return this->array.size();
    }

    void append_item(Data* data) { // list에 data를 덧붙이는 함수
        // Node* node = new Node(data); // 입력받는 data로 node 생성
        // this->array.push_back(node); 
        this->insert_at(this->get_count(), data);
    }

    Data* get_item(int index) {
        if (index < 0 || index >= this->array.size()) {
            return NULL;
        }
        Debug("index: %d\n", index);
        return this->array[index]->data;
    }

    Data* remove_at(int index) {
        if (index < 0 || index >= this->array.size()) {
            return NULL;
        }
        // Node_delete(list->array[index]);
        Data* data = this->array[index]->data; // 일단 data를 피신시키고 
        this->array[index]->data = NULL; // node와 data와의 연결을 끊고
        delete this->array[index]; // node를 지우고
        this->array.erase(this->array.begin() + index); // node포인터도 지운다 
        return data;
    }

    Data* get(int index) {
        return this->array[index]->data;
    }

    ~List() { // list 통째로 날린다
        for (int i = 0; i < this->array.size(); i++) {
            delete this->array[i]; // list->array 내부의 node 포인터 저장 공간 반납
        }
        if (this->iter != NULL) {
            delete this->iter;
        }
    }

    int find(Data* data) {
        for (int i = 0; i < this->array.size(); i++) {
            if (this->array[i]->data->equals(data)) {
                return i;
            }
        }
        return -1;
    }

    bool insert_at(int index, Data* data) {
        if (index < 0 || index > this->get_count()) // index 범위를 벗어나면 리턴 false 
            return false;
        Node* node = new Node(data);
        if (index < this->get_count()) {
            this->array.insert(this->array.begin() + index, node);
        } else {
            this->array.push_back(node);
        }
        return true;
    }

    void sort() {
        qsort(this->array, 0, this->get_count() - 1, compare);
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

class Stack: public List { // Stack이 List를 상속받는다(Stack inherits List, Stack extends List)
  public:
    Stack() {

    }

    void push(Data* data) {
        this->append_item(data);
    }

    Data* pop() {
        int count = this->get_count();
        Data* data = this->remove_at(count - 1);
        return data;
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
        return this->remove_at(0);;
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
    printf("%d\n", list->get_count());
    list->remove_item(data_4);
    printf("%d\n", list->get_count());
    list->print();

    printf("-----\n");
    List::Iterator* iter = list->iterator();
    while (iter->next() != NULL) {
        Data* data = iter->data;
        data->print();
    }    
    list->sort();
    printf("-----\n");
    iter = list->iterator();
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