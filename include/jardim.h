#ifndef JARDIM_H
#define JARDIM_H

#include <vector>
#include <string>
#include "celula.h"
#include "jardineiro.h" // Incluir jardineiro para a composição

class Jardim {
private:
    Celula** grelha; // Matriz dinâmica (Array de Arrays)
    int linhas;
    int colunas;
    Jardineiro* jardineiro; // O Jardineiro pertence ao Jardim

public:
    // Construtor e Destrutor
    Jardim(int lin, int col);
    ~Jardim();

    // Métodos de Acesso
    int getLinhas() const { return linhas; }
    int getColunas() const { return colunas; }

    // Gestão do Tabuleiro
    Celula& getCelula(int lin, int col);
    bool posicaoValida(int lin, int col) const;
    std::string posicaoParaString(int lin, int col) const;
    void imprimir();

    // Motor de Simulação
    void atualizarInstante();

    void adicionarPlanta(const std::string& tipo, int linha, int coluna);
    void removerPlanta(int linha, int coluna);

    void moverJardineiro(const std::string& direcao);
    void usarFerramentaJardineiro(char tipo);
    void comprarFerramenta(const std::string& tipo);

    void listarFerramentasJardineiro() const;
    void prepararNovaJogada(); // Reseta o jardineiro
};

#endif