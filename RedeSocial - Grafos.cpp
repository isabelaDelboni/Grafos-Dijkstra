#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define NAME_SIZE 50

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node* adjLists[MAX_USERS];
    char userNames[MAX_USERS][NAME_SIZE];
} Graph;

Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addUser(Graph* graph, const char* name) {
    if (graph->numVertices >= MAX_USERS) {
        printf("Limite máximo de usuários atingido!\n");
        return;
    }
    strcpy(graph->userNames[graph->numVertices], name);
    printf("Usuário \"%s\" cadastrado com ID %d\n", name, graph->numVertices);
    graph->numVertices++;
}

void addEdge(Graph* graph, int src, int dest) {
    if (src >= graph->numVertices || dest >= graph->numVertices) {
        printf("IDs inválidos!\n");
        return;
    }
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;

    printf(" \"%s\" e \"%s\" agora são amigos!\n",
           graph->userNames[src], graph->userNames[dest]);
}

void printGraph(Graph* graph) {
    printf("\n===== REDE DE AMIZADES =====\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("[%d] %s: ", i, graph->userNames[i]);
        Node* temp = graph->adjLists[i];
        if (!temp) printf("(sem amigos)");
        while (temp) {
            printf(" [%d] %s", temp->vertex, graph->userNames[temp->vertex]);
            if (temp->next) printf(" ?");
            temp = temp->next;
        }
        printf("\n");
    }
    printf("============================\n");
}

void dfsUtil(Graph* graph, int vertex, int visited[]) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    Node* temp = graph->adjLists[vertex];
    while (temp) {
        if (!visited[temp->vertex])
            dfsUtil(graph, temp->vertex, visited);
        temp = temp->next;
    }
}

void dfs(Graph* graph, int startVertex) {
    int visited[MAX_USERS] = {0};
    printf("DFS a partir de %s (ID %d): ", graph->userNames[startVertex], startVertex);
    dfsUtil(graph, startVertex, visited);
    printf("\n");
}

void bfs(Graph* graph, int startVertex) {
    int visited[MAX_USERS] = {0};
    int queue[MAX_USERS], front = 0, rear = 0;

    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    printf("BFS a partir de %s (ID %d): ", graph->userNames[startVertex], startVertex);

    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);

        Node* temp = graph->adjLists[current];
        while (temp) {
            int adj = temp->vertex;
            if (!visited[adj]) {
                visited[adj] = 1;
                queue[rear++] = adj;
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

void suggestFriends(Graph* graph, int userId) {
    if (userId >= graph->numVertices) {
        printf(" Usuário inválido!\n");
        return;
    }

    int visited[MAX_USERS] = {0};
    int level[MAX_USERS] = {0};
    int queue[MAX_USERS], front = 0, rear = 0;

    visited[userId] = 1;
    queue[rear++] = userId;

    while (front < rear) {
        int current = queue[front++];
        Node* temp = graph->adjLists[current];
        while (temp) {
            int neighbor = temp->vertex;
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                level[neighbor] = level[current] + 1;
                queue[rear++] = neighbor;
            }
            temp = temp->next;
        }
    }

    printf("\nSugestões de amigos para \"%s\" (ID %d):\n", graph->userNames[userId], userId);
    int found = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (level[i] == 2) {
            printf("- [%d] %s\n", i, graph->userNames[i]);
            found = 1;
        }
    }
    if (!found)
        printf("Nenhuma sugestão encontrada.\n");
}

int main() {
    Graph* graph = createGraph();
    int option;

    addUser(graph, "Alice");
    addUser(graph, "Bob");
    addUser(graph, "Carlos");
    addUser(graph, "Diana");
    addUser(graph, "Eva");

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 3);
    addEdge(graph, 3, 4);

    do {
        printf("\n--- MENU ---\n");
        printf("1. Adicionar novo usuário\n");
        printf("2. Criar amizade entre usuários\n");
        printf("3. Mostrar rede de amizades\n");
        printf("4. Explorar com DFS\n");
        printf("5. Explorar com BFS\n");
        printf("6. Sugerir amigos\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &option);

        if (option == 1) {
            char name[NAME_SIZE];
            printf("Digite o nome do novo usuário (sem espaços): ");
            scanf("%s", name); 
            addUser(graph, name);
        } else if (option == 2) {
            int id1, id2;
            printf("Digite os IDs dos dois usuários (ex: 0 3): ");
            scanf("%d %d", &id1, &id2);
            addEdge(graph, id1, id2);
        } else if (option == 3) {
            printGraph(graph);
        } else if (option == 4) {
            int id;
            printf("Digite o ID do usuário inicial: ");
            scanf("%d", &id);
            dfs(graph, id);
        } else if (option == 5) {
            int id;
            printf("Digite o ID do usuário inicial: ");
            scanf("%d", &id);
            bfs(graph, id);
        } else if (option == 6) {
            int id;
            printf("Digite o ID para sugestão de amigos: ");
            scanf("%d", &id);
            suggestFriends(graph, id);
        } else if (option == 0) {
            printf("?? Saindo...\n");
        } else {
            printf("Opção inválida!\n");
        }

    } while (option != 0);

    return 0;
}
