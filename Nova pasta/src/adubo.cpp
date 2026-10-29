#include "../include/adubo.h"
#include "../include/jardim.h"
#include "../include/celula.h"

Adubo::Adubo() : Ferramenta(100, '^') {}

void Adubo::usar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);
    cel.setNutrientes(cel.getNutrientes() + 10);
    reduzirUso();
}