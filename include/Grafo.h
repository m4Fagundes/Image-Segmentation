#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <list>
#include "Imagem.h" // Incluir Imagem.h
#include "UnionFind.h" // Incluir UnionFind.h

// ... (struct Aresta existente) ...
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
    // ... (Construtor e métodos antigos) ...
    Grafo(int numVertices);
    void adicionarAresta(int u, int v, double peso); // Pode ser removido futuramente
    int getNumVertices() const;

    // --------- Novo método para o Épico 2 ---------
    UnionFind segmentarFelzenszwalb(const Imagem& img, float k);


private:
    int V;
    // A lista de adjacência não será usada por este algoritmo, mas mantemos por enquanto.
    std::vector<std::list<Aresta>> adj;
};

#endif // GRAFO_H