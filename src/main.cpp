#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <ctime>

#include "Imagem.h"
#include "Grafo.h"
#include "UnionFind.h"
#include "stb_image_write.h"

/**
 * @brief Gera uma imagem colorida para o resultado do algoritmo Felzenszwalb (baseado em UnionFind).
 */
void visualizarSegmentacaoFelzenszwalb(const Imagem& imagemOriginal, UnionFind& segmentacao, const std::string& caminhoArquivoSaida) {
    int largura = imagemOriginal.getLargura();
    int altura = imagemOriginal.getAltura();
    const int CANAIS_SAIDA_RGB = 3;
    int totalDePixels = largura * altura;

    std::map<int, unsigned char[CANAIS_SAIDA_RGB]> coresPorComponente;
    std::mt19937 geradorAleatorio(std::time(0));
    std::uniform_int_distribution<int> distribuicaoCor(0, 255);

    auto* pixelsDeSaida = new unsigned char[totalDePixels * CANAIS_SAIDA_RGB];

    for (int i = 0; i < totalDePixels; ++i) {
        int idComponente = segmentacao.encontrar(i);
        
        if (coresPorComponente.find(idComponente) == coresPorComponente.end()) {
            coresPorComponente[idComponente][0] = distribuicaoCor(geradorAleatorio); // R
            coresPorComponente[idComponente][1] = distribuicaoCor(geradorAleatorio); // G
            coresPorComponente[idComponente][2] = distribuicaoCor(geradorAleatorio); // B
        }
        
        int idxPixelSaida = i * CANAIS_SAIDA_RGB;
        pixelsDeSaida[idxPixelSaida + 0] = coresPorComponente[idComponente][0];
        pixelsDeSaida[idxPixelSaida + 1] = coresPorComponente[idComponente][1];
        pixelsDeSaida[idxPixelSaida + 2] = coresPorComponente[idComponente][2];
    }
    
    if (stbi_write_png(caminhoArquivoSaida.c_str(), largura, altura, CANAIS_SAIDA_RGB, pixelsDeSaida, largura * CANAIS_SAIDA_RGB)) {
        std::cout << "-> Imagem (Felzenszwalb) salva em: " << caminhoArquivoSaida << std::endl;
    }

    delete[] pixelsDeSaida;
}


/**
 * @brief Gera uma imagem colorida para o resultado do algoritmo IFT (baseado em mapa de rótulos).
 */
void visualizarSegmentacaoIFT(int largura, int altura, const std::vector<int>& mapaDeRotulos, const std::string& caminhoArquivoSaida) {
    const int CANAIS_SAIDA_RGB = 3;
    int totalDePixels = largura * altura;

    std::map<int, unsigned char[CANAIS_SAIDA_RGB]> coresPorComponente;
    std::mt19937 geradorAleatorio(std::time(0));
    std::uniform_int_distribution<int> distribuicaoCor(0, 255);

    auto* pixelsDeSaida = new unsigned char[totalDePixels * CANAIS_SAIDA_RGB];

    for (int i = 0; i < totalDePixels; ++i) {
        int idComponente = mapaDeRotulos[i];
        
        if (coresPorComponente.find(idComponente) == coresPorComponente.end()) {
            coresPorComponente[idComponente][0] = distribuicaoCor(geradorAleatorio);
            coresPorComponente[idComponente][1] = distribuicaoCor(geradorAleatorio);
            coresPorComponente[idComponente][2] = distribuicaoCor(geradorAleatorio);
        }
        
        int idxPixelSaida = i * CANAIS_SAIDA_RGB;
        pixelsDeSaida[idxPixelSaida + 0] = coresPorComponente[idComponente][0];
        pixelsDeSaida[idxPixelSaida + 1] = coresPorComponente[idComponente][1];
        pixelsDeSaida[idxPixelSaida + 2] = coresPorComponente[idComponente][2];
    }
    
    if (stbi_write_png(caminhoArquivoSaida.c_str(), largura, altura, CANAIS_SAIDA_RGB, pixelsDeSaida, largura * CANAIS_SAIDA_RGB)) {
        std::cout << "-> Imagem (IFT) salva em: " << caminhoArquivoSaida << std::endl;
    }

    delete[] pixelsDeSaida;
}



void executarFluxoFelzenszwalb(const Imagem& imagemOriginal, const std::string& nomeArquivoBase) {
    std::cout << "\n--- EXECUTANDO ALGORITMO 1: Felzenszwalb (AMG) ---" << std::endl;
    int totalDePixels = imagemOriginal.getLargura() * imagemOriginal.getAltura();
    Grafo grafoDaImagem(totalDePixels);

    const float FATOR_K = 300.0f; 
    
    UnionFind segmentacaoFinal = grafoDaImagem.segmentarFelzenszwalb(imagemOriginal, FATOR_K);

    std::string caminhoArquivoSaida = "../results/" + nomeArquivoBase + "_felzenszwalb_k" + std::to_string((int)FATOR_K) + ".png";
    visualizarSegmentacaoFelzenszwalb(imagemOriginal, segmentacaoFinal, caminhoArquivoSaida);
}


void executarFluxoIFT(const Imagem& imagemOriginal, const std::string& nomeArquivoBase) {
    std::cout << "\n--- EXECUTANDO ALGORITMO 2: Image Foresting Transform (IFT) ---" << std::endl;
    int largura = imagemOriginal.getLargura();
    int altura = imagemOriginal.getAltura();
    
    Grafo grafoDaImagem(largura * altura);
    
    int sementesPorDimensao = 10;
    std::vector<int> sementes;
    int passoX = largura / sementesPorDimensao;
    int passoY = altura / sementesPorDimensao;
    for (int i = 1; i < sementesPorDimensao; ++i) {
        for (int j = 1; j < sementesPorDimensao; ++j) {
            sementes.push_back((i * passoY) * largura + (j * passoX));
        }
    }
    std::cout << "Sementes selecionadas: " << sementes.size() << std::endl;
    
    std::vector<int> mapaDeRotulos = grafoDaImagem.segmentarIFT(imagemOriginal, sementes);

    std::string caminhoArquivoSaida = "../results/" + nomeArquivoBase + "_ift_" + std::to_string(sementes.size()) + "seeds.png";
    visualizarSegmentacaoIFT(largura, altura, mapaDeRotulos, caminhoArquivoSaida);
}


// --- Ponto de Entrada Principal ---

int main() {
    Imagem imagemOriginal;
    std::string caminhoArquivoEntrada = "../img/lena.png";
    std::string nomeArquivoBase = "lena";

    if (!imagemOriginal.carregar(caminhoArquivoEntrada)) {
        return 1;
    }
    std::cout << "Imagem de entrada carregada: " << caminhoArquivoEntrada << std::endl;
    
    executarFluxoFelzenszwalb(imagemOriginal, nomeArquivoBase);
    executarFluxoIFT(imagemOriginal, nomeArquivoBase);

    std::cout << "\nProcesso concluido. Verifique a pasta /results." << std::endl;

    return 0;
}