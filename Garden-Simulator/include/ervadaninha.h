#ifndef ERVADANINHA_H
#define ERVADANINHA_H

#include "planta.h"
#include "jardim.h"

class ErvaDaninha : public Planta {
private:
    int instantes_desde_multiplicacao = 0;

public:
    ErvaDaninha();

    char getSimbolo() const override { return 'e'; }
    std::string getNomeEspecie() const override { return "Erva Daninha"; }
    std::string getBeleza() const override { return "Feia"; }

    void atualizar(Jardim* jardim, int lin, int col) override;
    bool tentarMultiplicar(Jardim* jardim, int lin, int col);
    bool eFeia() const override { return true; }

    Planta* duplicar() const override { return new ErvaDaninha(*this); }
};

#endif // ERVADANINHA_H
