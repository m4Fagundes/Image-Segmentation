#include <iostream>
#include "Imagem.h"
#include "Grafo.h"

int main() {
    std::cout << "EPICO 1: Estrutura Base do Projeto" << std::endl;

    // 1. Testar a classe Imagem
    Imagem img;
    std::string caminhoEntrada = "../img/lena.png"; // Adapte se usar outra imagem
    if (!img.carregar(caminhoEntrada)) {
        return 1; // Encerra se não conseguir carregar
    }

    std::cout << "Imagem carregada com sucesso: " << caminhoEntrada << std::endl;
    std::cout << "Dimensoes: " << img.getLargura() << "x" << img.getAltura() << std::endl;
    std::cout << "Canais: " << img.getCanais() << std::endl;

    std::string caminhoSaida = "../results/lena_copia.png";
    if (img.salvar(caminhoSaida)) {
        std::cout << "Imagem salva com sucesso em: " << caminhoSaida << std::endl;
    }

    // 2. Testar a classe Grafo (exemplo simples)
    int numPixels = img.getLargura() * img.getAltura();
    Grafo grafo(numPixels);
    std::cout << "\nObjeto Grafo criado com " << grafo.getNumVertices() << " vertices." << std::endl;
    std::cout << "A logica para preencher o grafo com os pixels sera no Epico 2." << std::endl;

    return 0;
}