#include <stdio.h>
#include <map>
#include <vector>
#include <algorithm>

#define map_has(m, e) ((m).find((e)) != (m).end())
#define vector_has(v, e) (std::find((v).begin(), (v).end(), (e)) != (v).end())

#define debug printf("(%s,%d)", __FILE__, __LINE__); printf

class Pair {
  public:
    char a;
    char b;

  public:
    Pair(char a, char b) {
        this->a = a;
        this->b = b;
    }
};

class LevelNode {
  public:
    int level;
    char node;

  public:
    LevelNode(int level, char node) {
        this->level = level;
        this->node = node;
    }
};

class Graph {
  private:
    std::map<char, std::map<char, bool>*> adjacency_map; 
    
  public:  
    Graph() {
        
    }

    ~Graph() {
        auto iter = this->adjacency_map.begin();
        while (iter != this->adjacency_map.end()) {
            delete (*iter).second;
            iter++;
        }
    }
    
    void add_node(char node) {
        if (! map_has(this->adjacency_map, node)) {
            std::map<char, bool>* neighbors = new std::map<char, bool>();
            this->adjacency_map[node] = neighbors;
        }
    }
    
    void add_link(char a_node, char b_node) {
        // debug("-\n");
        this->add_node(a_node);
        // debug("-\n");
        std::map<char, bool>* neighbors = this->adjacency_map[a_node];
        // debug("%p\n", neighbors);
        (*neighbors)[b_node] = true;
        // debug("-\n");
    }

    void add_links(char a_node, std::vector<char>& b_nodes) {
        for (auto b_node : b_nodes) {
            // debug("-\n");
            this->add_link(a_node, b_node);
        }
    }
    
    void add_pairs(std::vector<Pair*>& pairs) {
        for (Pair* pair : pairs) {
            this->add_link(pair->a, pair->b);
        }
    }
    
    std::vector<std::vector<LevelNode*>*>* search_paths(char a_node, char b_node) {
        LevelNode* start = new LevelNode(0, a_node);
        std::vector<LevelNode*>* path = new std::vector<LevelNode*>(); 
        path->push_back(start);
        std::vector<std::vector<LevelNode*>*>* paths = new std::vector<std::vector<LevelNode*>*>();
        // stack = []
        std::vector<LevelNode*>* q = new std::vector<LevelNode*>();
        std::vector<char>* visited = new std::vector<char>();
        // stack.append([0, a_node])
        q->push_back(start);
        while (q->size() > 0) {  // (len(stack) > 0)
            // level_node = stack.pop()
            LevelNode* level_node = (*q)[0]; q->erase(q->begin()); // q.dequeue()
            if (vector_has(*visited, level_node->node)) continue;
            std::vector<LevelNode*>* new_path = new std::vector<LevelNode*>();
            for (LevelNode* p : *path) {
                if (p->level < level_node->level) {
                    new_path->push_back(p);
                }
            }
            delete path;
            path = new_path;
            path->push_back(level_node);
            visited->push_back(level_node->node); 
            if (! map_has(this->adjacency_map, level_node->node)) continue;
            std::map<char, bool>* neighbors = this->adjacency_map[level_node->node];
            for (auto iter = neighbors->begin(); iter != neighbors->end(); ++iter) {
                char neighbor = iter->first;
                if (neighbor == b_node) {
                    path->push_back(new LevelNode(level_node->level + 1, neighbor));
                    paths->push_back(path);
                    path = new std::vector<LevelNode*>();
                    path->push_back(new LevelNode(0, a_node));
                    continue;
                }
                //stack.append([level_node[0] + 1, neighbor])
                q->push_back(new LevelNode(level_node->level + 1, neighbor));
            }
        }
        // 어정쩡하게 끝난 path가 있으면 path 자체와 path 내부의 levelnode를 delete
        for (LevelNode* levelnode : *path) {
            delete levelnode;
        }
        delete path;
        for (char visit : *visited) {
            printf("%c ", visit);
        }
        printf("\n");
        // print(visited)
        delete q;
        delete visited;
        return paths;
    }

    void print() {
        for (auto iter_k = this->adjacency_map.begin(); iter_k != this->adjacency_map.end(); ++iter_k) {
            char key = iter_k->first;
            printf("%c: ", key);
            for (auto iter_v = this->adjacency_map[key]->begin(); iter_v != this->adjacency_map[key]->end(); ++iter_v) { 
                char value = iter_v->first;
                printf("%c ", value);
            }
            printf("\n");
        }
    }
};

int main() {
    Graph* graph = new Graph();
    // debug("-\n");
    std::vector<char>* neighbors = new std::vector<char>();
    neighbors->push_back('B'); 
    neighbors->push_back('C'); 
    neighbors->push_back('X'); 
    // debug("-\n");
    graph->add_links('A', *neighbors);
    // debug("-\n");
    // graph.add_link('A', 'B')
    // graph.add_link('A', 'C')
    // graph.add_link('A', 'X')
    delete neighbors;
    neighbors = new std::vector<char>();
    neighbors->push_back('D'); 
    neighbors->push_back('C'); 
    neighbors->push_back('Y');
    graph->add_links('B', *neighbors);
    // debug("-\n");
    delete neighbors;
    // graph.add_link('B', 'D')
    // graph.add_link('B', 'C')
    // graph.add_link('B', 'Y')
    neighbors = new std::vector<char>();
    neighbors->push_back('F'); 
    neighbors->push_back('Z'); 
    neighbors->push_back('W');    
    neighbors->push_back('D');    
    graph->add_links('C', *neighbors);
    // debug("-\n");
    delete neighbors;
    // graph.add_link('C', 'F')
    // graph.add_link('C', 'Z')
    // graph.add_link('C', 'W')
    // graph.add_link('C', 'D')
    neighbors = new std::vector<char>();
    neighbors->push_back('D'); 
    neighbors->push_back('Z'); 
    graph->add_links('D', *neighbors);
    // debug("-\n");
    delete neighbors;    
    // graph.add_link('D', 'D')
    // graph.add_link('D', 'Z')
    std::vector<Pair*>* pairs = new std::vector<Pair*>(); 
    // debug("-\n");
    pairs->push_back(new Pair('A', 'B'));
    pairs->push_back(new Pair('A', 'C'));
    pairs->push_back(new Pair('A', 'X'));
    pairs->push_back(new Pair('B', 'D'));
    pairs->push_back(new Pair('B', 'C'));
    pairs->push_back(new Pair('B', 'Y'));
    pairs->push_back(new Pair('C', 'F'));
    pairs->push_back(new Pair('C', 'Z'));
    pairs->push_back(new Pair('C', 'W'));
    pairs->push_back(new Pair('C', 'D'));
    pairs->push_back(new Pair('D', 'D'));
    pairs->push_back(new Pair('D', 'Z'));
    // debug("-\n");
    // [['A', 'B'], ['A', 'C'], ['A', 'X'],
    //              ['B', 'D'], ['B', 'C'], ['B', 'Y'],
    //              ['C', 'F'], ['C', 'Z'], ['C', 'W'], ['C', 'D'],
    //              ['D', 'D'], ['D', 'Z']];
    graph->add_pairs(*pairs);
    for (Pair* pair : *pairs) {
        delete pair;
    }
    delete pairs;
    // debug("-\n");
    graph->print();
    // debug("-\n");
    std::vector<std::vector<LevelNode*>*>* paths = graph->search_paths('A', 'D');
    // debug("-\n");
    for (std::vector<LevelNode*>* path : *paths) {
        printf("[");
        for (int i = 0; i < path->size(); i++) {
            printf("(%d, %c) ", (*path)[i]->level, (*path)[i]->node);
        }
        printf("]\n");
    }
    
    for (std::vector<LevelNode*>* path : *paths) {
        for (LevelNode* levelnode : *path) {
            delete levelnode;
        }
        delete path;
    }
    delete paths;
    delete graph;
    return 0;
}