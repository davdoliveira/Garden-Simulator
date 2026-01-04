#ifndef COMANDOS_H
#define COMANDOS_H

#include <string>
#include <map> // <--- NECESSÁRIO PARA O MAPA DE SAVES
#include "jardim.h"

struct Coordenadas {
    int linha = -1;
    int coluna = -1;
    bool valida = false;
};

class Comando {
private:
    Jardim* jardim = nullptr;

    // --- NOVO: Mapa de Saves (Nome -> Ponteiro para Jardim) ---
    std::map<std::string, Jardim*> saves;

    Coordenadas stringParaPosicao(const std::string& pos) const;
public:
    Comando() = default;
    ~Comando();

    bool processar(const std::string& comando);
};

#endif //COMANDOS_H