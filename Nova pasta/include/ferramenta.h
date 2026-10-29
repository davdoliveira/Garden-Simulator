#ifndef FERRAMENTA_H
#define FERRAMENTA_H

#include <string>

class Jardim;  // Forward declaration

class Ferramenta {
protected:
    int usos_maximos;       // 0 = infinito (ex: Tesoura)
    int usos_restantes;     // Contador de usos restantes
    char simbolo;           // Símbolo visual no jardim

public:
    Ferramenta(int max_usos, char simb);

    virtual ~Ferramenta() = default;

    virtual void usar(Jardim* jardim, int lin, int col) = 0;
    virtual std::string getTipo() const = 0;

    virtual Ferramenta* duplicar() const = 0;

    char getSimbolo() const { return simbolo; }
    bool estaDesgastada() const { return usos_maximos > 0 && usos_restantes <= 0; }

    void reduzirUso();  // Só reduz se não for infinita
    int getUsosRestantes() const { return usos_restantes; }
};

#endif // FERRAMENTA_H