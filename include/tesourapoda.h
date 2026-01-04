#ifndef TESOURAPODA_H
#define TESOURAPODA_H

#include "ferramenta.h"

class TesouraPoda : public Ferramenta {
public:
    TesouraPoda();
    void usar(Jardim* jardim, int lin, int col) override;
    std::string getTipo() const override { return "TesouraPoda"; }

    Ferramenta* duplicar() const override { return new TesouraPoda(*this); }
};

#endif // TESOURAPODA_H