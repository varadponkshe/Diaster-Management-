#include <stdio.h>
#include <stdlib.h>

#define MAX 100

char* hospitalNames[] = {
    "Sahyadri Hospital",
    "Bharti Hospital",
    "Pune Junction",
    "Galaxy Hospital",
    "Dinanath Mangeshkar Hospital",
    "YCM Hospital (Safe Zone)"
};

struct Node {
    int vertex;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
    int* visited;
};

struct Queue {
    int items[MAX];
    int front;
    int rear;
};

struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Queue* createQueue() {
    struct Queue* q = malloc(sizeof(struct Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty(struct Queue* q) {
    return q->rear == -1;
}

void enqueue(struct Queue* q, int value) {
    if (q->rear == MAX - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue* q) {
    int item;
    if (isEmpty(q)) {
        item = -1;
    } else {
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear) {
            q->front = q->rear = -1;
        }
    }
    return item;
}

struct Graph* createGraph(int vertices) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(struct Node*));
    graph->visited = malloc(vertices * sizeof(int));
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void BFS(struct Graph* graph, int startVertex, int endVertex) {
    int parent[MAX];
    for (int i = 0; i < MAX; i++)
        parent[i] = -1;

    struct Queue* q = createQueue();

    graph->visited[startVertex] = 1;
    enqueue(q, startVertex);

    while (!isEmpty(q)) {
        int current = dequeue(q);
        struct Node* temp = graph->adjLists[current];
        while (temp) {
            int adj = temp->vertex;
            if (!graph->visited[adj]) {
                graph->visited[adj] = 1;
                parent[adj] = current;
                enqueue(q, adj);
            }
            temp = temp->next;
        }
    }

    int path[MAX];
    int count = 0;
    int crawl = endVertex;
    path[count++] = crawl;
    while (parent[crawl] != -1) {
        crawl = parent[crawl];
        path[count++] = crawl;
    }

    printf("\n-----------------------------------------\n");
    printf("Shortest Safe Route (Using BFS):\n");
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", hospitalNames[path[i]]);
        if (i != 0) printf(" -> ");
    }
    printf("\n-----------------------------------------\n");
}

void DFSUtil(struct Graph* graph, int vertex, int endVertex, int path[], int pathIndex) {
    graph->visited[vertex] = 1;
    path[pathIndex++] = vertex;

    if (vertex == endVertex) {
        printf("Route: ");
        for (int i = 0; i < pathIndex; i++) {
            printf("%s", hospitalNames[path[i]]);
            if (i != pathIndex - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        struct Node* temp = graph->adjLists[vertex];
        while (temp) {
            int adj = temp->vertex;
            if (!graph->visited[adj]) {
                DFSUtil(graph, adj, endVertex, path, pathIndex);
            }
            temp = temp->next;
        }
    }

    pathIndex--;
    graph->visited[vertex] = 0;
}

void DFS(struct Graph* graph, int startVertex, int endVertex) {
    int path[MAX];
    int pathIndex = 0;
    printf("\nAll Possible Safe Routes (Using DFS):\n");
    DFSUtil(graph, startVertex, endVertex, path, pathIndex);
}

int main() {
    struct Graph* graph = createGraph(6);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 5);
    addEdge(graph, 4, 5);

    printf("=========================================\n");
    printf("      Disaster Response Routing System\n");
    printf("=========================================\n");

    printf("\nHospitals and Locations:\n");
    for (int i = 0; i < 6; i++)
        printf("%d - %s\n", i, hospitalNames[i]);

    BFS(graph, 0, 5);
    for (int i = 0; i < graph->numVertices; i++)
        graph->visited[i] = 0;
    DFS(graph, 0, 5);

    printf("\n=========================================\n");
    printf("      End of Simulation\n");
    printf("=========================================\n");

    return 0;
}
