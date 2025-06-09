#include "Grafo.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Grafo::Grafo(int numVertices) : V(numVertices) {
    adj.resize(V);
}

int Grafo::getNumVertices() const {
    return V;
}

double calcularDiferenca(unsigned char* p1, unsigned char* p2, int canais) {
    if (canais >= 3) {
        long r = (long)p1[0] - (long)p2[0];
        long g = (long)p1[1] - (long)p2[1];
        long b = (long)p1[2] - (long)p2[2];
        return std::sqrt(r*r + g*g + b*b);
    }
    return std::abs((long)p1[0] - (long)p2[0]);
}

UnionFind Grafo::segmentarFelzenszwalb(const Imagem& img, float k) {
    int largura = img.getLargura();
    int altura = img.getAltura();
    int numPixels = getNumVertices();
    int canais = img.getCanais();

    std::vector<ArestaPonderada> arestas;
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int pixel_idx = y * largura + x;

            if (x < largura - 1) {
                int vizinho_idx = y * largura + (x + 1);
                double peso = calcularDiferenca(img.getPixel(x, y), img.getPixel(x + 1, y), canais);
                arestas.push_back({pixel_idx, vizinho_idx, peso});
            }

            if (y < altura - 1) {
                int vizinho_idx = (y + 1) * largura + x;
                double peso = calcularDiferenca(img.getPixel(x, y), img.getPixel(x, y + 1), canais);
                arestas.push_back({pixel_idx, vizinho_idx, peso});
            }
        }
    }
    std::cout << "Grafo construido com " << arestas.size() << " arestas." << std::endl;

    std::sort(arestas.begin(), arestas.end());
    std::cout << "Arestas ordenadas." << std::endl;

    UnionFind uf(numPixels);

    for (const auto& aresta : arestas) {
        int raizU = uf.encontrar(aresta.u);
        int raizV = uf.encontrar(aresta.v);

        if (raizU != raizV) {
            double diff_interna_u = uf.getDiferencaInterna(raizU);
            double diff_interna_v = uf.getDiferencaInterna(raizV);
            double limiar_u = diff_interna_u + (k / uf.getTamanho(raizU));
            double limiar_v = diff_interna_v + (k / uf.getTamanho(raizV));
            
            if (aresta.peso <= std::min(limiar_u, limiar_v)) {
                uf.unir(raizU, raizV, aresta.peso);
            }
        }
    }
    std::cout << "Segmentacao Felzenszwalb concluida." << std::endl;

    return uf;
}