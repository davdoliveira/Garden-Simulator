#ifndef JARDIM_H
#define JARDIM_H

#include <vector>
#include <string>
#include "celula.h"
#include "jardineiro.h"

class Jardim {
private:
    Celula** grelha;
    int linhas;
    int colunas;
    Jardineiro* jardineiro;

public:
    Jardim(int lin, int col);

    // --- NOVO: Construtor de Cópia ---
    Jardim(const Jardim& outro);

    ~Jardim();

    // Métodos de Acesso
    int getLinhas() const { return linhas; }
    int getColunas() const { return colunas; }

    // Gestão do Tabuleiro
    Celula& getCelula(int lin, int col);
    bool posicaoValida(int lin, int col) const;
    std::string posicaoParaString(int lin, int col) const;
    void imprimir();

    // Motor
    void atualizarInstante();
    void adicionarPlanta(const std::string& tipo, int linha, int coluna);
    void removerPlanta(int linha, int coluna);
    void moverJardineiro(const std::string& direcao);
    void usarFerramentaJardineiro(char tipo);
    void comprarFerramenta(const std::string& tipo);
    void listarFerramentasJardineiro() const;
    void prepararNovaJogada();
    void entrarJardineiro(int linha, int coluna);
    void sairJardineiro();
};

#endif