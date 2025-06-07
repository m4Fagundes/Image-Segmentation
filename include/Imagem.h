#ifndef IMAGEM_H
#define IMAGEM_H

#include <string>

class Imagem {
public:
    // Construtor e Destrutor
    Imagem();
    ~Imagem();

    // Carrega uma imagem de um arquivo
    bool carregar(const std::string& caminho);

    // Salva a imagem em um arquivo
    bool salvar(const std::string& caminho);

    // Getters
    int getLargura() const;
    int getAltura() const;
    int getCanais() const;
    unsigned char* getPixel(int x, int y) const;
    unsigned char* getData() const;


private:
    int largura;
    int altura;
    int canais; // Número de canais de cor (ex: 3 para RGB, 1 para cinza)
    unsigned char* dados; // Ponteiro para os dados dos pixels
};

#endif // IMAGEM_H