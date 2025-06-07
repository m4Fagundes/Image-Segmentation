#include "Grafo.h"

Grafo::Grafo(int numVertices) : V(numVertices) {
    adj.resize(V);
}

void Grafo::adicionarAresta(int u, int v, double peso) {
    if (u >= 0 && u < V && v >= 0 && v < V) {
        adj[u].push_back({v, peso});
        adj[v].push_back({u, peso}); // O grafo é não direcionado
    }
}

const std::list<Aresta>& Grafo::getVizinhos(int v) const {
    return adj[v];
}

int Grafo::getNumVertices() const {
    return V;
}