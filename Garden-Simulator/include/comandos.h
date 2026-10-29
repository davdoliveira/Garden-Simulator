#ifndef COMANDOS_H
#define COMANDOS_H

#include <string>
#include <map>
#include "jardim.h"

struct Coordenadas {
    int linha = -1;
    int coluna = -1;
    bool valida = false;
};

class Comando {
private:
    Jardim* jardim = nullptr;
    std::map<std::string, Jardim*> saves;

    Coordenadas stringParaPosicao(const std::string& pos) const;
    bool lerInteiroPositivo(const std::string& str, int& valor, bool permitirZero = false) const;

public:
    Comando() = default;
    ~Comando();

    bool processar(const std::string& comando);
};

#endif // COMANDOS_H
