// AVL tree implementation in C
// https://www.programiz.com/dsa/balanced-binary-tree

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// #define debug printf("(%s,%d)", __FILE__, __LINE__); printf
#define debug if (false) printf

typedef int Key;
typedef int Value;
typedef int Element;

// Create Node
typedef struct Node {
  Key key;
  Value value;
  struct Node *left;
  struct Node *right;
  int height;
} Node;

inline int Max(int a, int b) {
  return (a > b) ? a : b;
}

// Calculate height
int height(struct Node *This) {
  if (This == NULL)
    return 0;
  return This->height;
}

// Create a node
Node* newNode(Key key, Value value) {
  Node* This = (Node*) malloc(sizeof(Node));
  This->key = key;
  This->value = value;
  This->left = NULL;
  This->right = NULL;
  This->height = 1;
  return This;
}

// Right rotate
Node* rightRotate(Node* This) {
  assert(This->left != NULL);
  Node* x = This->left;
  Node* T2 = x->right;

  x->right = This;
  This->left = T2;

  This->height = Max(height(This->left), height(This->right)) + 1;
  x->height = Max(height(x->left), height(x->right)) + 1;

  return x; // 상위 node에서 새로운 subtree의 최상위 node를 연결하도록 리턴
}

// Left rotate
Node* leftRotate(Node* This) {
  Node* y = This->right;
  Node* T2 = y->left;

  y->left = This;
  This->right = T2;

  This->height = Max(height(This->left), height(This->right)) + 1;
  y->height = Max(height(y->left), height(y->right)) + 1;

  return y;
}

// Get the balance factor
int getBalance(Node* This) {
  if (This == NULL)
    return 0;
  return height(This->left) - height(This->right);
}

// 균형을 맞추고 결과적으로 최상위 node를 리턴, 균형이 맞으면 NULL을 리턴
Node* rebalance(Node* This, Key key) {
  int balance = getBalance(This); // balance factor가 -1, 0, 1이면 균형 잡힘, 절댓값이 2 이상이면 불균형  
  if (balance > 1) { // 왼쪽으로 기울어 있으면
    if (key < This->left->key) { // 왼쪽 사선으로 정렬되어 있으면, 오른쪽으로 회전
      return rightRotate(This); 
    }
    if (key > This->left->key) { // 왼쪽으로 삐뚤게 되어 있으면
      This->left = leftRotate(This->left); // 왼쪽 사선으로 정렬시킨 후 오른쪽으로 회전
      return rightRotate(This);      
    }
  }
  if (balance < -1) { // 오른쪽으로 기울어 있으면
    if (key > This->right->key) { // 오른쪽 사선으로 정렬되어 있으면, 왼쪽으로 회전
      return leftRotate(This);
    }
    if (key < This->right->key) { // 오른쪽으로 삐뚤게 되어 있으면
      This->right = rightRotate(This->right); // 오른쪽 사선으로 정렬시킨 후 왼쪽으로 회전
      return leftRotate(This);
    }
  }
  return NULL; // 좌우 균형이 맞으면
}

// This node 하위 tree에 key와 value를 insert하고, 결과적인 최상위 node를 리턴한다
Node* insertNode(Node* This, Key key, Value value) {
  // Find the correct position to insertNode the node and insertNode it
  if (This == NULL) // leaf node에 도달하면 새로 만들어 붙인다
    return (newNode(key, value));

  if (key < This->key)
    This->left = insertNode(This->left, key, value); // 좌측에 insert하고 최상위 node를 받아서 left에 붙인다
  else if (key > This->key)
    This->right = insertNode(This->right, key, value); // 우측에 insert하고 최상위 node를 받아서 right에 붙인다
  else { // key가 같으면 value를 덮어쓴다
    This->value = value;
    return This; // 최상위 node 변경 없음
  }

  // Update the balance factor of each node and
  // Balance the tree
  This->height = 1 + Max(height(This->left), height(This->right));

  // Rebalance
  Node* balanced = rebalance(This, key);
  if (balanced != NULL) return balanced; // balance 결과 변경된 최상위 node를 리턴

  return This; // inert는 했고, 최상위 node 변경은 없음
}

Node* minValueNode(Node* This) {
  Node* current = This;

  while (current->left != NULL)
    current = current->left;

  return current;
}

// 특정한 node를 기준으로 하위 tree에서 key를 찾아서 node를 지우고 그 결과 최상위 node를 리턴한다
Node* deleteNode(Node* This, Key key) {
  // Find the node and delete it
  if (This == NULL) // This가 NULL이면 지우고 할 것 없이 리턴
    return This;

  if (key < This->key)
    This->left = deleteNode(This->left, key); // 왼쪽에서 키를 지우고 결과로 생긴 최상위 node를 This->left에 연결
  else if (key > This->key)
    This->right = deleteNode(This->right, key);
  else { // key가 같으면 This를 지운다
    if ((This->left == NULL) || (This->right == NULL)) { // 좌우 자식 node 둘 중에 하나라도 NULL이면
      Node* temp = (This->left != NULL) ? This->left : This->right; 
      if (temp == NULL) { // 둘 다 NULL이면
        free(This);        
        return NULL;
      } else { // 좌우 sub tree가 하나(temp) 존재하면, 그것을 한 level 상승시킴
        *This = *temp; // 살아있는 놈(temp)을 한 단계 끌어올린다
        free(temp);
      }
    } else { // 좌우 자식 node 둘 다 있으면
      // 오른쪽에서 제일 작은 놈을 찾는다
      Node* temp = minValueNode(This->right);
      // 그놈을 최상위로 끌어올린다
      This->key = temp->key; 
      This->value = temp->value;
      // 그놈을 오른쪽에서 지운 후 그 결과 최상위 node를 This 오른쪽에 붙인다
      This->right = deleteNode(This->right, temp->key);
    }
  }

  // Update the balance factor of each node and
  // balance the tree
  This->height = 1 + Max(height(This->left), height(This->right));

  // Rebalance
  Node* balanced = rebalance(This, key);
  if (balanced != NULL) return balanced; // rebalance한 최상위 node를 리턴한다

  return This; // 지웠는데도 균형이 맞으면 This를 리턴한다
}

// Print the tree
void printPreOrder(Node* This) {
  if (This != NULL) {
    printf("%d:%d ", This->key, This->value);
    printPreOrder(This->left);
    printPreOrder(This->right);
  }
}

// This node 하위에서 key를 찾아서 그 value를 리턴하는데, 없으면 NULL을 리턴한다 
Value search(Node* This, Key key) {
  if (This == NULL) { // leaf node면 NULL 리턴 
    return NULL;
  }
  if (This->key == key) {
    return This->value;
  } else if (This->key < key) {
    // 오른쪽에서 찾은 value를 리턴한다
    return search(This->right, key);
  } else {
    return search(This->left, key);
  }
}

#define MAX_STACK_DEPTH 1000
typedef struct Stack {
  Node* array[MAX_STACK_DEPTH];
  int count;
} Stack;

typedef struct Iterator {
  Key key;
  Value value;
  Stack* stack;
  Node* current;
} Iterator;

typedef struct TreeMap {
  Node* root;
  int count;
  Iterator* iter;
} TreeMap;

TreeMap* TreeMap_create() {
  TreeMap* This = (TreeMap*) malloc(sizeof(TreeMap));
  This->root = NULL;
  This->count = 0;
  This->iter = NULL;
  return This;
}

// This node 하위 트리 전체를 지운다
void Node_delete(Node* This) {
  if (This == NULL) {
    return;
  }
  if (This->left != NULL) {
    Node_delete(This->left);
  }
  if (This->right != NULL) {
    Node_delete(This->right);
  }
  free(This);
}

void Node_print(Node* This, int level, int flag) {
  for (int i = 0; i < level; i++) {
    if (i == (level - 1) && flag == 1) { // 오른쪽일 때 +를 표시
      printf("+ ");
    } else {
      printf("| ");
    }
  }
  printf("(level = %d, key = %d, value = %d)\n", level, This->key, This->value);
  if (This->left != NULL) {
    Node_print(This->left, level + 1, 0);
  }
  if (This->right != NULL) {
    Node_print(This->right, level + 1, 1);
  }
}

int Node_count(Node* This) {
  if (This == NULL) {
    return 0;
  }
  return Node_count(This->left) + Node_count(This->right) + 1;
}

int TreeMap_count(TreeMap* This) {
  // return Node_count(This->root);
  return This->count;
}

void Node_fill_keys(Node* This, Key* keys, int* keys_i) {
  keys[*keys_i] = This->key;
  *keys_i += 1;
  if (This->left != NULL) {
    Node_fill_keys(This->left, keys, keys_i);
  }
  if (This->right != NULL) {
    Node_fill_keys(This->right, keys, keys_i);
  }
}

Key* TreeMap_keys(TreeMap* This) {
  if (This->root == NULL) {
    return NULL;
  }
  Key* keys = (Key*) malloc(sizeof(Key) * TreeMap_count(This));
  int keys_i = 0;
  Node_fill_keys(This->root, keys, &keys_i);
  return keys;
}

void Node_fill_values(Node* This, Value* values, int* values_i) {
  values[*values_i] = This->value;
  *values_i += 1;
  if (This->left != NULL) {
    Node_fill_values(This->left, values, values_i);
  }
  if (This->right != NULL) {
    Node_fill_values(This->right, values, values_i);
  }
}

Value* TreeMap_values(TreeMap* This) {
  if (This->root == NULL) {
    return NULL;
  }
  Value* values = (Value*) malloc(sizeof(Value) * TreeMap_count(This));
  int values_i = 0;
  Node_fill_values(This->root, values, &values_i);
  return values;
}

void TreeMap_delete(TreeMap* This) {
  // while (This->root != NULL) {
  //   Node* new_root = deleteNode(This->root, This->root->key);
  //   This->root = new_root;      
  // }
  Node_delete(This->root);
  free(This->iter);
  free(This);
}

// This TreeMap에서 key를 찾아서 상응하는 value*을 리턴하고, 없으면 NULL을 리턴한다
Value TreeMap_search(TreeMap* This, Key key) {
  return search(This->root, key);
}

bool TreeMap_contains(TreeMap* This, Key key) {
  if (TreeMap_search(This, key) == NULL) {
    return false;
  } else {
    return true;
  }
}

void TreeMap_insert(TreeMap* This, Key key, Value value) {
  if (! TreeMap_contains(This, key)) {
    This->count += 1;
  }
  // insert 하고 rebalancing 과정에서 root node의 변경이 일어날 수 있으므로 새로운 root를 기억
  This->root = insertNode(This->root, key, value);
}

void TreeMap_remove_item(TreeMap* This, Key key) {
  if (TreeMap_contains(This, key)) {
    This->count -= 1;
  }
  This->root = deleteNode(This->root, key);
}

void TreeMap_print(TreeMap* This) {
  if (This->root != NULL) {
    Node_print(This->root, 0, 0);
  }
}

Stack* Stack_create() {
  Stack* This = (Stack*) malloc(sizeof(Stack));
  This->count = 0;
  return This;
}

void Stack_push(Stack* This, Node* node) {
  This->array[This->count] = node;
  This->count += 1;
}

Node* Stack_pop(Stack* This) {
  This->count -= 1;
  return This->array[This->count];
}

int Stack_count(Stack* This) {
  return This->count;
}

Iterator* Iterator_next(Iterator* This) {
  while (This->current != NULL || This->stack->count != 0) {
    debug("-\n");    
    if (This->current != NULL) {
      Stack_push(This->stack, This->current);
      This->current = This->current->left;
      debug("-\n");
    } else {
      Node* node = Stack_pop(This->stack);
      This->value = node->value;
      This->key = node->key;
      This->current = node->right;
      debug("-\n");
      return This;
    }
  }
  debug("-\n");
  return NULL;
}

Iterator* Iterator_create(Node* node) {
  Iterator* This = (Iterator*) malloc(sizeof(Iterator));
  This->key = 0;
  This->value = 0;
  This->stack = Stack_create();
  This->current = node;
  debug("-\n");
  return This;
}

Iterator* TreeMap_iterator(TreeMap* This) {
  if (This->root == NULL) {
    debug("-\n");
    return NULL;
  }
  debug("-\n");
  Iterator* iter = Iterator_create(This->root);
  This->iter = iter;
  debug("-\n");
  return iter;
}

typedef struct Set {
    TreeMap* map;
} Set;

Set* Set_create() {
    Set* This = (Set*) malloc(sizeof(Set));
    This->map = TreeMap_create();
    return This;
}

int Set_count(Set* This) {
    return TreeMap_count(This->map);
}

bool Set_has(Set* This, Element element) {
    return TreeMap_contains(This->map, element);
}

void Set_add(Set* This, Element element) {
    TreeMap_insert(This->map, element, 1);
}

void Set_remove(Set* This, Element element) {
    TreeMap_remove_item(This->map, element);
}

Set* Set_intersection(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = TreeMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        if (Set_has(set2, iter->key)) {
            Set_add(set3, iter->key);
        }
    }
    return set3;
}

Set* Set_union(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = TreeMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        Set_add(set3, iter->key);
    }
    iter = TreeMap_iterator(set2->map);
    while (Iterator_next(iter) != NULL) {
        Set_add(set3, iter->key);
    }
    return set3;
}

Set* Set_difference(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = TreeMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        if (! Set_has(set2, iter->key)) {
            Set_add(set3, iter->key);
        }   
    }
    return set3;
}

Set* Set_symmetric_difference(Set* This, Set* set2) {
    Set* set3 = Set_create();
    Iterator* iter = TreeMap_iterator(This->map);
    while (Iterator_next(iter) != NULL) {
        if (! Set_has(set2, iter->key)) {
            Set_add(set3, iter->key);
        }   
    }
    iter = TreeMap_iterator(set2->map);
    while (Iterator_next(iter) != NULL) {
        if (! Set_has(This, iter->key)) {
            Set_add(set3, iter->key);
        }   
    }
    return set3;
}

void Set_print(Set* This) {
    printf("count: %d {", Set_count(This));
    Iterator* iter = TreeMap_iterator(This->map);
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
    TreeMap_delete(This->map);
    free(This);
}

int compare(int a, int b) {
  return b - a;
}

void qsort(int array[], int left, int right, int (*cmpf)(int, int)) {
  if (right <= left) return;

  int pivot = array[right];
  int pivotIdx = left;
  for (int i = left; i < right; i++) {
    if (cmpf(array[i], pivot) > 0) {
      int temp = array[i];
      array[i] = array[pivotIdx];
      array[pivotIdx] = temp;
      pivotIdx += 1;
    }
  }
  int temp = array[right];
  array[right] = array[pivotIdx];
  array[pivotIdx] = temp;

  qsort(array, left, pivotIdx - 1, cmpf);
  qsort(array, pivotIdx + 1, right, cmpf);
}

int main() {
  // Node* root = NULL;
  // root = insertNode(root, 2, Value_create(200));
  // root = insertNode(root, 1, Value_create(100));
  // root = insertNode(root, 7, Value_create(800));
  // root = insertNode(root, 4, Value_create(400));
  // root = insertNode(root, 5, Value_create(500));
  // root = insertNode(root, 3, Value_create(300));
  // root = insertNode(root, 8, Value_create(800));

  // printPreOrder(root);

  // root = deleteNode(root, 3);

  // printf("\nAfter deletion: ");
  // printPreOrder(root);
  // Value* value = search(root, 5);
  // printf("\nvalue : %d\n", *value);

  TreeMap* tree = TreeMap_create();
  TreeMap_insert(tree, 2, 200);
  TreeMap_insert(tree, 1, 100);
  TreeMap_insert(tree, 7, 700);
  TreeMap_insert(tree, 4, 400);
  TreeMap_insert(tree, 5, 500);
  TreeMap_insert(tree, 3, 300);
  TreeMap_insert(tree, 8, 800);
  TreeMap_insert(tree, 9, 900);
  TreeMap_insert(tree, 6, 600);
  TreeMap_insert(tree, 10, 1000);

  TreeMap_print(tree);
  Key* keys = TreeMap_keys(tree);
  printf("keys : ");
  for (int i = 0; i < TreeMap_count(tree); i++) {
    printf("%3d", keys[i]);
  }
  printf("\n");
  free(keys);

  Value* values = TreeMap_values(tree);
  printf("values : ");
  for (int i = 0; i < TreeMap_count(tree); i++) {
    printf("%d ", values[i]);
  }
  printf("\n");
  free(values);
  
  Iterator* iter = TreeMap_iterator(tree);
  while (iter != NULL) {
    printf("key = %d, value = %d\n", iter->key, iter->value);
    iter = Iterator_next(iter);
  }

  printf("----------\n");
  keys = TreeMap_keys(tree);
  qsort(keys, 0, TreeMap_count(tree) - 1, compare);
  for (int i = 0; i < TreeMap_count(tree); i++) {
    printf("key: %d, value: %d\n", keys[i], TreeMap_search(tree, keys[i]));
  }
  free(keys);
  printf("----------\n");

  TreeMap_delete(tree);

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