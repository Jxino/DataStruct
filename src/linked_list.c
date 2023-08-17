#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <assert.h>

// #define Debug printf("(%s,%d)", __FILE__, __LINE__); printf
#define Debug if (false) printf

// https://youtu.be/JlMyYuY1aXU (python)
// https://youtu.be/rS3xNaDpC5w (C)
// https://youtu.be/1ey5QpqbapU (Java)

// [List head]
//             [NodeX data nextX]
//             [Node1 data next1] // prev_node가 NULL인 경우
//                        [Node2 data next2] // prev_node는 Node1
//                                   [Node3 data NULL]
// 저장할 Data의 구조체를 선언
typedef struct{
    int age;
    char* name;
} Data;

Data* Data_create(int age, const char* name) {
    Data* This = (Data*) malloc(sizeof(Data));
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
    struct Node* prev;
    struct Node* next;
} Node;

Node* Node_create(Data* data) { // data를 포함하는 Node 객체 하나를 초기화해서 리턴한다
    Node* This = (Node*) malloc(sizeof(Node)); // 메모리를 할당받아온다
    This->data = data;
    This->prev = NULL;
    This->next = NULL;
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
    int count; // list에 들어 있는 node 개수를 기억한다
    Node* head;
    struct Iterator* iter;
} List;

typedef struct Iterator {
    List* list;
    Node* node;
    Node* next_node;
    Data* data;
} Iterator;

Iterator* Iterator_next(Iterator* iter) {
    if (iter->next_node == NULL) {
        return NULL;
    }
    iter->node = iter->next_node;
    iter->data = iter->node->data;
    iter->next_node = iter->node->next;
    return iter;
}

Iterator* List_iterator(List* This) {
    Iterator* iter = (Iterator*) malloc(sizeof(Iterator));
    iter->list = This;
    iter->node = NULL;
    iter->next_node = This->head;
    iter->data = NULL;
    This->iter = iter;
    return iter;
}

// List를 생성하는 함수 
List* List_create() {
    List* This = (List*) malloc(sizeof(List));
    This->head = NULL;
    This->count = 0;
    This->iter = NULL;
    return This; 
}

int List_count(List* This) {
    return This->count;
}

bool List_insert_at(List* This, int index, Data* data) {
    if (index < 0 || index > List_count(This)) // index 범위를 벗어나면 리턴 false 
        return false;
    Node* new_node = Node_create(data); // 새로 추가할 node 생성
    Node* cur_node = This->head;
    Node* prev_node = NULL;
    int i = 0;
    while (cur_node != NULL) { // 현재 node가 null인지 여부 검사
        if (index == i) { // 추가할 index 위치를 찾으면 그곳에 끼워 넣는다
            if (prev_node == NULL) { // 이전 node가 없으면 뒤로만 연결하면 됨
                new_node->next = cur_node; // 뒤로 연결
                cur_node->prev = new_node; 
                This->head = new_node; // 새로운 node가 list의 head가 됨
            } else { // 앞뒤로 다 연결
                new_node->next = cur_node; // 뒤로 연결 
                cur_node->prev = new_node; 
                prev_node->next = new_node; // 앞으로 연결
                new_node->prev = prev_node;
            }
            This->count += 1;
            return true; // 리턴
        }
        prev_node = cur_node;
        cur_node = cur_node->next; // 뒤로 이동 
        i += 1;
    }
    assert(index == List_count(This));
    if (prev_node == NULL) {
        assert(index == 0 && List_count(This) == 0);
        This->head = new_node;
    } else {
        prev_node->next = new_node;
        new_node->prev = prev_node;
    }
    This->count += 1;
    return true; // 여기엔 올 일이 없음
}

void List_prepend_item(List* This, Data* data) {
    // Node* node = Node_create(data); // data를 이용해서 node를 하나 생성한다
    
    // if (This->head == NULL) { // list가 비어 있으면 
    //     This->head = node;    // node를 list->head에 바로 할당
    // } else { 
    //     Node* prev_head = This->head; // 기존에 head에 있는 node를 temp에 피신시켜두고
    //     This->head = node;       // list->head에 새로 할당한 node를 할당
    //     node->next = prev_head;  // 피신시켜놓은 기존 head를 새로 할당한 node 꽁무니에 연결해준다
    //     prev_head->prev = node;  // 피신시켜놓은 prev_head->prev를 새로 들어온 node에 연결한다
    // }

    // This->count += 1;
    List_insert_at(This, 0, data);    
}

void List_delete(List* This) { // list 통째로 날린다
    Node* node = This->head;
    while (node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 delete한다
        Node* next_node = node->next;
        Node_delete(node);
        node = next_node;
    }
    if (This->iter != NULL) {
        free(This->iter);
    }
    free(This); // list객체 자체도 free시켜준다
}

void List_print(List* This) {
    Node* node = This->head;
    
    printf("count : %d ", This->count);
    while (node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
        Node* next_node = node->next;
        Node_print(node);
        node = next_node;
    }
    printf("\n");
}

void List_append_item(List* This, Data* data) {
    // Node* node = Node_create(data);
    // Node* cur_node = This->head;

    // This->count += 1;
    
    // if (This->head == NULL) { // list가 비어있으면
    //     This->head = node;    // list->head에 node를 연결해주고 끝낸다
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
    List_insert_at(This, This->count, data);
}

Data* List_get_item(List* This, int index) {
    Node* node = This->head;
    int i = 0;
    if (index < 0) {
        return NULL;
    }
    
    while (node != NULL) {
        if (index == i) {
            return node->data;
        }
        node = node->next;
        i += 1;
    }
    return NULL;
}

void List_remove_item(List* This, Data* data) {
    Node* node = This->head;
    Node* prev_node = NULL; // 최초에는 이전 node가 없다
    while (node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
        Node* next_node = node->next;
        if (node->data == data) { // 지워야 할 node가 식별됨
            Node_delete(node);
            This->count -= 1; 
            if (prev_node != NULL) { 
                if (next_node == NULL) {
                    prev_node->next = NULL;
                } else {
                    prev_node->next = next_node; // 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                    next_node->prev = prev_node;
                }
            } else { // 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                if (next_node == NULL) {
                    This->head = next_node;
                } else {
                    This->head = next_node; //list->head를 다음 node로 연결해 준다
                    next_node->prev = NULL;
                }
            }
        } 
        prev_node = node; // 이전 노드를 기억
        node = next_node; // 다음 노드로 이동
    }
}

Data* List_remove_at(List* This, int index) {
    Node* cur_node = This->head;
    Node* prev_node = NULL;
    int i = 0;
    if (index < 0) {
        return NULL;
    }
    
    Data* cur_data = NULL;
    while (cur_node != NULL) {
        Node* next_node = cur_node->next;
        if (index == i) {
            cur_data = cur_node->data; // 현재 node의 data를 피신시켜두고
            cur_node->data = NULL; // 현재 node의 data link를 끊고            
            Node_delete(cur_node);
            This->count -= 1;
            if (prev_node != NULL) { 
                if (next_node == NULL) {
                    prev_node->next = NULL;
                } else {
                    prev_node->next = next_node; // 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                    next_node->prev = prev_node;
                }
                
            } else { // 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                if (next_node == NULL) {
                    This->head = next_node;
                } else {
                    This->head = next_node; //list->head를 다음 node로 연결해 준다
                    next_node->prev = NULL;
                }
                
            }
        }
        prev_node = cur_node; // 이전 노드를 기억
        cur_node = next_node; // 다음 노드로 이동
        i += 1;
    }
    return cur_data;
}

int List_find(List* This, Data* data) {
    Node* cur_node = This->head;
    int i = 0;
    
    while (cur_node != NULL) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
        if (Data_equals(cur_node->data, data)) {
            return i;
        }
        cur_node = cur_node->next;
        i += 1;
    }
    return -1;
}

int compare(Node* a, Node* b) {
    int diff = b->data->age - a->data->age;
    if (diff != 0) return diff;
    if (strcmp(b->data->name, a->data->name) > 0) return 1;
    if (strcmp(a->data->name, b->data->name) > 0) return -1;
    return 0;
}

void swap(Node** array, int i, int j) {
    Data* temp = array[i]->data;
    array[i]->data = array[j]->data;
    array[j]->data = temp;
}

void q_sort(Node** array, int left, int right, int (*compf)(Node* a, Node* b)) {
    if (right <= left) return;
    Node* pivot = array[right];
    int pivotIdx = left;
    for (int i = left; i < right; i++) {
        if (compf(array[i], pivot) > 0) {
            swap(array, i, pivotIdx);
            pivotIdx += 1;
        }
    }
    swap(array, right, pivotIdx);

    q_sort(array, left, pivotIdx - 1, compf);
    q_sort(array, pivotIdx + 1, right, compf);
}

void List_sort(List* This) {
    Node** array = (Node**) malloc(sizeof(Node*) * List_count(This));
    Iterator* iter = List_iterator(This);
    for (int i = 0; Iterator_next(iter) != NULL; i++) {
        array[i] = iter->node;
    }
    q_sort(array, 0, List_count(This) - 1, compare);
    free(array);
}

typedef struct{
    List* list;
} Stack;

Stack* Stack_create() {
    Stack* This = (Stack*) malloc(sizeof(Stack));
    List* list = List_create();
    This->list = list;
    return This;
}

void Stack_delete(Stack* This) {
    List_delete(This->list);
    free(This);
}

void Stack_push(Stack* This, Data* data) {
    List_append_item(This->list, data);
}

Data* Stack_pop(Stack* This) {
    return List_remove_at(This->list, List_count(This->list) - 1);
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
    return List_remove_at(This->list, 0);
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
    Debug("-\n");
    
    Stack* s1 = Stack_create();
    Data* sdata = Data_create(20, "김유찬");
    Stack_push(s1, sdata);
    Data* sdata_2 = Data_create(20, "김준");
    Stack_push(s1, sdata_2);
    Debug("-\n");
    Stack_print(s1);
    Debug("-\n");
    Data* sdata_x = Stack_pop(s1);
    if (sdata_x != NULL){
        Data_print(sdata_x); printf("\n");
        Data_delete(sdata_x);
    }
    Debug("-\n");    
    sdata_x = Stack_pop(s1);
    if (sdata_x != NULL){
        Data_print(sdata_x); printf("\n");
        Data_delete(sdata_x);
    }
    Debug("-\n");
    sdata_x = Stack_pop(s1);
    if (sdata_x != NULL){
        Data_print(sdata_x); printf("\n");
        Data_delete(sdata_x);
    }
    Debug("-\n");
    Data* sdata_3 = Data_create(20, "안재형");
    Stack_push(s1, sdata_3);
    Stack_print(s1);
    Debug("-\n");
    Stack_delete(s1);
    Debug("-\n");
    
    Q* q = Q_create();
    Data* qdata = Data_create(20, "김유찬");
    Q_enqueue(q, qdata);
    Data* qdata_2 = Data_create(20, "김태환");
    Q_enqueue(q, qdata_2);
    Data* qdata_3 = Data_create(20, "이호준");
    Q_enqueue(q, qdata_3);
    Data* qdata_4 = Q_dequeue(q);
    if (qdata_4 != NULL) {
        Data_print(qdata_4); printf("\n");
        Data_delete(qdata_4);
    }    
    Q_print(q);
    Q_delete(q);
    return 0;
}