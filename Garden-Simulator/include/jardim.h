#ifndef JARDIM_H
#define JARDIM_H

#include <string>
#include "celula.h"
#include "jardineiro.h"

class Ferramenta;

class Jardim {
private:
    Celula** grelha;
    int linhas;
    int colunas;
    Jardineiro* jardineiro;
    int instante_atual;

    Ferramenta* criarFerramentaAleatoria();
    void colocarFerramentaAleatoria();
    void verificarApanhaFerramenta();

public:
    Jardim(int lin, int col);
    Jardim(const Jardim& outro);
    ~Jardim();

    int getLinhas() const { return linhas; }
    int getColunas() const { return colunas; }
    int getInstanteAtual() const { return instante_atual; }
    Jardineiro* getJardineiro() const { return jardineiro; }

    Celula& getCelula(int lin, int col);
    const Celula& getCelula(int lin, int col) const;
    bool posicaoValida(int lin, int col) const;
    std::string posicaoParaString(int lin, int col) const;
    void imprimir() const;

    void atualizarInstante();
    bool adicionarPlanta(const std::string& tipo, int linha, int coluna);
    bool removerPlanta(int linha, int coluna);
    bool moverJardineiro(const std::string& direcao);
    bool pegarFerramentaJardineiro(int num_serie);
    void largarFerramentaJardineiro();
    bool comprarFerramenta(const std::string& tipo);

    void listarFerramentasJardineiro() const;
    void listarPlantas() const;
    void listarPlanta(int lin, int col) const;
    void listarArea() const;
    void listarSolo(int lin, int col, int raio) const;

    bool entrarJardineiro(int linha, int coluna);
    bool sairJardineiro();
};

#endif // JARDIM_H
