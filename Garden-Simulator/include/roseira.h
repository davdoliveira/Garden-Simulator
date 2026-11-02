#ifndef ROSEIRA_H
#define ROSEIRA_H

#include "planta.h"
#include "Settings.h"

class Jardim;

class Roseira : public Planta {
private:
    int agua_absorvida_vida = 0;
    int nutrientes_absorvidos_vida = 0;

public:
    Roseira();

    char getSimbolo() const override { return 'r'; }
    std::string getNomeEspecie() const override { return "Roseira"; }
    std::string getBeleza() const override { return "Bonita"; }

    void atualizar(Jardim* jardim, int lin, int col) override;
    bool deveMorrer(Jardim* jardim, int lin, int col);
    bool tentarMultiplicar(Jardim* jardim, int lin, int col);

    Planta* duplicar() const override { return new Roseira(*this); }
};

#endif // ROSEIRA_H
