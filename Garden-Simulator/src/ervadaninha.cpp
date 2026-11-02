#include "../include/ervadaninha.h"
#include "../include/celula.h"
#include "../include/Settings.h"
#include <iostream>

ErvaDaninha::ErvaDaninha() : Planta(Settings::ErvaDaninha::inicial_agua, Settings::ErvaDaninha::inicial_nutrientes) {
    instantes_desde_multiplicacao = 0;
}

bool ErvaDaninha::tentarMultiplicar(Jardim* jardim, int lin, int col) {
    if (nutrientes_interna <= Settings::ErvaDaninha::multiplica_nutrientes_maior ||
        instantes_desde_multiplicacao < Settings::ErvaDaninha::multiplica_instantes) {
        return false;
    }

    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc)) {
            Celula& cel_vizinha = jardim->getCelula(nl, nc);

            if (cel_vizinha.getPlanta() != nullptr) {
                std::cout << "A Erva Daninha em " << jardim->posicaoParaString(lin, col)
                          << " sufocou a planta em " << jardim->posicaoParaString(nl, nc) << "!" << std::endl;
                cel_vizinha.removerPlanta();
            }

            ErvaDaninha* nova = new ErvaDaninha();
            nova->agua_interna = Settings::ErvaDaninha::inicial_agua;
            nova->nutrientes_interna = Settings::ErvaDaninha::nova_nutrientes;
            nova->setJaAgiu(true);

            cel_vizinha.setPlanta(nova);
            instantes_desde_multiplicacao = 0;

            std::cout << "Nasceu uma nova Erva Daninha em " << jardim->posicaoParaString(nl, nc)
                      << " (multiplicada de " << jardim->posicaoParaString(lin, col) << ")." << std::endl;
            return true;
        }
    }
    return false;
}

void ErvaDaninha::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);

    int absorve_agua = (Settings::ErvaDaninha::absorcao_agua < cel.getAgua() ? Settings::ErvaDaninha::absorcao_agua : cel.getAgua());
    int absorve_nut = (Settings::ErvaDaninha::absorcao_nutrientes < cel.getNutrientes() ? Settings::ErvaDaninha::absorcao_nutrientes : cel.getNutrientes());

    agua_interna += absorve_agua;
    nutrientes_interna += absorve_nut;
    cel.setAgua(cel.getAgua() - absorve_agua);
    cel.setNutrientes(cel.getNutrientes() - absorve_nut);

    envelhecer();
    instantes_desde_multiplicacao++;

    if (idade >= Settings::ErvaDaninha::morre_instantes) {
        std::cout << "A Erva Daninha em " << jardim->posicaoParaString(lin, col) << " morreu de velhice." << std::endl;
        cel.removerPlanta();
        return;
    }

    tentarMultiplicar(jardim, lin, col);
}
