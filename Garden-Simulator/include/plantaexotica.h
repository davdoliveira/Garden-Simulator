//
// Created by david on 02/11/2025.
//

#ifndef PLANTAEXOTICA_H
#define PLANTAEXOTICA_H

#include "planta.h"
#include "jardim.h"

class PlantaExotica : public Planta {
private:
    int contador_ciclo = 0;  // Conta instantes para ciclo de 5

public:
    PlantaExotica();  // Inicializa com 20 água e 20 nutrientes

    char getSimbolo() const override { return 'x'; }

    // Comportamento por instante
    void atualizar(Jardim* jardim, int lin, int col) override;

    // Tenta injetar nutrientes (ciclo de 5)
    bool tentarInjetarNutrientes(Jardim* jardim, int lin, int col);

    Planta* duplicar() const override { return new PlantaExotica(*this); }
};

#endif // PLANTAEXOTICA_H
