//
// Created by david on 01/11/2025.
//

#ifndef CACTO_H
#define CACTO_H

#include "planta.h"
#include "jardim.h"

class Cacto : public Planta {
private:
    int cont_agua_alta = 0;     // Contador: instantes com água > 100 no solo
    int cont_nut_baixo = 0;     // Contador: instantes com nutrientes = 0 no solo
    int nutrientes_acumulados = 0;  // Total absorvido durante a vida

public:
    Cacto();  // Inicializa com 0 água e 0 nutrientes internos

    char getSimbolo() const override { return 'c'; }

    // Comportamento por instante
    void atualizar(Jardim* jardim, int lin, int col) override;

    // Verifica se deve morrer
    bool deveMorrer(int agua_solo, int nutrientes_solo);

    // Tenta multiplicar com divisão igual
    bool tentarMultiplicar(Jardim* jardim, int lin, int col);

    Planta* duplicar() const override { return new Cacto(*this); }
};

#endif // CACTO_H