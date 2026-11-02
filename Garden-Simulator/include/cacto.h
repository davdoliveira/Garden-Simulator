#ifndef CACTO_H
#define CACTO_H

#include "planta.h"
#include "jardim.h"

class Cacto : public Planta {
private:
    int cont_agua_alta = 0;
    int cont_nut_baixo = 0;
    int nutrientes_acumulados = 0;

public:
    Cacto();

    char getSimbolo() const override { return 'c'; }
    std::string getNomeEspecie() const override { return "Cacto"; }
    std::string getBeleza() const override { return "Neutra"; }

    void atualizar(Jardim* jardim, int lin, int col) override;
    bool deveMorrer(int agua_solo, int nutrientes_solo);
    bool tentarMultiplicar(Jardim* jardim, int lin, int col);

    Planta* duplicar() const override { return new Cacto(*this); }
};

#endif // CACTO_H
