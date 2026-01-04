//
// Created by david on 01/11/2025.
//

#ifndef ROSEIRA_H
#define ROSEIRA_H

#include "planta.h"
#include "Settings.h"

class Jardim;
class Roseira : public Planta {
private:
    int cont_agua_alta = 0;     // Contador: quantos instantes água > 100
    int cont_nut_baixo = 0;     // Contador: quantos instantes nutrientes == 0

public:
    Roseira();

    // Funções obrigatórias da classe base (atualizar deve ter os parâmetros para acesso ao Jardim)
    char getSimbolo() const override { return 'r'; }
    // O seu Roseira.cpp usa Roseira::atualizar(Jardim* jardim, int lin, int col), ajustando a assinatura aqui.
    void atualizar(Jardim* jardim, int lin, int col) override;

    // Função auxiliar para verificar se deve morrer
    bool deveMorrer(Jardim* jardim, int lin, int col);

    // Função para tentar multiplicar
    bool tentarMultiplicar(Jardim* jardim, int lin, int col);

    Planta* duplicar() const override { return new Roseira(*this); }
};

#endif // ROSEIRA_H