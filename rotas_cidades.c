#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CIDADES 100
#define MAX_NOME 50

typedef struct Rota {
    int destino;
    int distancia;
    struct Rota* proxima;
} Rota;

typedef struct Cidade {
    char nome[MAX_NOME];
    Rota* rotas;
} Cidade;

typedef struct Grafo {
    Cidade cidades[MAX_CIDADES];
    int numCidades;
} Grafo;

Grafo* criarGrafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numCidades = 0;
    return grafo;
}

int encontrarIndiceCidade(Grafo* grafo, const char* nome) {
    for (int i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void adicionarCidade(Grafo* grafo, const char* nome) {
    if (grafo->numCidades >= MAX_CIDADES) {
        printf("Número máximo de cidades atingido!\n");
        return;
    }
    
    if (encontrarIndiceCidade(grafo, nome) != -1) {
        printf("Cidade já cadastrada!\n");
        return;
    }
    
    strcpy(grafo->cidades[grafo->numCidades].nome, nome);
    grafo->cidades[grafo->numCidades].rotas = NULL;
    grafo->numCidades++;
    printf("Cidade '%s' adicionada com sucesso!\n", nome);
}

void adicionarRota(Grafo* grafo, const char* origem, const char* destino, int distancia) {
    int idxOrigem = encontrarIndiceCidade(grafo, origem);
    int idxDestino = encontrarIndiceCidade(grafo, destino);
    
    if (idxOrigem == -1 || idxDestino == -1) {
        printf("Cidade de origem ou destino não encontrada!\n");
        return;
    }
    
    // Adiciona rota da origem para o destino
    Rota* novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = idxDestino;
    novaRota->distancia = distancia;
    novaRota->proxima = grafo->cidades[idxOrigem].rotas;
    grafo->cidades[idxOrigem].rotas = novaRota;
    
    // Adiciona rota do destino para a origem (grafo não direcionado)
    novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = idxOrigem;
    novaRota->distancia = distancia;
    novaRota->proxima = grafo->cidades[idxDestino].rotas;
    grafo->cidades[idxDestino].rotas = novaRota;
    
    printf("Rota entre '%s' e '%s' (%d km) adicionada com sucesso!\n", origem, destino, distancia);
}

void dijkstra(Grafo* grafo, int origem, int destino) {
    int distancias[MAX_CIDADES];
    int visitados[MAX_CIDADES];
    int anteriores[MAX_CIDADES];
    
    // Inicialização
    for (int i = 0; i < grafo->numCidades; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = 0;
        anteriores[i] = -1;
    }
    distancias[origem] = 0;
    
    for (int count = 0; count < grafo->numCidades - 1; count++) {
        // Encontra o vértice com menor distância não visitado
        int min = INT_MAX, u;
        for (int v = 0; v < grafo->numCidades; v++) {
            if (!visitados[v] && distancias[v] <= min) {
                min = distancias[v];
                u = v;
            }
        }
        
        visitados[u] = 1;
        
        // Atualiza as distâncias dos vértices adjacentes
        Rota* rota = grafo->cidades[u].rotas;
        while (rota != NULL) {
            int v = rota->destino;
            if (!visitados[v] && distancias[u] != INT_MAX && 
                distancias[u] + rota->distancia < distancias[v]) {
                distancias[v] = distancias[u] + rota->distancia;
                anteriores[v] = u;
            }
            rota = rota->proxima;
        }
    }
    
    // Exibe o resultado
    if (distancias[destino] == INT_MAX) {
        printf("Não existe caminho entre %s e %s\n", 
               grafo->cidades[origem].nome, grafo->cidades[destino].nome);
        return;
    }
    
    printf("Menor distancia entre %s e %s: %d km\n", 
           grafo->cidades[origem].nome, grafo->cidades[destino].nome, distancias[destino]);
    
    // Reconstruir e exibir o caminho
    int caminho[MAX_CIDADES];
    int atual = destino;
    int tamanhoCaminho = 0;
    
    while (anteriores[atual] != -1) {
        caminho[tamanhoCaminho++] = atual;
        atual = anteriores[atual];
    }
    caminho[tamanhoCaminho] = origem;
    
    printf("Caminho: ");
    for (int i = tamanhoCaminho; i >= 0; i--) {
        printf("%s", grafo->cidades[caminho[i]].nome);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

void listarCidades(Grafo* grafo) {
    printf("\nCidades cadastradas:\n");
    for (int i = 0; i < grafo->numCidades; i++) {
        printf("%d: %s\n", i, grafo->cidades[i].nome);
    }
}

void listarRotas(Grafo* grafo) {
    printf("\nRotas cadastradas:\n");
    for (int i = 0; i < grafo->numCidades; i++) {
        Rota* rota = grafo->cidades[i].rotas;
        while (rota != NULL) {
            // Para evitar duplicação (já que o grafo é não direcionado)
            if (i < rota->destino) {
                printf("%s <-> %s (%d km)\n", 
                       grafo->cidades[i].nome, 
                       grafo->cidades[rota->destino].nome, 
                       rota->distancia);
            }
            rota = rota->proxima;
        }
    }
}

void menu() {
    printf("\n=== SISTEMA DE ROTAS ENTRE CIDADES ===\n");
    printf("1. Adicionar cidade\n");
    printf("2. Adicionar rota\n");
    printf("3. Listar cidades\n");
    printf("4. Listar rotas\n");
    printf("5. Calcular menor caminho\n");
    printf("6. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Grafo* grafo = criarGrafo();
    int opcao;
    
    do {
        menu();
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer
        
        switch (opcao) {
            case 1: {
                char nome[MAX_NOME];
                printf("Nome da cidade: ");
                fgets(nome, MAX_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove a quebra de linha
                adicionarCidade(grafo, nome);
                break;
            }
            case 2: {
                char origem[MAX_NOME], destino[MAX_NOME];
                int distancia;
                
                printf("Cidade de origem: ");
                fgets(origem, MAX_NOME, stdin);
                origem[strcspn(origem, "\n")] = '\0';
                
                printf("Cidade de destino: ");
                fgets(destino, MAX_NOME, stdin);
                destino[strcspn(destino, "\n")] = '\0';
                
                printf("Distancia (km): ");
                scanf("%d", &distancia);
                getchar();
                
                adicionarRota(grafo, origem, destino, distancia);
                break;
            }
            case 3:
                listarCidades(grafo);
                break;
            case 4:
                listarRotas(grafo);
                break;
            case 5: {
                char origem[MAX_NOME], destino[MAX_NOME];
                
                printf("Cidade de origem: ");
                fgets(origem, MAX_NOME, stdin);
                origem[strcspn(origem, "\n")] = '\0';
                
                printf("Cidade de destino: ");
                fgets(destino, MAX_NOME, stdin);
                destino[strcspn(destino, "\n")] = '\0';
                
                int idxOrigem = encontrarIndiceCidade(grafo, origem);
                int idxDestino = encontrarIndiceCidade(grafo, destino);
                
                if (idxOrigem == -1 || idxDestino == -1) {
                    printf("Cidade de origem ou destino não encontrada!\n");
                } else {
                    dijkstra(grafo, idxOrigem, idxDestino);
                }
                break;
            }
            case 6:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 6);
    
    // Liberar memória
    // (Implementação da liberação de memória ficaria aqui)
    
    return 0;
}