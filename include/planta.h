#ifndef PLANTA_H
#define PLANTA_H
#include "Settings.h"

class Jardim; // Necessário para a assinatura de atualizar

class Planta {
protected:
    int agua_interna;
    int nutrientes_interna;
    int idade;

public:
    Planta(int agua_inicial, int nut_inicial);
    virtual ~Planta () {};

    virtual char getSimbolo() const = 0;
    virtual void atualizar(Jardim* jardim, int lin, int col) = 0;

    virtual Planta* duplicar() const = 0;

    // Getters...
    int getAguaInterna() const { return agua_interna; }
    int getNutrientesInterna() const { return nutrientes_interna; }
    int getIdade() const { return idade; }

    void envelhecer() { idade++; }
    virtual bool eFeia() const { return false; }
};

#endif // PLANTA_H