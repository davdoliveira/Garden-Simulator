//
// Created by david on 02/11/2025.
//

#include "../include/ervadaninha.h"
#include "../include/celula.h"

ErvaDaninha::ErvaDaninha() : Planta(5, 5) {
    // Inicial: 5 água e 5 nutrientes
    instantes_desde_multiplicacao = 0;
}

bool ErvaDaninha::tentarMultiplicar(Jardim* jardim, int lin, int col) {
    // Condições: nutrientes > 30 e cooldown >= 5
    if (nutrientes_interna <= 30 || instantes_desde_multiplicacao < 5) {
        return false;
    }

    int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (int i = 0; i < 4; ++i) { // Loop C++ clássico
        int nl = lin + dirs[i][0];
        int nc = col + dirs[i][1];
        if (jardim->posicaoValida(nl, nc)) {
            Celula& cel_vizinha = jardim->getCelula(nl, nc);

            // Mata a planta que lá estiver (se houver)
            if (cel_vizinha.getPlanta() != nullptr) {
                delete cel_vizinha.getPlanta();
                cel_vizinha.setPlanta(nullptr);
            }

            // Cria nova erva daninha
            ErvaDaninha* nova = new ErvaDaninha();
            cel_vizinha.setPlanta(nova);

            // Reseta o cooldown
            instantes_desde_multiplicacao = 0;
            return true;
        }
    }
    return false;
}

void ErvaDaninha::atualizar(Jardim* jardim, int lin, int col) {
    Celula& cel = jardim->getCelula(lin, col);

    // 1. Absorve 1 água e 1 nut do solo (se houver)
    if (cel.getAgua() > 0) {
        agua_interna++;
        cel.setAgua(cel.getAgua() - 1);
    }
    if (cel.getNutrientes() > 0) {
        nutrientes_interna++;
        cel.setNutrientes(cel.getNutrientes() - 1);
    }

    // 2. Verificar morte por idade
    if (idade >= 60) {
        cel.removerPlanta();
        delete this;
        return;
    }

    // 3. Tentar multiplicar
    tentarMultiplicar(jardim, lin, col);

    // 4. Atualizar cooldown e idade
    instantes_desde_multiplicacao++;
    envelhecer();
}