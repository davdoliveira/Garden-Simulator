#ifndef FERRAMENTA_H
#define FERRAMENTA_H

#include <string>

class Jardim;

class Ferramenta {
private:
    static int proximo_num_serie;

protected:
    int num_serie;
    int capacidade_maxima; // 0 = infinito (ex: Tesoura de Poda)
    int quantidade_atual;  // Água no regador, adubo no pacote, usos na FerramentaZ
    char simbolo;

public:
    Ferramenta(int cap_max, char simb);
    Ferramenta(const Ferramenta& outra) = default;
    virtual ~Ferramenta() = default;

    virtual void usar(Jardim* jardim, int lin, int col) = 0;
    virtual std::string getTipo() const = 0;
    virtual std::string getDetalhes() const = 0;
    virtual Ferramenta* duplicar() const = 0;

    int getNumSerie() const { return num_serie; }
    char getSimbolo() const { return simbolo; }
    int getQuantidadeAtual() const { return quantidade_atual; }
    int getCapacidadeMaxima() const { return capacidade_maxima; }

    bool estaDesgastada() const { return capacidade_maxima > 0 && quantidade_atual <= 0; }
    void gastar(int qtd);
};

#endif // FERRAMENTA_H
