#include "../include/roseira.h"
#include "../include/celula.h"
#include "../include/jardim.h"
#include <cstdlib>

Roseira::Roseira() : Planta(25, 25) {
}

bool Roseira::deveMorrer(Jardim* jardim, int lin, int col) {
    if (agua_interna <= 0 || nutrientes_interna <= 0) return true;
    if (nutrientes_interna >= 200) return true;
    int vizinhos_ocupados = 0;
    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc) &&
            jardim->getCelula(nl, nc).getPlanta() != nullptr) {
            vizinhos_ocupados++;
        }
    }
    return (vizinhos_ocupados == 4);
}

bool Roseira::tentarMultiplicar(Jardim* jardim, int lin, int col) {
    if (nutrientes_interna <= 100) return false;
    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc) &&
            jardim->getCelula(nl, nc).getPlanta() == nullptr) {
            Roseira* nova = new Roseira();
            nova->agua_interna = agua_interna / 2;
            nova->nutrientes_interna = Settings::Roseira::nova_nutrientes;
            agua_interna = agua_interna / 2;
            nutrientes_interna = Settings::Roseira::original_nutrientes;
            jardim->getCelula(nl, nc).setPlanta(nova);
            return true;
        }
    }
    return false;
}

void Roseira::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);

    // 1. Perde 4 água e 4 nutrientes
    int nova_agua = agua_interna - Settings::Roseira::perda_agua;
    agua_interna = (nova_agua > 0 ? nova_agua : 0);
    int novos_nut = nutrientes_interna - Settings::Roseira::perda_nutrientes;
    nutrientes_interna = (novos_nut > 0 ? novos_nut : 0);


    // 2. Absorve 5 água e 8 nutrientes do solo (se houver)
    int absorve_agua = (Settings::Roseira::absorcao_agua < cel.getAgua() ? Settings::Roseira::absorcao_agua : cel.getAgua());
    int absorve_nut = (Settings::Roseira::absorcao_nutrientes < cel.getNutrientes() ? Settings::Roseira::absorcao_nutrientes : cel.getNutrientes());

    agua_interna += absorve_agua;
    nutrientes_interna += absorve_nut;
    cel.setAgua(cel.getAgua() - absorve_agua);
    cel.setNutrientes(cel.getNutrientes() - absorve_nut);

    // 3. Verificar morte
    if (deveMorrer(jardim, lin, col)) {
        cel.setAgua(cel.getAgua() + agua_interna / 2);
        cel.setNutrientes(cel.getNutrientes() + nutrientes_interna / 2);
        cel.removerPlanta();
        delete this;
        return;
    }

    // 4. Tentar multiplicar
    tentarMultiplicar(jardim, lin, col);

    // 5. Envelhecer
    envelhecer();
}