#include "../include/regador.h"
#include "../include/jardim.h"
#include "../include/celula.h"
#include "../include/Settings.h"

Regador::Regador() : Ferramenta(Settings::Regador::capacidade, 'g') {}

void Regador::usar(Jardim* jardim, int lin, int col) {
    if (estaDesgastada()) return;
    Celula& cel = jardim->getCelula(lin, col);
    int dose = (quantidade_atual >= Settings::Regador::dose) ? Settings::Regador::dose : quantidade_atual;
    cel.setAgua(cel.getAgua() + dose);
    gastar(dose);
}

std::string Regador::getTipo() const {
    return "Regador";
}

std::string Regador::getDetalhes() const {
    return "Agua: " + std::to_string(quantidade_atual) + "/" + std::to_string(capacidade_maxima);
}
