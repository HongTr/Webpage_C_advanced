#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#define INFINITIVE_VALUE 1000
typedef struct Graph{
    JRB edges;
    JRB vertices;
} Graph;
struct Queue{
    int info;
    float priority;
    struct Queue* link;
};
typedef struct Queue* Queue;
typedef struct index{
    int* pre;
    int* post;
}index;
//Graph
Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge1(Graph graph, int v1, int v2, float trongso);
void addEdge2(Graph graph, int v1, int v2, float trongso);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* input);
int outdegree(Graph graph, int v, int* output);
int getAdjacentVertices (Graph graph, int v, int* adj);
int NumberVertex(Graph graph);
int DAG(Graph graph);
float getEdgeValue(Graph graph, int v1, int v2);
void dropGraph(Graph graph);

float dijkstra(Graph graph, int start, int stop, int* path, int* length);
void DFS(Graph graph, int start, int stop);
void BFS(Graph graph, int v1, int v2);
int weak_connect(Graph graph, int v1, int v2);
//P_Queue
Queue createQueue();
int IsQueueEmpty(Queue queue);
Queue Enqueue(Queue queue, int item, float priority);
Queue Dequeue(Queue queue, int* output);

// for test
void addVertex_int(Graph graph, int id, int name);
int getVertex_int(Graph graph, int id);
