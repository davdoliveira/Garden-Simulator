#include "../include/ferramentaz.h"
#include "../include/jardim.h"

FerramentaZ::FerramentaZ() : Ferramenta(3, 'z') {}  // 3 usos

void FerramentaZ::usar(Jardim* jardim, int lin, int col) {
    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int* d = dirs[i]; // Aponta para o array da direção atual
        int nl = lin + d[0];
        int nc = col + d[1];

        if (jardim->posicaoValida(nl, nc)) {
            Celula& cel = jardim->getCelula(nl, nc);
            if (cel.getPlanta() != nullptr) {
                cel.removerPlanta();
            }
        }
    }
    reduzirUso();
}