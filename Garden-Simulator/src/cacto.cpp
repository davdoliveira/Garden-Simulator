#include "../include/cacto.h"
#include "../include/celula.h"
#include <cstdlib>

Cacto::Cacto() : Planta(0, 0) {
    // Valores internos começam em 0
    cont_agua_alta = 0;
    cont_nut_baixo = 0;
    nutrientes_acumulados = 0;
}

bool Cacto::deveMorrer(int agua_solo, int nutrientes_solo) {
    // Resetar contadores se condição não for atendida
    if (agua_solo <= 100) {
        cont_agua_alta = 0;
    } else {
        cont_agua_alta++;
    }

    if (nutrientes_solo > 0) {
        cont_nut_baixo = 0;
    } else {
        cont_nut_baixo++;
    }

    // Morre se: água > 100 por 3 instantes OU nutrientes == 0 por >3 instantes
    return (cont_agua_alta >= 3) || (cont_nut_baixo > 3);
}

bool Cacto::tentarMultiplicar(Jardim* jardim, int lin, int col) {
    if (agua_interna <= 50 || nutrientes_interna <= 100) return false;

    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) { // Loop C++ clássico
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc) &&
            jardim->getCelula(nl, nc).getPlanta() == nullptr) {

            // novo cacto
            Cacto* novo = new Cacto();
            novo->agua_interna = agua_interna / 2;
            novo->nutrientes_interna = nutrientes_interna / 2;
            novo->nutrientes_acumulados = 0;  // novo começa do zero

            // Dividir igualmente
            agua_interna /= 2;
            nutrientes_interna /= 2;

            jardim->getCelula(nl, nc).setPlanta(novo);
            return true;
        }
    }
    return false;
}

void Cacto::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);
    int agua_solo = cel.getAgua();
    int nut_solo = cel.getNutrientes();

    // 1. Absorção: 25% da água + até 5 nutrientes
    int absorve_agua = (agua_solo * 25) / 100;

    // CORRIGIDO: Substituído std::min
    int absorve_nut = (5 < nut_solo ? 5 : nut_solo);

    agua_interna += absorve_agua;
    nutrientes_interna += absorve_nut;
    nutrientes_acumulados += absorve_nut;  // acumula durante a vida

    cel.setAgua(agua_solo - absorve_agua);
    cel.setNutrientes(nut_solo - absorve_nut);

    // 2. Verificar morte
    if (deveMorrer(cel.getAgua(), cel.getNutrientes())) {
        // Deixa TODOS os nutrientes acumulados no solo
        cel.setNutrientes(cel.getNutrientes() + nutrientes_acumulados);
        cel.removerPlanta();
        delete this;
        return;
    }

    // 3. Tentar multiplicar
    tentarMultiplicar(jardim, lin, col);

    // 4. Envelhecer
    envelhecer();
}