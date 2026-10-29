#include "../include/adubo.h"
#include "../include/jardim.h"
#include "../include/celula.h"
#include "../include/Settings.h"

Adubo::Adubo() : Ferramenta(Settings::Adubo::capacidade, 'a') {}

void Adubo::usar(Jardim* jardim, int lin, int col) {
    if (estaDesgastada()) return;
    Celula& cel = jardim->getCelula(lin, col);
    int dose = (quantidade_atual >= Settings::Adubo::dose) ? Settings::Adubo::dose : quantidade_atual;
    cel.setNutrientes(cel.getNutrientes() + dose);
    gastar(dose);
}

std::string Adubo::getDetalhes() const {
    return "Nutrientes: " + std::to_string(quantidade_atual) + "/" + std::to_string(capacidade_maxima);
}
