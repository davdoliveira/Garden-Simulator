#include "../include/cacto.h"
#include "../include/celula.h"
#include "../include/Settings.h"
#include <iostream>

Cacto::Cacto() : Planta(0, 0) {
    cont_agua_alta = 0;
    cont_nut_baixo = 0;
    nutrientes_acumulados = 0;
}

bool Cacto::deveMorrer(int agua_solo, int nutrientes_solo) {
    if (agua_solo > Settings::Cacto::morre_agua_solo_maior) {
        cont_agua_alta++;
    } else {
        cont_agua_alta = 0;
    }

    if (nutrientes_solo < Settings::Cacto::morre_nutrientes_solo_menor) {
        cont_nut_baixo++;
    } else {
        cont_nut_baixo = 0;
    }

    return (cont_agua_alta >= Settings::Cacto::morre_agua_solo_instantes) ||
           (cont_nut_baixo > Settings::Cacto::morre_nutrientes_solo_instantes);
}

bool Cacto::tentarMultiplicar(Jardim* jardim, int lin, int col) {
    if (agua_interna <= Settings::Cacto::multiplica_agua_maior ||
        nutrientes_interna <= Settings::Cacto::multiplica_nutrientes_maior) {
        return false;
    }

    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc) &&
            jardim->getCelula(nl, nc).getPlanta() == nullptr) {

            Cacto* novo = new Cacto();
            novo->agua_interna = agua_interna / 2;
            novo->nutrientes_interna = nutrientes_interna / 2;
            novo->nutrientes_acumulados = 0;
            novo->setJaAgiu(true);

            agua_interna /= 2;
            nutrientes_interna /= 2;

            jardim->getCelula(nl, nc).setPlanta(novo);
            std::cout << "Nasceu um novo Cacto em " << jardim->posicaoParaString(nl, nc)
                      << " (multiplicado de " << jardim->posicaoParaString(lin, col) << ")." << std::endl;
            return true;
        }
    }
    return false;
}

void Cacto::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);
    int agua_solo = cel.getAgua();
    int nut_solo = cel.getNutrientes();

    int absorve_agua = (agua_solo * Settings::Cacto::absorcao_agua_percentagem) / 100;
    int max_nut = Settings::Cacto::absorcao_nutrientes;
    int absorve_nut = (max_nut < nut_solo ? max_nut : nut_solo);

    agua_interna += absorve_agua;
    nutrientes_interna += absorve_nut;
    nutrientes_acumulados += absorve_nut;

    cel.setAgua(agua_solo - absorve_agua);
    cel.setNutrientes(nut_solo - absorve_nut);

    if (deveMorrer(cel.getAgua(), cel.getNutrientes())) {
        cel.setNutrientes(cel.getNutrientes() + nutrientes_acumulados);
        std::cout << "O Cacto em " << jardim->posicaoParaString(lin, col) << " morreu." << std::endl;
        cel.removerPlanta();
        return;
    }

    tentarMultiplicar(jardim, lin, col);
    envelhecer();
}
