//
// Created by david on 02/11/2025.
//

#include "../include/plantaexotica.h"
#include "../include/celula.h"

PlantaExotica::PlantaExotica() : Planta(20, 20) {
    contador_ciclo = 0;
}

bool PlantaExotica::tentarInjetarNutrientes(Jardim* jardim, int lin, int col) {
    // Só age a cada 5 instantes
    if (contador_ciclo != 4) return false;

    // Custo: 10 água interna
    if (agua_interna < 10) return false;

    // Paga o custo
    agua_interna -= 10;

    // Injeta 15 nutrientes no solo
    Celula& cel = jardim->getCelula(lin, col);
    cel.setNutrientes(cel.getNutrientes() + 15);

    return true;
}

void PlantaExotica::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);

    // 1. Tentar injetar nutrientes (a cada 5 instantes)
    tentarInjetarNutrientes(jardim, lin, col);

    // 2. Verificar morte
    if (agua_interna <= 0 || nutrientes_interna <= 0) {
        cel.removerPlanta();
        delete this;
        return;
    }

    // 3. Atualizar contador de ciclo
    contador_ciclo = (contador_ciclo + 1) % 5;

    // 4. Envelhecer
    envelhecer();
}
