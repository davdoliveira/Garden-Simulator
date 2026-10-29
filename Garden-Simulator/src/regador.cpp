#include "../include/regador.h"
#include "../include/jardim.h"
#include "../include/celula.h"

Regador::Regador() : Ferramenta(200, '>') {}  // 200 usos, símbolo '>'

void Regador::usar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);
    cel.setAgua(cel.getAgua() + 10);  // Aumenta 10 unidades de água
    reduzirUso();                     // Consome 1 uso
}

std::string Regador::getTipo() const {
    return "Regador";
}