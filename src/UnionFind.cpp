#include "UnionFind.h"
#include <utility>

UnionFind::UnionFind(int n) {
    componentes.resize(n);
    for (int i = 0; i < n; ++i) {
        componentes[i] = {i, 1, 0.0};
    }
}

int UnionFind::encontrar(int i) {
    if (componentes[i].pai != i) {
        componentes[i].pai = encontrar(componentes[i].pai);
    }
    return componentes[i].pai;
}

void UnionFind::unir(int p, int q, double peso) {
    int raizP = encontrar(p);
    int raizQ = encontrar(q);

    if (raizP != raizQ) {
        if (componentes[raizP].tamanho < componentes[raizQ].tamanho) {
            std::swap(raizP, raizQ);
        }

        componentes[raizQ].pai = raizP;
        componentes[raizP].tamanho += componentes[raizQ].tamanho;
        componentes[raizP].diferenca_interna = peso;
    }
}

int UnionFind::getTamanho(int i) {
    return componentes[encontrar(i)].tamanho;
}

double UnionFind::getDiferencaInterna(int i) {
    return componentes[encontrar(i)].diferenca_interna;
}