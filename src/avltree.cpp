// AVL tree implementation in C
// https://www.programiz.com/dsa/avl-tree

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <vector>

using namespace std;

// #define debug printf("(%s,%d)", __FILE__, __LINE__); printf
#define debug if (false) printf

typedef int Key;
typedef int Value;
typedef int Element;

class Node {
  public:  
    Key key;
    Value value;
    Node* left;
    Node* right;
    int height;

  public:
    Node (Key key, Value value) {
        this->key = key;
        this->value = value;
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
    }

    // this node 하위 트리 전체를 지운다
    static void deleteAll(Node* node) {
        if (node == NULL) {
            return;
        }
        if (node->left != NULL) {
            Node::deleteAll(node->left);
        }
        if (node->right != NULL) {
            Node::deleteAll(node->right);
        }
    }

    // Calculate height
    static int get_height(Node* node) {
        if (node == NULL)
            return 0;
        return node->height;
    }

    static inline int Max(int a, int b) {
        return (a > b) ? a : b;
    }

    // Right rotate
    static Node* rightRotate(Node* node) {
        assert(node->left != NULL);
        Node* x = node->left;
        Node* T2 = x->right;

        x->right = node;
        node->left = T2;

        node->height = Node::Max(Node::get_height(node->left), Node::get_height(node->right)) + 1;
        x->height = Node::Max(Node::get_height(x->left), Node::get_height(x->right)) + 1;

        return x; // 상위 node에서 새로운 subtree의 최상위 node를 연결하도록 리턴
    }

    // Left rotate
    static Node* leftRotate(Node* node) {
        Node* y = node->right;
        Node* T2 = y->left;

        y->left = node;
        node->right = T2;

        node->height = Node::Max(Node::get_height(node->left), Node::get_height(node->right)) + 1;
        y->height = Node::Max(Node::get_height(y->left), Node::get_height(y->right)) + 1;

        return y;
    }

    // Get the balance factor
    static int getBalance(Node* node) {
        if (node == NULL)
            return 0;
        return Node::get_height(node->left) - Node::get_height(node->right);
    }

    // 균형을 맞추고 결과적으로 최상위 node를 리턴, 균형이 맞으면 NULL을 리턴
    static Node* rebalance(Node* node, Key key) {
        int balance = Node::getBalance(node); // balance factor가 -1, 0, 1이면 균형 잡힘, 절댓값이 2 이상이면 불균형  
        if (balance > 1) { // 왼쪽으로 기울어 있으면
            if (key < node->left->key) { // 왼쪽 사선으로 정렬되어 있으면, 오른쪽으로 회전
                return Node::rightRotate(node); 
            }
            if (key > node->left->key) { // 왼쪽으로 삐뚤게 되어 있으면
                node->left = Node::leftRotate(node->left); // 왼쪽 사선으로 정렬시킨 후 오른쪽으로 회전
                return Node::rightRotate(node);      
            }
        }
        if (balance < -1) { // 오른쪽으로 기울어 있으면
            if (key > node->right->key) { // 오른쪽 사선으로 정렬되어 있으면, 왼쪽으로 회전
                return Node::leftRotate(node);
            }
            if (key < node->right->key) { // 오른쪽으로 삐뚤게 되어 있으면
                node->right = Node::rightRotate(node->right); // 오른쪽 사선으로 정렬시킨 후 왼쪽으로 회전
                return Node::leftRotate(node);
            }
        }
        return NULL; // 좌우 균형이 맞으면
    }

    static Node* minValueNode(Node* node) {
        Node* current = node;

        while (current->left != NULL)
            current = current->left;

        return current;
    }

    // 특정한 node를 기준으로 하위 tree에서 key를 찾아서 node를 지우고 그 결과 최상위 node를 리턴한다
    static Node* deleteNode(Node* node, Key key) {
        // Find the node and delete it
        if (node == NULL) // This가 NULL이면 지우고 할 것 없이 리턴
            return node;

        if (key < node->key)
            node->left = Node::deleteNode(node->left, key); // 왼쪽에서 키를 지우고 결과로 생긴 최상위 node를 This->left에 연결
        else if (key > node->key)
            node->right = Node::deleteNode(node->right, key);
        else { // key가 같으면 This를 지운다
            if ((node->left == NULL) || (node->right == NULL)) { // 좌우 자식 node 둘 중에 하나라도 NULL이면
                Node* temp = (node->left != NULL) ? node->left : node->right; 
                if (temp == NULL) { // 둘 다 NULL이면
                    delete node;        
                    return NULL;
                } else { // 좌우 sub tree가 하나(temp) 존재하면, 그것을 한 level 상승시킴
                    *node = *temp; // 살아있는 놈(temp)을 한 단계 끌어올린다
                    delete temp;
                }
            } else { // 좌우 자식 node 둘 다 있으면
                // 오른쪽에서 제일 작은 놈을 찾는다
                Node* temp = Node::minValueNode(node->right);
                // 그놈을 최상위로 끌어올린다
                node->key = temp->key; 
                node->value = temp->value;
                // 그놈을 오른쪽에서 지운 후 그 결과 최상위 node를 This 오른쪽에 붙인다
                node->right = Node::deleteNode(node->right, temp->key);
            }
        }

        // Update the balance factor of each node and
        // balance the tree
        node->height = 1 + Node::Max(Node::get_height(node->left), Node::get_height(node->right));

        // Rebalance
        Node* balanced = Node::rebalance(node, key);
        if (balanced != NULL) return balanced; // rebalance한 최상위 node를 리턴한다

        return node; // 지웠는데도 균형이 맞으면 This를 리턴한다
    }

        // Print the tree
    static void printPreOrder(Node* node) {
        if (node != NULL) {
            printf("%d:%d ", node->key, node->value);
            Node::printPreOrder(node->left);
            Node::printPreOrder(node->right);
        }
    }

    // this node 하위에서 key를 찾아서 그 value를 리턴하는데, 없으면 NULL을 리턴한다 
    static Value searchNode(Node* node, Key key) {
        if (node == NULL) { // leaf node면 NULL 리턴 
            return -999;
        }
        if (node->key == key) {
            return node->value;
        } else if (node->key < key) {
            // 오른쪽에서 찾은 value를 리턴한다
            return Node::searchNode(node->right, key);
        } else {
            return Node::searchNode(node->left, key);
        }
    }

    static void printNode(Node* node, int level, int flag) {
        for (int i = 0; i < level; i++) {
            if (i == (level - 1) && flag == 1) { // 오른쪽일 때 +를 표시
                printf("+ ");
            } else {
                printf("| ");
            }
        }
        printf("(level = %d, key = %d, value = %d)\n", level, node->key, node->value);
        if (node->left != NULL) {
            Node::printNode(node->left, level + 1, 0);
        }
        if (node->right != NULL) {
            Node::printNode(node->right, level + 1, 1);
        }
    }

    static void fill_keys(Node* node, vector<Key>* keys) {
        keys->push_back(node->key);
        if (node->left != NULL) {
            Node::fill_keys(node->left, keys);
        }
        if (node->right != NULL) {
            Node::fill_keys(node->right, keys);
        }
    }

    static void fill_values(Node* node, vector<Value>* values) {
        values->push_back(node->value);
        if (node->left != NULL) {
            Node::fill_values(node->left, values);
        }
        if (node->right != NULL) {
            Node::fill_values(node->right, values);
        }
    }

    static int countNode(Node* node) {
        if (node == NULL) {
            return 0;
        }
        return Node::countNode(node->left) + Node::countNode(node->right) + 1;
    }

    // This node 하위 tree에 key와 value를 insert하고, 결과적인 최상위 node를 리턴한다
    static Node* insertNode(Node* node, Key key, Value value) {
        // Find the correct position to insertNode the node and insertNode it
        if (node == NULL) // leaf node에 도달하면 새로 만들어 붙인다
            return (new Node(key, value));

        if (key < node->key)
            node->left = Node::insertNode(node->left, key, value); // 좌측에 insert하고 최상위 node를 받아서 left에 붙인다
        else if (key > node->key)
            node->right = Node::insertNode(node->right, key, value); // 우측에 insert하고 최상위 node를 받아서 right에 붙인다
        else { // key가 같으면 value를 덮어쓴다
            node->value = value;
            return node; // 최상위 node 변경 없음
        }

        // Update the balance factor of each node and
        // Balance the tree
        node->height = 1 + Node::Max(Node::get_height(node->left), Node::get_height(node->right));

        // Rebalance
        Node* balanced = Node::rebalance(node, key);
        if (balanced != NULL) return balanced; // balance 결과 변경된 최상위 node를 리턴

        return node; // inert는 했고, 최상위 node 변경은 없음
    }
};

class Stack {
  private:
    vector<Node*> array;

  public:
    Stack() {
    
    }

    void push(Node* node) {
        this->array.push_back(node);
    }

    Node* pop() {
        Node* node = this->array[this->array.size() - 1];
        this->array.pop_back();
        return node;
    }

    int get_count() {
        return this->array.size();
    }
};

class Iterator {
  public:
    Key key;
    Value value;
    Stack* stack;
    Node* current;

  public:
    Iterator(Node* node) {
        this->key = 0;
        this->value = 0;
        this->stack = new Stack();
        this->current = node;
        debug("-\n");
    }

    Iterator* next() {
        while (this->current != NULL || this->stack->get_count() != 0) {
            debug("-\n");    
            if (this->current != NULL) {
                this->stack->push(this->current);
                this->current = this->current->left;
                debug("-\n");
            } else {
                Node* node = this->stack->pop();
                this->value = node->value;
                this->key = node->key;
                this->current = node->right;
                debug("-\n");
                return this;
            }
        }
        debug("-\n");
        return NULL;
    }

    ~Iterator() {
        delete this->stack;
    }
};

class TreeMap {
  public:
    Node* root;
    int count;
    Iterator* iter;

  public:
    TreeMap() {
        this->root = NULL;
        this->count = 0;
        this->iter = NULL;
    }

    int get_count() {
        // return Node_count(this->root);
        return this->count;
    }
    
    vector<Key>* keys() {
        if (this->root == NULL) {
            return NULL;
        }
        vector<Key>* keys = new vector<Key>;
        Node::fill_keys(this->root, keys);
        debug("%d\n", keys->size());
        return keys;
    }
    
    vector<Value>* values() {
        if (this->root == NULL) {
            return NULL;
        }
        vector<Value>* values = new vector<Value>;
        Node::fill_values(this->root, values);
        return values;
    }

    // this TreeMap에서 key를 찾아서 상응하는 value*을 리턴하고, 없으면 NULL을 리턴한다
    Value search(Key key) {
        return Node::searchNode(this->root, key);
    }

    bool contains(Key key) {
        if (this->search(key) == -999) {
            return false;
        } else {
            return true;
        }
    }

    void insert(Key key, Value value) {
        if (! this->contains(key)) {
            this->count += 1;
        }
        // insert 하고 rebalancing 과정에서 root node의 변경이 일어날 수 있으므로 새로운 root를 기억
        this->root = Node::insertNode(this->root, key, value);
    }

    void remove_item(Key key) {
        if (this->contains(key)) {
            this->count -= 1;
        }   
        this->root = Node::deleteNode(this->root, key);
    }

    void print() {
        if (this->root != NULL) {
            Node::printNode(this->root, 0, 0);
        }
    }

    Iterator* iterator() {
        if (this->root == NULL) {
            debug("-\n");
            return NULL;
        }
        debug("-\n");
        Iterator* iter = new Iterator(this->root);
        this->iter = iter;
        debug("-\n");
        return iter;
    }

    ~TreeMap() {  
        Node::deleteAll(this->root);
        delete this->iter;
    }
};

class Set: public TreeMap{  
  public:  
    Set() {}
    
    ~Set() {}

    void add(Element element) {
        this->insert(element, 1);
    }

    bool has(Element element) {
        return this->contains(element);
    }
    
    Set* intersection(Set* set2) {
        Set* set3 = new Set();
        Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            if (set2->has(iter->key)) {
                set3->add(iter->key);
            }
        }
        return set3;
    }

    Set* union_set(Set* set2) {
        Set* set3 = new Set();
        Iterator* iter = this->iterator();
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
        Iterator* iter = this->iterator();
        while (iter->next() != NULL) {
            if (! set2->has(iter->key)) {
                set3->add(iter->key);
            }
        }
        return set3;
    }

    Set* symmetric_difference(Set* set2) {
        Set* set3 = new Set();
        Iterator* iter = this->iterator();
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
        printf("{");
        Iterator* iter = this->iterator();
        for (int i = 0; iter->next() != NULL; i++) {
            if (i == this->get_count() - 1) {
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
    Node* root = NULL;
    root = Node::insertNode(root, 2, 200);
    root = Node::insertNode(root, 1, 100);
    root = Node::insertNode(root, 7, 700);
    root = Node::insertNode(root, 4, 400);
    root = Node::insertNode(root, 5, 500);
    root = Node::insertNode(root, 3, 300);
    root = Node::insertNode(root, 8, 800);

    Node::printPreOrder(root);

    root = Node::deleteNode(root, 3);

    printf("\nAfter deletion: ");
    Node::printPreOrder(root);
    Value value = Node::searchNode(root, 5);
    printf("\nvalue : %d\n", value);

    TreeMap* tree = new TreeMap();
    tree->insert(2, 200);
    tree->insert(1, 100);
    tree->insert(7, 700);
    tree->insert(4, 400);
    tree->insert(5, 500);
    tree->insert(3, 300);
    tree->insert(8, 800);
    tree->insert(9, 900);
    tree->insert(6, 600);
    tree->insert(10, 1000);

    tree->print();
    vector<Key>* keys = tree->keys();
    printf("keys : ");
    debug("%d, %d\n", keys->size(), tree->get_count());
    for (int i = 0; i < tree->get_count(); i++) {
        printf("%3d", (*keys)[i]);
    }
    printf("\n");
    delete keys;

    vector<Value>* values = tree->values();
    printf("values : ");
    for (int i = 0; i < tree->get_count(); i++) {
        printf("%d ", (*values)[i]);
    }
    printf("\n");
    delete values;
    
    Iterator* iter = tree->iterator();
    while (iter->next() != NULL) {
        printf("key = %d, value = %d\n", iter->key, iter->value);
    }
    
    printf("----------\n");
    keys = tree->keys();
    qsort(*keys, 0, keys->size() - 1, compare);
    for (auto key : (*keys)) {
        printf("key: %d, value: %d\n", key, tree->search(key));
    }
    printf("----------\n");

    delete tree;

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