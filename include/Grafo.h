#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <list>

// Estrutura para representar uma aresta com peso
struct Aresta {
    int destino;
    double peso;
};

class Grafo {
public:
    // Construtor que inicializa um grafo com um número de vértices
    Grafo(int numVertices);

    // Adiciona uma aresta bidirecional entre u e v com um peso
    void adicionarAresta(int u, int v, double peso);

    // Retorna a lista de adjacência de um vértice específico
    const std::list<Aresta>& getVizinhos(int v) const;

    // Retorna o número total de vértices
    int getNumVertices() const;

private:
    int V; // Número de vértices
    std::vector<std::list<Aresta>> adj; // Lista de adjacência
};

#endif // GRAFO_H