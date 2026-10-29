#ifndef ADUBO_H
#define ADUBO_H

#include "ferramenta.h"

class Adubo : public Ferramenta {
public:
    Adubo();
    void usar(Jardim* jardim, int lin, int col) override;
    std::string getTipo() const override { return "Pacote de Adubo"; }
    std::string getDetalhes() const override;
    Ferramenta* duplicar() const override { return new Adubo(*this); }
};

#endif // ADUBO_H
