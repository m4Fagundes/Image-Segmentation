#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <ctime>
#include <string>

#include "Imagem.h"
#include "Grafo.h"
#include "UnionFind.h"
#include "stb_image_write.h"


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
        std::cout << "-> Imagem (Felzenszwalb) salva em: " << caminhoArquivoSaida << std::endl;
    }
    delete[] pixelsDeSaida;
}

void visualizarSegmentacaoIFT(int largura, int altura, const std::vector<int>& mapaDeRotulos, const std::string& caminhoArquivoSaida) {
    const int CANAIS_SAIDA_RGB = 3;
    int totalDePixels = largura * altura;
    std::map<int, unsigned char[CANAIS_SAIDA_RGB]> coresPorComponente;
    std::mt19937 geradorAleatorio(std::time(0));
    std::uniform_int_distribution<int> distribuicaoCor(0, 255);
    auto* pixelsDeSaida = new unsigned char[totalDePixels * CANAIS_SAIDA_RGB];

    for (int i = 0; i < totalDePixels; ++i) {
        int idComponente = mapaDeRotulos[i];
        if (idComponente != -1 && coresPorComponente.find(idComponente) == coresPorComponente.end()) {
            coresPorComponente[idComponente][0] = distribuicaoCor(geradorAleatorio);
            coresPorComponente[idComponente][1] = distribuicaoCor(geradorAleatorio);
            coresPorComponente[idComponente][2] = distribuicaoCor(geradorAleatorio);
        }
    }

    for (int i = 0; i < totalDePixels; ++i) {
        int idComponente = mapaDeRotulos[i];
        int idxPixelSaida = i * CANAIS_SAIDA_RGB;
        if(idComponente != -1){
            pixelsDeSaida[idxPixelSaida + 0] = coresPorComponente[idComponente][0];
            pixelsDeSaida[idxPixelSaida + 1] = coresPorComponente[idComponente][1];
            pixelsDeSaida[idxPixelSaida + 2] = coresPorComponente[idComponente][2];
        } else {
            pixelsDeSaida[idxPixelSaida + 0] = 0;
            pixelsDeSaida[idxPixelSaida + 1] = 0;
            pixelsDeSaida[idxPixelSaida + 2] = 0;
        }
    }
    
    if (stbi_write_png(caminhoArquivoSaida.c_str(), largura, altura, CANAIS_SAIDA_RGB, pixelsDeSaida, largura * CANAIS_SAIDA_RGB)) {
        std::cout << "-> Imagem (IFT) salva em: " << caminhoArquivoSaida << std::endl;
    }
    delete[] pixelsDeSaida;
}



void executarFluxoFelzenszwalb(const Imagem& imagemOriginal, const std::string& nomeArquivoBase, float k) {
    std::cout << "\n--- EXECUTANDO Felzenszwalb (AMG) com K = " << k << " ---" << std::endl;
    int totalDePixels = imagemOriginal.getLargura() * imagemOriginal.getAltura();
    Grafo grafoDaImagem(totalDePixels);
    
    UnionFind segmentacaoFinal = grafoDaImagem.segmentarFelzenszwalb(imagemOriginal, k);

    std::string caminhoArquivoSaida = "../results/" + nomeArquivoBase + "_felzenszwalb_k" + std::to_string((int)k) + ".png";
    visualizarSegmentacaoFelzenszwalb(imagemOriginal, segmentacaoFinal, caminhoArquivoSaida);
}

void executarFluxoIFT(const Imagem& imagemOriginal, const std::string& nomeArquivoBase, int sementesPorDimensao) {
    std::cout << "\n--- EXECUTANDO IFT com grade de " << sementesPorDimensao << "x" << sementesPorDimensao << " ---" << std::endl;
    int largura = imagemOriginal.getLargura();
    int altura = imagemOriginal.getAltura();
    
    Grafo grafoDaImagem(largura * altura);
    
    std::vector<int> sementes;
    if (sementesPorDimensao > 1) {
        int passoX = largura / sementesPorDimensao;
        int passoY = altura / sementesPorDimensao;
        for (int i = 1; i < sementesPorDimensao; ++i) {
            for (int j = 1; j < sementesPorDimensao; ++j) {
                sementes.push_back((i * passoY) * largura + (j * passoX));
            }
        }
    }
    std::cout << "Sementes geradas: " << sementes.size() << std::endl;
    
    std::vector<int> mapaDeRotulos = grafoDaImagem.segmentarIFT(imagemOriginal, sementes);

    std::string caminhoArquivoSaida = "../results/" + nomeArquivoBase + "_ift_" + std::to_string(sementes.size()) + "seeds.png";
    visualizarSegmentacaoIFT(largura, altura, mapaDeRotulos, caminhoArquivoSaida);
}



int main() {
    Imagem imagemOriginal;
    std::string caminhoArquivoEntrada = "../img/cartoon.png";
    std::string nomeArquivoBase = "cartoon";

    if (!imagemOriginal.carregar(caminhoArquivoEntrada)) {
        return 1;
    }
    std::cout << "Imagem de entrada carregada: " << caminhoArquivoEntrada << std::endl;
    
    std::vector<float> valoresDeK = {500.0f, 6000.0f, 10000.0f};
    for (float k : valoresDeK) {
        executarFluxoFelzenszwalb(imagemOriginal, nomeArquivoBase, k);
    }

    std::vector<int> valoresDeSementes = {8, 15, 25};
    for (int sementes : valoresDeSementes) {
        executarFluxoIFT(imagemOriginal, nomeArquivoBase, sementes);
    }

    std::cout << "\nProcesso de testes concluido. Verifique a pasta /results." << std::endl;

    return 0;
}