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
    int getNumVertices() const;
    
    // Algoritmo 1: Baseado em AMG
    UnionFind segmentarFelzenszwalb(const Imagem& img, float k);
    // Algoritmo 2: Baseado em Caminho Mínimo (IFT)
    std::vector<int> segmentarIFT(const Imagem& img, const std::vector<int>& sementes);


private:
    int V; // Número de vértices (pixels)
    std::vector<std::list<Aresta>> adj;
};

#endif