#include "../include/ferramenta.h"

int Ferramenta::proximo_num_serie = 1;

Ferramenta::Ferramenta(int cap_max, char simb)
    : num_serie(proximo_num_serie++),
      capacidade_maxima(cap_max),
      quantidade_atual(cap_max),
      simbolo(simb) {}

void Ferramenta::gastar(int qtd) {
    if (capacidade_maxima > 0) {
        quantidade_atual -= qtd;
        if (quantidade_atual < 0) quantidade_atual = 0;
    }
}
