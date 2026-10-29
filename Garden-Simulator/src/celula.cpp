#include "../include/celula.h"
#include "../include/planta.h"
#include "../include/ferramenta.h"

Celula::Celula() : agua(0), nutrientes(0), planta(nullptr), ferramenta(nullptr) {}

void Celula::setPlanta(Planta* p) {
    if (planta != nullptr && planta != p) {
        delete planta;
    }
    planta = p;
}

void Celula::setFerramenta(Ferramenta* f) {
    if (ferramenta != nullptr && ferramenta != f) {
        delete ferramenta;
    }
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
    // Prioridade visual na celula: planta > ferramenta > espaço vazio
    if (planta != nullptr) {
        return planta->getSimbolo();
    } else if (ferramenta != nullptr) {
        return ferramenta->getSimbolo();
    } else {
        return ' ';
    }
}

void Celula::esquecerFerramenta() {
    ferramenta = nullptr;
}

Celula::~Celula() {
    delete planta;
    delete ferramenta;
}
