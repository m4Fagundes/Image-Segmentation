#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <ctime> // FIX: Incluir para std::time

#include "Imagem.h"
#include "Grafo.h"
#include "UnionFind.h"
#include "stb_image_write.h"

/**
 * @brief Gera uma imagem colorida a partir do resultado da segmentação.
 * * Itera sobre cada pixel, encontra o componente (segmento) ao qual ele pertence
 * e o colore com uma cor aleatória única para aquele componente.
 * * @param imagemOriginal A imagem base para obter as dimensões.
 * @param segmentacao O objeto UnionFind contendo os componentes finais.
 * @param caminhoArquivoSaida O local para salvar a imagem resultante.
 */
void visualizarSegmentacao(const Imagem& imagemOriginal, UnionFind& segmentacao, const std::string& caminhoArquivoSaida) { // FIX: Removido o 'const'
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
        
        pixelsDeSaida[i * CANAIS_SAIDA_RGB + 0] = coresPorComponente[idComponente][0];
        pixelsDeSaida[i * CANAIS_SAIDA_RGB + 1] = coresPorComponente[idComponente][1];
        pixelsDeSaida[i * CANAIS_SAIDA_RGB + 2] = coresPorComponente[idComponente][2];
    }
    
    if (stbi_write_png(caminhoArquivoSaida.c_str(), largura, altura, CANAIS_SAIDA_RGB, pixelsDeSaida, largura * CANAIS_SAIDA_RGB)) {
        std::cout << "Imagem segmentada salva em: " << caminhoArquivoSaida << std::endl;
    } else {
        std::cerr << "Erro ao salvar a imagem segmentada." << std::endl;
    }

    delete[] pixelsDeSaida;
}

int main() {
    std::cout << "EPICO 2: Segmentacao com AMG (Felzenszwalb)" << std::endl;

    Imagem imagemOriginal;
    std::string caminhoArquivoEntrada = "../img/lena.png";
    if (!imagemOriginal.carregar(caminhoArquivoEntrada)) {
        return 1;
    }
    std::cout << "Imagem carregada: " << imagemOriginal.getLargura() << "x" << imagemOriginal.getAltura() << std::endl;

    int totalDePixels = imagemOriginal.getLargura() * imagemOriginal.getAltura();
    Grafo grafoDaImagem(totalDePixels);

    // O fator K controla a preferência por segmentos maiores. 
    // Quanto maior o K, maiores os segmentos resultantes.
    const float FATOR_K = 300.0f; 
    
    UnionFind segmentacaoFinal = grafoDaImagem.segmentarFelzenszwalb(imagemOriginal, FATOR_K);

    std::string caminhoArquivoSaida = "../results/lena_segmentada_k300.png";
    visualizarSegmentacao(imagemOriginal, segmentacaoFinal, caminhoArquivoSaida);

    return 0;
}