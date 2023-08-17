#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
// tree == Directed Acyclic Graph
#define debug printf("(%s,%d)", __FILE__, __LINE__); printf

typedef uint32_t Key;
typedef uint64_t Data;
// typedef struct Data {
//     int age;
//     char* name;
// } Data;
typedef struct Node {
    Key key;
    Data* value;
    Node* left;
    Node* right;
} Node;

Node* Node_create(Key key, Data* value) {
    Node* This = (Node*) malloc(sizeof(Node));
    This->key = key;
    This->value = value;
    This->left = NULL;
    This->right = NULL;
    return This;
}

void Node_delete(Node* This) { // A B
    free(This->value);
    if (This->left != NULL) Node_delete(This->left); // B BB
    if (This->right != NULL) Node_delete(This->right); // C BC
    free(This);
}

void Node_print(Node* This) {
    printf("(%d:%d)\n", This->key, This->value);
}

void Node_insert_item(Node* This, Key key, Data* value) {
    if (This->key == key) {
        free(This->value);
        This->value = value;
    } else if (This->key > key) {
        if (This->left == NULL) {
            This->left = Node_create(key, value);
        } else {
            Node_insert(This->left, key, value);
        }
    } else {
        if (This->right == NULL) {
            This->right = Node_create(key, value);
        } else {
            Node_insert(This->right, key, value);
        }
    }
}

void Node_remove_item(Node* This, Key key) {
    if (This->key == key) {
        if (This->left == NULL) {
            free(This->left);
            else {

            }
        } else {
            if (This->right)
        }
            

    }
}

typedef struct Tree {
    Node* root;
} Tree;

Tree* Tree_create() {
    Tree* This = (Tree*) malloc(sizeof(Tree));
    This->root = NULL;
    return This;
}

void Tree_delete(Tree* This) {
    if (This->root != NULL) Node_delete(This->root);
    free(This);
}

void Tree_insert(Tree* This, Key key, Data* value) {
    Node* node = Node_create(key, value);
    if (This->root == NULL) This->root = node;
    else {

    }
}