#ifndef PLANTA_H
#define PLANTA_H

#include <string>
#include "Settings.h"

class Jardim;

class Planta {
protected:
    int agua_interna;
    int nutrientes_interna;
    int idade;
    bool ja_agiu_neste_instante;

public:
    Planta(int agua_inicial, int nut_inicial);
    virtual ~Planta() = default;

    virtual char getSimbolo() const = 0;
    virtual std::string getNomeEspecie() const = 0;
    virtual std::string getBeleza() const = 0;
    virtual void atualizar(Jardim* jardim, int lin, int col) = 0;
    virtual Planta* duplicar() const = 0;

    int getAguaInterna() const { return agua_interna; }
    int getNutrientesInterna() const { return nutrientes_interna; }
    int getIdade() const { return idade; }

    bool getJaAgiu() const { return ja_agiu_neste_instante; }
    void setJaAgiu(bool v) { ja_agiu_neste_instante = v; }

    void envelhecer() { idade++; }
    virtual bool eFeia() const { return getBeleza() == "Feia"; }
};

#endif // PLANTA_H
