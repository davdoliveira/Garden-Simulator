#ifndef FERRAMENTAZ_H
#define FERRAMENTAZ_H

#include "ferramenta.h"

class FerramentaZ : public Ferramenta {
public:
    FerramentaZ();
    void usar(Jardim* jardim, int lin, int col) override;
    std::string getTipo() const override { return "FerramentaZ"; }
    std::string getDetalhes() const override;
    Ferramenta* duplicar() const override { return new FerramentaZ(*this); }
};

#endif // FERRAMENTAZ_H
