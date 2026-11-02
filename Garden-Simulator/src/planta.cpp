#include "../include/planta.h"

Planta::Planta(int agua_inicial, int nut_inicial)
    : agua_interna(agua_inicial),
      nutrientes_interna(nut_inicial),
      idade(0),
      ja_agiu_neste_instante(false) {}
