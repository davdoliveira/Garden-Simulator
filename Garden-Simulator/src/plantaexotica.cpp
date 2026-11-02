#include "../include/plantaexotica.h"
#include "../include/celula.h"
#include <iostream>

PlantaExotica::PlantaExotica() : Planta(20, 20) {
    contador_ciclo = 0;
}

bool PlantaExotica::tentarInjetarNutrientes(Jardim* jardim, int lin, int col) {
    if (contador_ciclo != 4) return false;
    if (agua_interna < 10) return false;

    agua_interna -= 10;

    Celula& cel = jardim->getCelula(lin, col);
    cel.setNutrientes(cel.getNutrientes() + 15);
    std::cout << "A Planta Exotica em " << jardim->posicaoParaString(lin, col)
              << " fertilizou o solo (+15 nutrientes)." << std::endl;
    return true;
}

void PlantaExotica::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);

    tentarInjetarNutrientes(jardim, lin, col);

    if (agua_interna <= 0 || nutrientes_interna <= 0) {
        std::cout << "A Planta Exotica em " << jardim->posicaoParaString(lin, col) << " morreu." << std::endl;
        cel.removerPlanta();
        return;
    }

    contador_ciclo = (contador_ciclo + 1) % 5;
    envelhecer();
}
