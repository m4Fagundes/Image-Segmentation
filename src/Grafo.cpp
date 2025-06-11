#include "Grafo.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

double calcularDiferenca(unsigned char* p1, unsigned char* p2, int canais);


Grafo::Grafo(int numVertices) : V(numVertices) {

}

int Grafo::getNumVertices() const {
    return this->V;
}

/**
 * @brief Segmenta a imagem usando o algoritmo de Felzenszwalb & Huttenlocher (baseado em AMG).
 */
UnionFind Grafo::segmentarFelzenszwalb(const Imagem& img, float k) {
    int largura = img.getLargura();
    int altura = img.getAltura();
    int numPixels = this->getNumVertices();

    std::vector<ArestaPonderada> arestas;
    arestas.reserve(largura * altura * 2);

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int pixelAtualIdx = y * largura + x;
            
            if (x < largura - 1) {
                int vizinhoIdx = y * largura + (x + 1);
                double peso = calcularDiferenca(img.getPixel(x, y), img.getPixel(x + 1, y), img.getCanais());
                arestas.push_back({pixelAtualIdx, vizinhoIdx, peso});
            }
            if (y < altura - 1) {
                int vizinhoIdx = (y + 1) * largura + x;
                double peso = calcularDiferenca(img.getPixel(x, y), img.getPixel(x, y + 1), img.getCanais());
                arestas.push_back({pixelAtualIdx, vizinhoIdx, peso});
            }
        }
    }

    std::sort(arestas.begin(), arestas.end());

    UnionFind uf(numPixels);
    for (const auto& aresta : arestas) {
        int raizU = uf.encontrar(aresta.u);
        int raizV = uf.encontrar(aresta.v);

        if (raizU != raizV) {
            double limiarU = uf.getDiferencaInterna(raizU) + (k / uf.getTamanho(raizU));
            double limiarV = uf.getDiferencaInterna(raizV) + (k / uf.getTamanho(raizV));
            
            if (aresta.peso <= std::min(limiarU, limiarV)) {
                uf.unir(raizU, raizV, aresta.peso);
            }
        }
    }
    std::cout << "Segmentacao Felzenszwalb (AMG) concluida." << std::endl;
    return uf;
}

/**
 * @brief Segmenta a imagem usando a Image Foresting Transform (baseada em Caminho Mínimo).
 */
std::vector<int> Grafo::segmentarIFT(const Imagem& img, const std::vector<int>& sementes) {
    int largura = img.getLargura();
    int altura = img.getAltura();
    int numPixels = this->getNumVertices();
    
    const double CUSTO_MAX = std::numeric_limits<double>::max();
    std::vector<double> custos(numPixels, CUSTO_MAX);
    std::vector<int> mapaDeRotulos(numPixels, -1);
    
    using ParCustoPixel = std::pair<double, int>;
    std::priority_queue<ParCustoPixel, std::vector<ParCustoPixel>, std::greater<ParCustoPixel>> filaDePrioridade;

    for (int sementeIdx : sementes) {
        custos[sementeIdx] = 0;
        mapaDeRotulos[sementeIdx] = sementeIdx;
        filaDePrioridade.push({0.0, sementeIdx});
    }

    const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    while (!filaDePrioridade.empty()) {
        double custoAtual = filaDePrioridade.top().first;
        int p_idx = filaDePrioridade.top().second;
        filaDePrioridade.pop();

        if (custoAtual > custos[p_idx]) {
            continue;
        }

        int p_x = p_idx % largura;
        int p_y = p_idx / largura;

        for (int i = 0; i < 8; ++i) {
            int q_x = p_x + dx[i];
            int q_y = p_y + dy[i];

            if (q_x >= 0 && q_x < largura && q_y >= 0 && q_y < altura) {
                int q_idx = q_y * largura + q_x;
                
                double pesoAresta = calcularDiferenca(img.getPixel(p_x, p_y), img.getPixel(q_x, q_y), img.getCanais());
                double novoCusto = std::max(custos[p_idx], pesoAresta);

                if (novoCusto < custos[q_idx]) {
                    custos[q_idx] = novoCusto;
                    mapaDeRotulos[q_idx] = mapaDeRotulos[p_idx];
                    filaDePrioridade.push({novoCusto, q_idx});
                }
            }
        }
    }
    
    std::cout << "Segmentacao IFT (Caminho Minimo) concluida." << std::endl;
    return mapaDeRotulos;
}

/**
 * @brief Calcula a diferença (distância) entre as cores de dois pixels.
 */
double calcularDiferenca(unsigned char* p1, unsigned char* p2, int canais) {
    if (canais >= 3) {
        long r = (long)p1[0] - (long)p2[0];
        long g = (long)p1[1] - (long)p2[1];
        long b = (long)p1[2] - (long)p2[2];
        return std::sqrt(r * r + g * g + b * b);
    }
    return std::abs((long)p1[0] - (long)p2[0]);
}