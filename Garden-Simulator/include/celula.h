//
// Created by david on 17/10/2025.
//

#ifndef CELULA_H
#define CELULA_H

#include "Settings.h"

class Planta;
class Ferramenta;

class Celula {
private:
    int agua;
    int nutrientes;
    Planta* planta;
    Ferramenta* ferramenta;

public:
    Celula();

    int getAgua() const { return agua; }
    int getNutrientes() const { return nutrientes; }
    void setAgua(int a) { agua = a; }
    void setNutrientes(int n) { nutrientes = n; }

    Planta* getPlanta() const { return planta; }
    Ferramenta* getFerramenta() const { return ferramenta; }

    void setPlanta(Planta* p);
    void setFerramenta(Ferramenta* f);

    void removerPlanta();
    void removerFerramenta();
    void esquecerFerramenta();
    char getSimbolo() const;

    ~Celula();
};


#endif //CELULA_H
