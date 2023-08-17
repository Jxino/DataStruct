 #include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// #define debug printf("(%s,%d)", __FILE__, __LINE__); printf
#define debug if (false) printf

// 저장할 Data의 구조체를 선언
// typedef int Data;
typedef struct{
    int age;
    char* name;
} Data;

Data* Data_create(int age, const char* name) {
    Data* This = (Data*) malloc(sizeof(Data));
    memset(This, 0, sizeof(Data));
    This->age = age;
    This->name = (char*) malloc(strlen(name) + 1);
    strcpy(This->name, name);
    return This;
}

void Data_delete(Data* This) {
    free(This->name);
    free(This);
}

void Data_print(Data* This) {
    if (This == NULL) {
        printf("NULL");
    } else {
        printf("(%d, %s)", This->age, This->name);
    }
}

bool Data_equals(Data* This, Data* data) { // this == data
    if (This->age != data->age) return false; 
    if (strcmp(This->name, data->name) != 0) return false;
    return true;
}


// Node 구조체를 선언한다.
// data필드는 저장할 데이터를 담고 있고 next는 다음 Node를 가리키는 포인터다.
typedef struct Node{
    Data* data;
} Node;

Node* Node_create(Data* data) { // data를 포함하는 Node 객체 하나를 초기화해서 리턴한다
    Node* This = (Node*) malloc(sizeof(Node)); // 메모리를 할당받아온다
    This->data = data;
    return This;
}
                                                                               
void Node_delete(Node* This) { 
    free(This->data);
    free(This); // 메모리를 free해 준다
}

void Node_print(Node* This) {
    Data_print(This->data);
}

// List는 Node를 가리키는 head포인터를 가지고 있다. 
typedef struct{
    int count; // list에 들어 있는 node 개수를 기억한다 : 초기값 0
    int capacity; // 최대 용량 : 초기값 10
    Node** array; // node포인터들을 담고 있는 array
    struct Iterator* iter;
} List;

typedef struct Iterator {
    List* list;
    int idx;
    int next_idx;
    Data* data;
} Iterator;

Iterator* Iterator_next(Iterator* iter) {
    assert(iter != NULL);
    if (iter->next_idx >= iter->list->count) {
        return NULL;
    }
    iter->idx = iter->next_idx;
    iter->data = iter->list->array[iter->idx]->data;
    iter->next_idx += 1;
    return iter;
}

Iterator* List_iterator(List* This) {
    Iterator* iter = (Iterator*) malloc(sizeof(Iterator));
    iter->list = This;
    iter->idx = 0;
    iter->next_idx = 0;
    iter->data = NULL;
    This->iter = iter;
    return iter;
}

// List를 생성하는 함수 
List* List_create() { 
    List* This = (List*) malloc(sizeof(List)); // List 객체를 동적으로 생성한다 
    This->count = 0; // 개수를 0으로 초기화
    This->capacity = 10; // 초기 용량은 10으로 초기화
    This->array = (Node**) malloc(sizeof(Node*) * This->capacity); // node들을 저장할 array를 동적으로 생성
    This->iter = NULL;
    return This; // List 포인터 리턴
}

bool List_insert_at(List* This, int index, Data* data) {
    if (index < 0 || index > This->count) // index 범위를 벗어나면 리턴 false 
        return false;
    Node* node = Node_create(data);
    
    if (This->count == This->capacity) { // 꽉 찼으면, 용량을 확장한다
        This->capacity += 10;
        Node** new_array = (Node**) malloc(sizeof(Node*) * This->capacity);
        for (int i = 0; i < This->count; i++) {
            new_array[i] = This->array[i];
        }
        free(This->array);
        This->array = new_array; 
    }
    for (int i = This->count - 1; i >= index; i--) {
        This->array[i + 1] = This->array[i];
    }
    This->array[index] = node;
    This->count += 1;
    return true;
}

void List_prepend_item(List* This, Data* data) { // List에 data를 추가하는 함수
    // Node* node = Node_create(data); // data를 이용해서 node를 하나 생성한다
    // if (This->count < This->capacity) { // 용량이 남아 있으면
    //     This->array[This->count] = node; // node를 array의 count 위치에 할당
    // } else { // 용량이 부족하면
    //     This->capacity += 10; // 용량을 10만큼 증가시킴
    //     Node** new_array = (Node**) malloc(sizeof(Node*) * This->capacity); // 새로운 용량만큼 array 동적으로 생성
    //     for (int i = 0; i < This->count; i++) { 
    //         new_array[i] = This->array[i]; // new_array에 기존 array 값들 전부 복붙한다
    //     } 
    //     new_array[This->count] = node; // new_array의 count 위치에 새로운 node도 추가함
    //     free(This->array); // 기존 array 공간 반납
    //     This->array = new_array; // new_array를 list->array에 할당
    // }
    // This->count += 1; // list->count 값 1 증가시킴
    List_insert_at(This, 0, data);
}

void List_delete(List* This) { // list 통째로 날린다
    debug("-\n");
    for (int i = 0; i < This->count; i++) {
        debug("-\n");
        Node_delete(This->array[i]); // list->array 내부의 node 포인터 저장 공간 반납
    }
    debug("-\n");
    free(This->array); // list->array 반납
    debug("-\n");
    if (This->iter != NULL) {
        free(This->iter);
    }
    debug("-\n");
    free(This); // list객체 자체도 free시켜준다
}

void List_print(List* This) { // 
    for(int i = 0; i < This->count; i++) {
        Node_print(This->array[i]);                                    
    }
    printf("\n");
}

void List_remove_item(List* This, Data* data) { // list에서 data 삭제
    for (int i = 0; i < This->count; i++) { 
        if (Data_equals(This->array[i]->data, data)) { // 지울 data가 들어 있는 node를 찾으면 
            Node_delete(This->array[i]); // 해당 node를 지운다
            for (int k = i; k < (This->count - 1); k++) { 
                    This->array[k] = This->array[k + 1]; // 뒤에 있는 node들을 한 칸씩 앞으로 옮겨 준다
            }
        }                                        
    }
    This->count -= 1; // This->count 값을 1 증가시킴
}

int List_count(List* This) {
    return This->count;
}

void List_append_item(List* This, Data* data) { // This에 data를 덧붙이는 함수
    // Node* node = Node_create(data); // 입력받는 data로 node 생성
    // if (This->count < This->capacity) { // 용량이 남으면
    //     This->array[This->count] = node; // 뒤에 node 추가 
    // } else { // 용량이 부족하면
    //     This->capacity += 10; // 용량을 10 증가시키고
    //     Node** new_array = (Node**) malloc(sizeof(Node*) * This->capacity); // 새로운 용량만큼 new_array 할당
    //     for (int i = 0; i < This->count; i++) {
    //         new_array[i] = This->array[i]; // new_array에 기존 array 값들 전부 복붙
    //     } 
    //     new_array[This->count] = node; // 입력받은 data가 들어 있는 node도 new_array 뒤에 추가
    //     free(This->array); // 기존 array 저장 공간 반납
    //     This->array = new_array; // This->array에 new_array 할당
    // }
    // This->count += 1; // This->count 값 1 증가시킴
    List_insert_at(This, This->count, data);
}

Data* List_get_item(List* This, int index) {
    if (index < 0 || index >= This->count) {
        return NULL;
    }
    debug("index: %d\n", index);
    return This->array[index]->data;
}

Data* List_remove_at(List* This, int index) {
    if (index < 0 || index >= This->count) {
        return NULL;
    }
    // Node_delete(This->array[index]);
    Data* data = This->array[index]->data;
    This->array[index]->data = NULL;
    Node_delete(This->array[index]); 
    for (int i = index; i < (This->count - 1); i++) {
        This->array[i] = This->array[i + 1];
    }
    This->count -= 1;
    return data;
}

int List_find(List* This, Data* data) {
    for (int i = 0; i < This->count; i++) {
        if (Data_equals(This->array[i]->data, data)) {
            return i;
        }
    }
    return -1;
}

Data* List_get(List* list, int index) {
    assert(list != NULL);
    if (index < 0 || index >= list->count) {
        return NULL;
    }
    return list->array[index]->data;
}

void List_put(List* list, int index, Data* data) {
    List_insert_at(list, index, data);
}

int compare(Node* a, Node* b) {
    int diff = b->data->age - a->data->age;
    if (diff != 0) return diff;
    if (strcmp(b->data->name, a->data->name) > 0) return 1;
    if (strcmp(a->data->name, b->data->name) > 0) return -1;
    return 0;
}

void q_sort(Node** array, int left, int right, int (*compf)(Node* a, Node* b)) {
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

    q_sort(array, left, pivotIdx - 1, compf);
    q_sort(array, pivotIdx + 1, right, compf);
}

void List_sort(List* This) {
    q_sort(This->array, 0, List_count(This) - 1, compare);
}

typedef struct Stack {
    List* list;
} Stack;

Stack* Stack_create() {
    Stack* This = (Stack*) malloc(sizeof(Stack));
    List* list = List_create();
    This->list = list;
    return This;
}

void Stack_delete(Stack* This) {
    debug("-\n");
    List_delete(This->list);
    debug("-\n");
    free(This);
}

void Stack_push(Stack* This, Data* data) {
    List_append_item(This->list, data);
}

Data* Stack_pop(Stack* This) {
    int count = List_count(This->list);
    Data* data = List_remove_at(This->list, count - 1);
    return data;
}

void Stack_print(Stack* This) {
    List_print(This->list);
}

typedef struct {
    List* list;
} Q;

Q* Q_create() {
    Q* This = (Q*) malloc(sizeof(Q));
    This->list = List_create();
    return This;
}

void Q_delete(Q* This) {
    List_delete(This->list);
    free(This);
}

void Q_enqueue(Q* This, Data* data) {
    List_append_item(This->list, data);
}

Data* Q_dequeue(Q* This) {
    Data* data =  List_get_item(This->list, 0);
    List_remove_at(This->list, 0);
    return data;
}

void Q_print(Q* This) {
    List_print(This->list);
}

int main() {
    List* list = List_create();
    Data* data = Data_create(20, "Jinho");
    List_prepend_item(list, data);
    Data* data_2 = Data_create(52, "Hiongun");
    List_prepend_item(list, data_2);
    Data* data_3 = Data_create(50, "Sujung");
    List_prepend_item(list, data_3);
    Data* data_4 = Data_create(20, "Junho");
    List_prepend_item(list, data_4);
    Data* data_5 = Data_create(20, "Junhyuk");
    List_prepend_item(list, data_5);
    List_print(list);
    List_insert_at(list, 1, Data_create(20, "Yongjun"));
    List_print(list);
    Data* yongju = Data_create(20, "Yongju");
    int idx = List_find(list, yongju);
    printf("idx = %d\n", idx);        
    Data_delete(yongju);
    Data* data_6 = Data_create(20, "Min");
    List_append_item(list, data_6);
    printf("%d\n", List_count(list));
    List_remove_item(list, data_4);
    printf("%d\n", List_count(list));
    List_print(list);
    
    List_sort(list);
    printf("-----\n");
    Iterator* iter = List_iterator(list);
    while (Iterator_next(iter) != NULL) {
        Data* data = iter->data;
        Data_print(data);
    }
    printf("\n-----\n");
    List_delete(list);
    debug("-\n");

    Stack* s1 = Stack_create();
    Data* sdata = Data_create(20, "김유찬");
    Stack_push(s1, sdata);
    Data* sdata_2 = Data_create(20, "김준");
    Stack_push(s1, sdata_2);
    debug("-\n");
    Stack_print(s1);
    Data* sdata_x = Stack_pop(s1);
    debug("-\n");
    if (sdata_x != NULL){
        Data_print(sdata_x); printf("\n");
        Data_delete(sdata_x);
    }    
    sdata_x = Stack_pop(s1);
    debug("-\n");
    if (sdata_x != NULL){
        Data_print(sdata_x); printf("\n");
        Data_delete(sdata_x);
    }
    sdata_x = Stack_pop(s1);
    debug("-\n");
    if (sdata_x != NULL){
        debug("-\n");
        Data_print(sdata_x); printf("\n");
        Data_delete(sdata_x);
    }
    Data* sdata_3 = Data_create(20, "안재형");
    debug("-\n");
    Stack_push(s1, sdata_3);
    debug("-\n");
    Stack_print(s1);
    debug("-\n");
    Stack_delete(s1);
    debug("-\n");

    Q* q = Q_create();
    Data* qdata = Data_create(20, "김유찬");
    Q_enqueue(q, qdata);
    Data* qdata_2 = Data_create(20, "김태환");
    Q_enqueue(q, qdata_2);
    Data* qdata_3 = Data_create(20, "이호준");
    Q_enqueue(q, qdata_3);
    Data* qdata_4 = Q_dequeue(q);
    debug("-\n");
    if (qdata_4 != NULL) {
        Data_print(qdata_4); printf("\n");
        debug("-\n");        
        Data_delete(qdata_4);
    }    

    debug("-\n");
    Q_print(q);
    debug("-\n");
    Q_delete(q);
    debug("-\n");
    return 0;
}