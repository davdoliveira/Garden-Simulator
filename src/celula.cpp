//
// Created by david on 17/10/2025.
//

#include "../include/celula.h"
#include "../include/planta.h"
#include "../include/ferramenta.h"

Celula::Celula() : agua(0), nutrientes(0), planta(nullptr), ferramenta(nullptr) {}

void Celula::setPlanta(Planta* p) {
    if (planta != nullptr) delete planta;  // Libera planta antiga
    planta = p;
}

void Celula::setFerramenta(Ferramenta* f) {
    if (ferramenta != nullptr) delete ferramenta;
    ferramenta = f;
}

void Celula::removerPlanta() {
    if (planta != nullptr) {
        delete planta;
        planta = nullptr;
    }
}

void Celula::removerFerramenta() {
    if (ferramenta != nullptr) {
        delete ferramenta;
        ferramenta = nullptr;
    }
}

char Celula::getSimbolo() const {
    // Prioridade: jardineiro > planta > ferramenta > .
    if (ferramenta != nullptr) {
        return ferramenta->getSimbolo();
    } else if (planta != nullptr) {
        return planta->getSimbolo();
    } else {
        return '.';
    }
}

void Celula::esquecerFerramenta() {
    ferramenta = nullptr; // Apenas coloca a null, NÃO faz delete!
}

Celula::~Celula() {
    delete planta;
    delete ferramenta;
}