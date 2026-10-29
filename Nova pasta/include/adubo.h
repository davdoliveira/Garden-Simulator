#ifndef ADUBO_H
#define ADUBO_H

#include "ferramenta.h"

class Adubo : public Ferramenta {
public:
    Adubo();
    void usar(Jardim* jardim, int lin, int col) override;
    std::string getTipo() const override { return "Adubo"; }

    Ferramenta* duplicar() const override { return new Adubo(*this); }
};

#endif // ADUBO_H
