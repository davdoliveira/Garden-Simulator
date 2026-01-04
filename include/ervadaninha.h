#ifndef ERVADANINHA_H
#define ERVADANINHA_H

#include "planta.h"
#include "jardim.h"

class ErvaDaninha : public Planta {
private:
    int instantes_desde_multiplicacao = 0;  // Contador para cooldown

public:
    ErvaDaninha();  // Inicializa com 5 água e 5 nutrientes

    char getSimbolo() const override { return 'e'; }

    // Comportamento por instante
    void atualizar(Jardim* jardim, int lin, int col) override;

    // Tenta multiplicar, matando a planta vizinha se necessário
    bool tentarMultiplicar(Jardim* jardim, int lin, int col);

    bool eFeia() const override { return true; }

    Planta* duplicar() const override { return new ErvaDaninha(*this); }
};

#endif // ERVADANINHA_H