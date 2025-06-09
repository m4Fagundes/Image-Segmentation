#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <vector>

struct Componente {
    int pai;
    int tamanho;
    double diferenca_interna;
};

class UnionFind {
public:
    UnionFind(int n);
    int encontrar(int i);
    void unir(int p, int q, double peso);
    int getTamanho(int i);
    double getDiferencaInterna(int i);

private:
    std::vector<Componente> componentes;
};

#endif