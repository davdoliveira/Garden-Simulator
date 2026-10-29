#include "../include/ferramenta.h"

Ferramenta::Ferramenta(int max_usos, char simb)
    : usos_maximos(max_usos), usos_restantes(max_usos), simbolo(simb) {}

void Ferramenta::reduzirUso() {
    if (usos_maximos > 0) {  // Só reduz se tiver limite (0 = infinito)
        usos_restantes--;
    }
}