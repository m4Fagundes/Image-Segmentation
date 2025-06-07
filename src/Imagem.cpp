#include "Imagem.h"
#include <iostream>

// Define as implementações do STB aqui, ANTES de incluir os headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Imagem::Imagem() : largura(0), altura(0), canais(0), dados(nullptr) {}

Imagem::~Imagem() {
    if (dados) {
        stbi_image_free(dados);
    }
}

bool Imagem::carregar(const std::string& caminho) {
    if (dados) {
        stbi_image_free(dados);
    }
    dados = stbi_load(caminho.c_str(), &largura, &altura, &canais, 0);
    if (!dados) {
        std::cerr << "Erro ao carregar a imagem: " << caminho << std::endl;
        return false;
    }
    return true;
}

bool Imagem::salvar(const std::string& caminho) {
    if (!dados) {
        std::cerr << "Nenhum dado de imagem para salvar." << std::endl;
        return false;
    }
    // Salva como PNG
    return stbi_write_png(caminho.c_str(), largura, altura, canais, dados, largura * canais);
}

int Imagem::getLargura() const { return largura; }
int Imagem::getAltura() const { return altura; }
int Imagem::getCanais() const { return canais; }
unsigned char* Imagem::getData() const { return dados; }

unsigned char* Imagem::getPixel(int x, int y) const {
    if (x < 0 || x >= largura || y < 0 || y >= altura) {
        return nullptr;
    }
    return dados + (y * largura + x) * canais;
}