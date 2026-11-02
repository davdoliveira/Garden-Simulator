#include "../include/ferramentaz.h"
#include "../include/jardim.h"
#include "../include/celula.h"
#include "../include/planta.h"
#include <iostream>

FerramentaZ::FerramentaZ() : Ferramenta(3, 'z') {}

void FerramentaZ::usar(Jardim* jardim, int lin, int col) {
    if (estaDesgastada()) return;
    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];

        if (jardim->posicaoValida(nl, nc)) {
            Celula& cel = jardim->getCelula(nl, nc);
            if (cel.getPlanta() != nullptr) {
                std::cout << "FerramentaZ eliminou a planta '" << cel.getPlanta()->getSimbolo()
                          << "' na posicao vizinha " << jardim->posicaoParaString(nl, nc) << "." << std::endl;
                cel.removerPlanta();
            }
        }
    }
    gastar(1);
}

std::string FerramentaZ::getDetalhes() const {
    return "Usos restantes: " + std::to_string(quantidade_atual) + "/" + std::to_string(capacidade_maxima) + " (limpa plantas nas 4 posicoes ortogonais)";
}
