#include "../include/roseira.h"
#include "../include/celula.h"
#include "../include/jardim.h"
#include "../include/Settings.h"
#include <iostream>

Roseira::Roseira() : Planta(Settings::Roseira::inicial_agua, Settings::Roseira::inicial_nutrientes) {
    agua_absorvida_vida = 0;
    nutrientes_absorvidos_vida = 0;
}

bool Roseira::deveMorrer(Jardim* jardim, int lin, int col) {
    if (agua_interna < Settings::Roseira::morre_agua_menor ||
        nutrientes_interna < Settings::Roseira::morre_nutrientes_menor) {
        return true;
    }
    if (nutrientes_interna > Settings::Roseira::morre_nutrientes_maior) {
        return true;
    }
    int vizinhos_validos = 0;
    int vizinhos_ocupados = 0;
    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc)) {
            vizinhos_validos++;
            if (jardim->getCelula(nl, nc).getPlanta() != nullptr) {
                vizinhos_ocupados++;
            }
        }
    }
    return (vizinhos_validos > 0 && vizinhos_ocupados == vizinhos_validos);
}

bool Roseira::tentarMultiplicar(Jardim* jardim, int lin, int col) {
    if (nutrientes_interna <= Settings::Roseira::multiplica_nutrientes_maior) {
        return false;
    }
    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) {
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc) &&
            jardim->getCelula(nl, nc).getPlanta() == nullptr) {
            Roseira* nova = new Roseira();
            nova->agua_interna = (agua_interna * Settings::Roseira::nova_agua_percentagem) / 100;
            nova->nutrientes_interna = Settings::Roseira::nova_nutrientes;
            nova->setJaAgiu(true);

            agua_interna = (agua_interna * Settings::Roseira::original_agua_percentagem) / 100;
            nutrientes_interna = Settings::Roseira::original_nutrientes;

            jardim->getCelula(nl, nc).setPlanta(nova);
            std::cout << "Nasceu uma nova Roseira em " << jardim->posicaoParaString(nl, nc)
                      << " (multiplicada de " << jardim->posicaoParaString(lin, col) << ")." << std::endl;
            return true;
        }
    }
    return false;
}

void Roseira::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);

    int nova_agua = agua_interna - Settings::Roseira::perda_agua;
    agua_interna = (nova_agua > 0 ? nova_agua : 0);
    int novos_nut = nutrientes_interna - Settings::Roseira::perda_nutrientes;
    nutrientes_interna = (novos_nut > 0 ? novos_nut : 0);

    int absorve_agua = (Settings::Roseira::absorcao_agua < cel.getAgua() ? Settings::Roseira::absorcao_agua : cel.getAgua());
    int absorve_nut = (Settings::Roseira::absorcao_nutrientes < cel.getNutrientes() ? Settings::Roseira::absorcao_nutrientes : cel.getNutrientes());

    agua_interna += absorve_agua;
    nutrientes_interna += absorve_nut;
    agua_absorvida_vida += absorve_agua;
    nutrientes_absorvidos_vida += absorve_nut;

    cel.setAgua(cel.getAgua() - absorve_agua);
    cel.setNutrientes(cel.getNutrientes() - absorve_nut);

    if (deveMorrer(jardim, lin, col)) {
        cel.setAgua(cel.getAgua() + agua_absorvida_vida / 2);
        cel.setNutrientes(cel.getNutrientes() + nutrientes_absorvidos_vida / 2);
        std::cout << "A Roseira em " << jardim->posicaoParaString(lin, col) << " morreu." << std::endl;
        cel.removerPlanta();
        return;
    }

    tentarMultiplicar(jardim, lin, col);
    envelhecer();
}
