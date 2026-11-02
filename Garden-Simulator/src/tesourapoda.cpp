#include "../include/tesourapoda.h"
#include "../include/jardim.h"
#include "../include/celula.h"
#include "../include/planta.h"
#include <iostream>

TesouraPoda::TesouraPoda() : Ferramenta(0, 't') {}

void TesouraPoda::usar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);
    Planta* p = cel.getPlanta();

    if (p != nullptr && p->eFeia()) {
        std::cout << "A Tesoura de Poda eliminou a planta feia '" << p->getSimbolo()
                  << "' em " << jardim->posicaoParaString(lin, col) << "." << std::endl;
        cel.removerPlanta();
    }
}
