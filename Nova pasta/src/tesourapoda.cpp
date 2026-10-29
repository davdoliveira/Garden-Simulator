#include "../include/tesourapoda.h"
#include "../include/jardim.h"
#include "../include/celula.h"
#include "../include/planta.h"

TesouraPoda::TesouraPoda() : Ferramenta(0, '<') {}  // 0 = infinito

void TesouraPoda::usar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);
    Planta* p = cel.getPlanta();

    if (p != nullptr && p->eFeia()) {
        cel.removerPlanta();
    }
}