#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <list>
#include "Imagem.h"
#include "UnionFind.h"

struct Aresta {
    int destino;
    double peso;
};

struct ArestaPonderada {
    int u, v;
    double peso;

    bool operator<(const ArestaPonderada& outra) const {
        return peso < outra.peso;
    }
};


class Grafo {
public:
    Grafo(int numVertices);
    void adicionarAresta(int u, int v, double peso); // Pode ser removido futuramente
    int getNumVertices() const;

    UnionFind segmentarFelzenszwalb(const Imagem& img, float k);


private:
    int V;
    // A lista de adjacência não será usada por este algoritmo, mas mantemos por enquanto.
    std::vector<std::list<Aresta>> adj;
};

#endif