#include <stdio.h>
#include <stdlib.h>

// 그래프의 최대 노드 개수
#define MAX_NODES 100

// 인접 리스트를 위한 구조체 정의
struct Node {
    int data;
    struct Node* next;
};

// 그래프를 나타내는 구조체 정의
struct Graph {
    int numNodes;
    struct Node* adjLists[MAX_NODES];
};

// 새로운 노드 생성
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 그래프 초기화
struct Graph* createGraph(int numNodes) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; ++i)
        graph->adjLists[i] = NULL;
    return graph;
}

// 간선 추가
void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// DFS 탐색 함수
void DFS(struct Graph* graph, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);

    struct Node* adjList = graph->adjLists[node];
    while (adjList) {
        int neighbor = adjList->data;
        if (!visited[neighbor])
            DFS(graph, neighbor, visited);
        adjList = adjList->next;
    }
}

// BFS 탐색 함수
void BFS(struct Graph* graph, int startNode) {
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;

    visited[startNode] = 1;
    queue[rear++] = startNode; // enqueue

    while (front < rear) {
        int node = queue[front++]; // dequeue
        visited[node] = 1; // 방문함
        printf("%d ", node);
        struct Node* adjList = graph->adjLists[node];
        while (adjList) {
            int neighbor = adjList->data;
            if (!visited[neighbor]) {
                queue[rear++] = neighbor; // enqueue
            }
            adjList = adjList->next;
        }
    }
}

int main() {
    int numNodes = 6; // 노드 개수 설정
    struct Graph* graph = createGraph(numNodes);

    // 간선 추가
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 5);

    printf("DFS 탐색 결과: ");
    int visitedDFS[MAX_NODES] = {0};
    DFS(graph, 0, visitedDFS);

    printf("\nBFS 탐색 결과: ");
    BFS(graph, 0);

    return 0;
}