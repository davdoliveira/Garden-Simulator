#ifndef PLANTAEXOTICA_H
#define PLANTAEXOTICA_H

#include "planta.h"
#include "jardim.h"

class PlantaExotica : public Planta {
private:
    int contador_ciclo = 0;

public:
    PlantaExotica();

    char getSimbolo() const override { return 'x'; }
    std::string getNomeEspecie() const override { return "Planta Exotica"; }
    std::string getBeleza() const override { return "Bonita"; }

    void atualizar(Jardim* jardim, int lin, int col) override;
    bool tentarInjetarNutrientes(Jardim* jardim, int lin, int col);

    Planta* duplicar() const override { return new PlantaExotica(*this); }
};

#endif // PLANTAEXOTICA_H
