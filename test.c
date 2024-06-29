#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

// Queue structure
struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

// Graph node structure
struct Node {
    int vertex;
    struct Node* next;
};

// Graph structure with adjacency list representation
struct Graph {
    struct Node* adjList[MAX_NODES];
    int visited[MAX_NODES];
};

// Function to create a new graph node
struct Node* createNode(int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Function to create a queue
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

// Function to check if the queue is empty
int isEmpty(struct Queue* queue) {
    return queue->rear == -1;
}

// Function to add an element to the queue
void enqueue(struct Queue* queue, int value) {
    if (queue->rear == MAX_NODES - 1) {
        printf("Queue overflow\n");
    } else {
        if (queue->front == -1)
            queue->front = 0;
        queue->rear++;
        queue->items[queue->rear] = value;
    }
}

// Function to remove an element from the queue
int dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue underflow\n");
        return -1;
    } else {
        int item = queue->items[queue->front];
        queue->front++;
        if (queue->front > queue->rear) {
            queue->front = queue->rear = -1;
        }
        return item;
    }
}

// Function to perform Breadth-First Search (BFS) on the graph
void bfs(struct Graph* graph, int startVertex) {
    struct Queue* queue = createQueue();

    graph->visited[startVertex] = 1; // Mark the startVertex as visited
    enqueue(queue, startVertex);    // Enqueue the startVertex

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        printf("%d ", currentVertex);

        // Explore all adjacent vertices
        struct Node* temp = graph->adjList[currentVertex];
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (graph->visited[adjVertex] == 0) { // If not visited
                graph->visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
            temp = temp->next;
        }
    }
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest) {
    // Add edge from src to dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Since graph is undirected, add edge from dest to src also
    newNode = createNode(src);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

// Function to initialize the graph
struct Graph* createGraph() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    for (int i = 0; i < MAX_NODES; ++i) {
        graph->adjList[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Main function
int main() {
    struct Graph* graph = createGraph();

    // Adding edges
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 4);

    printf("Breadth First Search starting from vertex 0: \n");
    bfs(graph, 0);

    return 0;
}
