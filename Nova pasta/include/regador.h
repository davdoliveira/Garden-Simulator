#ifndef REGADOR_H
#define REGADOR_H

#include "ferramenta.h"

class Regador : public Ferramenta {
public:
    Regador();
    void usar(Jardim* jardim, int lin, int col) override;
    std::string getTipo() const override;

    Ferramenta* duplicar() const override { return new Regador(*this); }
};

#endif // REGADOR_H