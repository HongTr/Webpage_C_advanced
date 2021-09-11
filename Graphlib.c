#include "Graphlib.h"

Graph createGraph(){
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}

void addVertex(Graph graph, int id, char* name){    
    JRB check = jrb_find_int(graph.vertices, id);
    if (check == NULL) 
        jrb_insert_int(graph.vertices, id, new_jval_s(name));
    else
        printf("Vertex already exists!\n");
}

char *getVertex(Graph graph, int id){
    JRB search = jrb_find_int(graph.vertices, id);
    if(search == NULL){
        //printf("Not found.\n");
        return NULL;
    }
    else return jval_s(search->val);
}

void addEdge1(Graph graph, int v1, int v2, float trong_so){ //add edge in directed graph
    JRB node = jrb_find_int(graph.edges, v1);
    if (node == NULL){
        JRB list = make_jrb();
        jrb_insert_int(graph.edges, v1, new_jval_v(list)); 
        jrb_insert_int(list, v2, new_jval_f(trong_so));
    }
    else{
        JRB list = (JRB)jval_v(node->val);
        jrb_insert_int(list, v2, new_jval_f(trong_so));
    }
}

void addEdge2(Graph graph, int v1, int v2, float trong_so){ //add edge in undirected graph
    JRB search = jrb_find_int(graph.edges, v1);
    JRB tree, node;
    if(search == NULL)
    {
        tree = make_jrb();
        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_f(trong_so));
    }
    else
    {
        node = jrb_find_int(graph.edges, v1);
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_f(trong_so));
    }

    JRB search1 = jrb_find_int(graph.edges, v2);
    JRB tree1, node1;
    if(search1 == NULL)
    {
        tree1 = make_jrb();
        jrb_insert_int(graph.edges, v2, new_jval_v(tree1));
        jrb_insert_int(tree1, v1, new_jval_f(trong_so));
    }
    else
    {
        node1 = jrb_find_int(graph.edges, v2);
        tree1 = (JRB) jval_v(node1->val);
        jrb_insert_int(tree1, v1, new_jval_f(trong_so));
    }
}

int hasEdge(Graph graph, int v1, int v2){
    JRB check = jrb_find_int(graph.edges, v1);
    if (check == NULL) return 0;
    if (jrb_find_int((JRB)jval_v(check->val), v2) == NULL) return 0;
    return 1;
}

int indegree(Graph graph, int v, int* output){ //Check adjacent vertices list in Undirected Graph
    int total = 0; 
    JRB temp, node, search;
    jrb_traverse(temp, graph.edges){
        node = (JRB) jval_v(temp->val);
        search = jrb_find_int(node, v); //check danh sach ke xem co v khong
        if(search != NULL) output[total++] = jval_i(temp->key);
    }
    return total;
}

int outdegree(Graph graph, int v, int* output){
    int total = 0; 
    JRB node = jrb_find_int(graph.edges, v);
    if(node == NULL) return 0;
    JRB list = (JRB) jval_v(node->val);
    jrb_traverse(node, list){
        output[total++] = jval_i(node->key);
    } 
    return total;
}

int getAdjacentVertices(Graph graph, int v, int* output){ //Duplicate "output" when dealing with Undirected Graph
    int temp[100], t, s;
    s = indegree(graph, v, output);
    t = outdegree(graph, v, temp);
    for (int i = 0; i < t; i++) output[s+i] = temp[i];
    return s + t;
}

int NumberVertex(Graph graph){
    int stop = 0;
    JRB trav;
    jrb_traverse(trav, graph.vertices) stop++;
    return stop;
}

float getEdgeValue(Graph graph, int v1, int v2){
    JRB node = jrb_find_int(graph.edges, v1);
    JRB list = (JRB) jval_v(node->val);
    JRB search = jrb_find_int(list, v2);
    if(search == NULL) return INFINITIVE_VALUE;
    else return jval_f(search->val);
}

void dropGraph(Graph graph){
    JRB node;
    jrb_traverse(node, graph.edges){
        jrb_free_tree(node);
    }
    jrb_traverse(node, graph.vertices){
        jrb_free_tree(node);
    }
}

//Priority Queue stuffs
Queue createQueue(){
    Queue queue = NULL;
    return queue;
}
int IsQueueEmpty(Queue queue){
    return queue==NULL;
}
Queue Enqueue(Queue queue, int item, float priority){
	Queue temp, q;
	temp = (Queue)malloc(sizeof(struct Queue));
	temp->info = item;
	temp->priority = priority;

	if( queue == NULL || priority < queue->priority )//< to >
	{
		temp->link = queue;
		queue = temp;
	}
	else
	{
		q = queue;
		while( q->link != NULL && q->link->priority <= priority ) q=q->link;
		temp->link = q->link;
		q->link = temp;
	}

    return queue;
}
Queue Dequeue(Queue queue, int* output){
	Queue temp;
	if(queue == NULL) printf("Queue Underflow\n");
	else
	{
		temp = queue;
		*output = temp->info;
		queue = queue->link;
		free(temp);
	}

    return queue;
}

//Algorithms
void DFS(Graph graph, int v1, int v2){
    int* visited = (int*)calloc(100, sizeof(int));
    Dllist q = new_dllist();
    dll_append(q, new_jval_i(v1));
    while(!dll_empty(q)){
        Dllist a =  dll_last(q);
        int x = jval_i(a->val);
        dll_delete_node(a);
        
        if (!visited[x] && x != v2){              
            printf("%d ", x);   //Co the thay bang lenh khac            
            visited[x] = 1;             
            int* list_adj = (int*)malloc(100*sizeof(int));              
            int i = getAdjacentVertices(graph, x, list_adj);
            for (int j = 0; j < i; j++){
                if (!visited[list_adj[j]]) dll_append(q, new_jval_i(list_adj[j]));   
            }
        }
        else if (x == v2){
            printf("%d ", x);
            break;
        }
    }
}

void BFS(Graph graph, int v1, int v2){
    int* visited = (int*)calloc(100, sizeof(int));
    Dllist q = new_dllist();
    dll_append(q, new_jval_i(v1));
    while(!dll_empty(q)){
        Dllist a =  dll_first(q); 
        int x = jval_i(a->val);
        dll_delete_node(a);

        if (!visited[x] && x != v2){    
            printf("%d ", x);     //Co the thay tuy yeu cau      
            visited[x] = 1;             
            int* list_adj = (int*)malloc(100*sizeof(int));              
            int i = getAdjacentVertices(graph, x, list_adj);
            for (int j = 0; j < i; j++){
                if (!visited[list_adj[j]])  dll_append(q, new_jval_i(list_adj[j]));
            }
        }
        else if (x == v2){
            printf("%d ", x);
            break;
        }
    }
}

int DAG(Graph graph){
    int* output = (int*)malloc(100*sizeof(int));
    JRB temp;

    jrb_traverse(temp, graph.vertices){
        int* visited = (int*)calloc(100, sizeof(int));
        //Create new stack then push temp->key 
        Dllist q = new_dllist();
        dll_append(q, temp->key);
        while(!dll_empty(q)){
            //pop
            Dllist node = dll_first(q);
            int pop = jval_i(node->val); // pop ban dau = start
            dll_delete_node(node);
            
            if(visited[pop] == 0) visited[pop] = 1;
            int adj = outdegree(graph, pop, output);
            for (int i = 0; i < adj; i++){
                if (visited[output[i]] == 0) dll_append(q, new_jval_i(output[i]));
                if (output[i] == jval_i(temp->key)) return 0; //return co chu trinh
            }
        }
        free_dllist(q);
    }
    return 1;
}

float dijkstra(Graph graph, int start, int stop, int* path, int* length){
    int number = 10000;

    int prev[number], t[number];
    //visited array
    int* visited = (int*)calloc(number, sizeof(int));
    visited[start] = 1;

    //distance from start to u array
    float distance[number];
    for (int i = 0; i < number; i++) distance[i] = INFINITIVE_VALUE;
    distance[start] = 0;

    //path and length

    // Initialize Priority Queue
    Queue q = createQueue();
    q = Enqueue(q, start, distance[start]);


    int n; //number of adjacent
    int* output = (int*)malloc(number*sizeof(int)); //list of outdgree
    int deq; //record dequeue of q
    //Dijkstra
    while(!IsQueueEmpty(q)){
        q = Dequeue(q, &deq);
        n = getAdjacentVertices(graph, deq, output);
        visited[deq] = 1;
        for (int i = 0; i < n; i++){
            if (*(visited + output[i]) == 0){
                float temp = distance[deq] + getEdgeValue(graph, deq, output[i]);
                if (temp < distance[output[i]]){
                    distance[output[i]] = temp;
                    prev[output[i]] = deq;
                    q = Enqueue(q, output[i], distance[output[i]]);
                }
            }
        }
    }
    float dis = distance[stop];
    
    t[0] = stop;
    int a = 1;
    while(stop != start){
        stop = prev[stop];
        t[a++] = stop;
    }
    for (int i = a - 1; i >= 0; i--) path[a-i-1] = t[i];
    *length = a;
    
    return dis;
}

int weak_connect(Graph graph, int v1, int v2){ //if v2 == -1, check the entire of graph
    int* visited = (int*)calloc(100, sizeof(int));
    int way[100], index = 0;
    int last;
    Dllist q = new_dllist();
    dll_append(q, new_jval_i(v1));
    while(!dll_empty(q)){
        Dllist a =  dll_first(q);
        int x = jval_i(a->val);
        dll_delete_node(a);
        if (!visited[x] && x != v2){               
            way[index] = x; index++;
            visited[x] = 1;             
            int* list_adj = (int*)malloc(100*sizeof(int));              
            int i = getAdjacentVertices(graph, x, list_adj);
            for (int j = 0; j < i; j++){
                if (!visited[list_adj[j]]) dll_append(q, new_jval_i(list_adj[j]));     
            }
        }
        else if (x == v2){
            way[index] = x;
            break;
        }
    }
    if (v2 != -1){
        if (way[index] != v2) return 0;
        else return 1;
    }
    else{
        if (index == NumberVertex(graph)) return 1;
        else return 0;
    }
}

//for test
void addVertex_int(Graph graph, int id, int name){    
    JRB check = jrb_find_int(graph.vertices, id);
    if (check == NULL) 
        jrb_insert_int(graph.vertices, id, new_jval_i(name));
    else
        printf("Vertex already exists!\n");
}
int getVertex_int(Graph graph, int id){
    JRB search = jrb_find_int(graph.vertices, id);
    if(search == NULL){
        //printf("Not found.\n");
        return -1;
    }
    else return (int)jval_v(search->val);
}