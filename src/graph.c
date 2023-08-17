#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"
#include "map.h"

// #define debug printf("(%s, %d)", __FILE__, __LINE__); printf
#define debug if (false) printf

typedef struct Graph {
    Map* adjacency_map;
} Graph;

typedef struct Pair {
    char a;
    char b;
} Pair;

Pair* Pair_create(char a, char b) {
    Pair* This = (Pair*) malloc(sizeof(Pair));
    This->a = a;
    This->b = b;
    return This;
}

typedef struct LevelNode {
    int level;
    char node;
} LevelNode;

LevelNode* LevelNode_create(int level, char node) {
    LevelNode* This = (LevelNode*) malloc(sizeof(LevelNode));
    This->level = level;
    This->node = node;
    return This;
}

Graph* Graph_create() {
    Graph* This = (Graph*) malloc(sizeof(Graph));
    This->adjacency_map = M_CREATE(sizeof(char), "c", sizeof(Map*), "p");
    return This;
}

void Graph_destroy(Graph* This) {
    Iterator* iter = M_ITER(This->adjacency_map);
    while (I_NEXT(iter) != NULL) {
        M_DESTROY(iter->p_value);
    }
    M_DESTROY(This->adjacency_map);
}

void Graph_add_node(Graph* This, char node) {
    debug("Graph_add_node(%c)\n", node);
    if (! M_HAS(This->adjacency_map, node)) {
        debug("Map does not have %c\n", node);
        Map* neighbors = M_CREATE(sizeof(char), "c", sizeof(bool), "b");
        M_PUT(This->adjacency_map, node, neighbors);
    }
}

void Graph_add_link(Graph* This, char a_node, char b_node) {
    debug("Graph_add_link()\n");
    debug("a_node=%c, b_node=%c\n", a_node, b_node);
    Graph_add_node(This, a_node);
    Map* neighbors; M_RETR(This->adjacency_map, a_node, &neighbors);
    bool True = true;
    M_PUT(neighbors, b_node, True);
}

void Graph_add_links(Graph* This, char a_node, List* b_nodes) {
    for (size_t i = 0; i < L_COUNT(b_nodes); i++) {
        char b_node; L_RETR(b_nodes, i, &b_node);
        Graph_add_link(This, a_node, b_node);
    }
}

void Graph_add_pairs(Graph* This, List* pairs) {
    for (size_t i = 0; i < L_COUNT(pairs); i++) {
        Pair* pair; L_RETR(pairs, i, &pair);
        Graph_add_link(This, pair->a, pair->b);
    }
}

List* Graph_search_paths(Graph* This, char a_node, char b_node) {
    debug("Graph_search_paths()\n");
    List* path = L_CREATE(sizeof(LevelNode*), "p");
    LevelNode* start = LevelNode_create(0, a_node);
    debug("Graph_search_paths() L_ADD(path, start);\n");
    L_ADD(path, start);
    List* paths = L_CREATE(sizeof(List*), "p");
    // stack = []
    List* q = L_CREATE(sizeof(LevelNode*), "p");
    List* visited = L_CREATE(sizeof(char), "c");
    // stack.append([0, a_node])
    debug("Graph_search_paths() L_ADD(q, start);\n");
    L_ADD(q, start);
   //q.append([0, a_node])
    while (L_COUNT(q) > 0) { // (len(stack) > 0)
        // level_node = stack.pop()
        LevelNode* level_node; L_REMOVE(q, 0, &level_node); // q.dequeue()
        debug("Graph_search_paths() q.dequeue node=%c\n", level_node->node);
        if (L_HAS(visited, level_node->node)) {
            debug("Graph_search_paths() already visited %c\n", level_node->node);
            continue;
        }
        List* new_path = L_CREATE(sizeof(LevelNode*), "p");
        for (size_t i = 0; i < L_COUNT(path); i++) {
            LevelNode* p; L_RETR(path, i, &p);
            if (p->level < level_node->level) {
                L_ADD(new_path, p);
            }
        }
        L_DESTROY(path);
        path = new_path;
        L_ADD(path, level_node);
        L_ADD(visited, level_node->node);
        if (! M_HAS(This->adjacency_map, level_node->node)) {
            debug("Graph_search_paths() has no node %c\n", level_node->node);
            continue;
        }    
        Map* neighbors; M_RETR(This->adjacency_map, level_node->node, &neighbors);
        Iterator* iter = M_ITER(neighbors);
        debug("Graph_search_paths() iterator neighbors %c\n", level_node->node);
        while (I_NEXT(iter) != NULL) {
            char neighbor = *((char*) (iter->p_key));
            debug("Graph_search_paths() neighbor %c\n", neighbor);
            if (neighbor == b_node) {
                debug("Graph_search_paths() destination %c\n", neighbor);
                LevelNode* x = LevelNode_create(level_node->level + 1, neighbor);
                L_ADD(path, x);
                L_ADD(paths, path);
                path = L_CREATE(sizeof(LevelNode*), "p");
                LevelNode* y = LevelNode_create(0, a_node); 
                L_ADD(path, y);
                continue;
            }
            //stack.append([level_node[0] + 1, neighbor])
            LevelNode* z = LevelNode_create(level_node->level + 1, neighbor);
            debug("Graph_search_paths() L_ADD(q, z); %c\n", z->node);
            L_ADD(q, z);
        }
    }
    for (size_t i = 0; i < L_COUNT(visited); i++) {
        char cur_node; L_RETR(visited, i, &cur_node);
        printf("%c ", cur_node);
    }
    printf("\n");
    for (size_t i = 0; i < L_COUNT(path); i++) {
        LevelNode* cur_levelnode; L_RETR(path, i, &cur_levelnode);
        free(cur_levelnode);
    }
    L_DESTROY(path);
    for (size_t i = 0; i < L_COUNT(q); i++) {
        LevelNode* cur_levelnode; L_RETR(q, i, &cur_levelnode);
        free(cur_levelnode);
    }
    L_DESTROY(q);
    L_DESTROY(visited);
    return paths;
}

void Graph_print(Graph* This) {
    // key in self.adjacency_map.keys()
    List* keys = M_KEYS(This->adjacency_map);
    for (size_t i = 0; i < L_COUNT(keys); i++) {
        char key; L_RETR(keys, i, &key);
        printf("%c: ", key);
        Map* neighbors; M_RETR(This->adjacency_map, key, &neighbors);
        List* values = M_KEYS(neighbors);
        for (size_t i = 0; i < L_COUNT(values); i++) {
            char value; L_RETR(values, i, &value);
            printf("%c ", value);
        }
        printf("\n");
    }
}

int main() {
    Graph* graph = Graph_create();
    List* neighbors = L_CREATE(sizeof(char), "c");
    char b = 'B'; char c = 'C'; char x = 'X';
    L_ADD(neighbors, b);
    L_ADD(neighbors, c);
    L_ADD(neighbors, x);
    Graph_add_links(graph, 'A', neighbors);
    L_DESTROY(neighbors);
    // graph.add_link('A', 'B')
    // graph.add_link('A', 'C')
    // graph.add_link('A', 'X')
    neighbors = L_CREATE(sizeof(char), "c");
    char d = 'D'; char cc = 'C'; char y = 'Y';
    L_ADD(neighbors, d);
    L_ADD(neighbors, cc);
    L_ADD(neighbors, y);    
    Graph_add_links(graph, 'B', neighbors);
    L_DESTROY(neighbors);
    // graph.add_link('B', 'D')
    // graph.add_link('B', 'C')
    // graph.add_link('B', 'Y')
    neighbors = L_CREATE(sizeof(char), "c");
    char f = 'F'; char z = 'Z'; char w = 'W'; char dd = 'D';
    L_ADD(neighbors, f);
    L_ADD(neighbors, z);
    L_ADD(neighbors, w);    
    L_ADD(neighbors, dd);    
    Graph_add_links(graph, 'B', neighbors);
    L_DESTROY(neighbors);
    // graph.add_link('C', 'F')
    // graph.add_link('C', 'Z')
    // graph.add_link('C', 'W')
    // graph.add_link('C', 'D')
    neighbors = L_CREATE(sizeof(char), "c");
    char ddd = 'D'; char zz = 'Z';
    L_ADD(neighbors, ddd);
    L_ADD(neighbors, zz);
    L_DESTROY(neighbors);
    // graph.add_link('D', 'D')
    // graph.add_link('D', 'Z')
    List* pairs = L_CREATE(sizeof(Pair*), "p");
    Pair* pa = Pair_create('A', 'B');
    Pair* pb = Pair_create('A', 'C');
    Pair* pc = Pair_create('A', 'X');
    Pair* pd = Pair_create('B', 'D');
    Pair* pe = Pair_create('B', 'C');
    Pair* pf = Pair_create('B', 'Y');
    Pair* pg = Pair_create('C', 'F');
    Pair* ph = Pair_create('C', 'Z');
    Pair* pi = Pair_create('C', 'W');
    Pair* pj = Pair_create('C', 'D');
    Pair* pk = Pair_create('D', 'D');
    Pair* pl = Pair_create('D', 'Z');
    
    L_ADD(pairs, pa);
    L_ADD(pairs, pb);
    L_ADD(pairs, pc);
    L_ADD(pairs, pd);
    L_ADD(pairs, pe);
    L_ADD(pairs, pf);
    L_ADD(pairs, pg);
    L_ADD(pairs, ph);
    L_ADD(pairs, pi);
    L_ADD(pairs, pj);
    L_ADD(pairs, pk);
    L_ADD(pairs, pl);
    Graph_add_pairs(graph, pairs);
    L_DESTROY(pairs);
    Graph_print(graph);
    List* paths = Graph_search_paths(graph, 'A', 'D');
    // path in paths
    for (size_t i = 0; i < L_COUNT(paths); i++) {
        List* path; L_RETR(paths, i, &path);
        printf("[");
        for (size_t j = 0; j < L_COUNT(path); j++) {
            LevelNode* cur_levelnode; L_RETR(path, j, &cur_levelnode);
            printf("(%d, %c) ", cur_levelnode->level, cur_levelnode->node);
        }
        printf("]\n");
    }
    for (size_t i = 0; i < L_COUNT(paths); i++) {
        List* path; L_RETR(paths, i, &path);
        for (size_t j = 0; j < L_COUNT(path); j++) {
            LevelNode* cur_levelnode; L_RETR(path, j, &cur_levelnode);
            free(cur_levelnode);
        }
        L_DESTROY(path);
    }
    L_DESTROY(paths);
    Graph_destroy(graph);
}