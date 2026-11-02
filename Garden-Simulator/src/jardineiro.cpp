#include "../include/jardineiro.h"
#include "../include/ferramenta.h"
#include "../include/jardim.h"
#include "../include/Settings.h"
#include <iostream>
#include <string>

using namespace std;

Jardineiro::Jardineiro(int lin, int col)
    : linha(lin), coluna(col), numFerramentas(0), capacidade(4), ferramenta_na_mao(nullptr) {
    ferramentas = new Ferramenta*[capacidade];
}

Jardineiro::Jardineiro(const Jardineiro& outro) {
    linha = outro.linha;
    coluna = outro.coluna;
    capacidade = outro.capacidade;
    numFerramentas = outro.numFerramentas;
    ferramenta_na_mao = nullptr;

    cont_movimentos = outro.cont_movimentos;
    cont_plantacoes = outro.cont_plantacoes;
    cont_colheitas = outro.cont_colheitas;
    cont_entradas = outro.cont_entradas;
    cont_saidas = outro.cont_saidas;

    ferramentas = new Ferramenta*[capacidade];
    for (int i = 0; i < numFerramentas; i++) {
        ferramentas[i] = outro.ferramentas[i]->duplicar();
        if (outro.ferramenta_na_mao == outro.ferramentas[i]) {
            ferramenta_na_mao = ferramentas[i];
        }
    }
}

Jardineiro::~Jardineiro() {
    for (int i = 0; i < numFerramentas; i++) {
        delete ferramentas[i];
    }
    delete[] ferramentas;
}

void Jardineiro::redimensionar() {
    int novaCapacidade = capacidade * 2;
    Ferramenta** novoArray = new Ferramenta*[novaCapacidade];
    for (int i = 0; i < numFerramentas; i++) {
        novoArray[i] = ferramentas[i];
    }
    delete[] ferramentas;
    ferramentas = novoArray;
    capacidade = novaCapacidade;
}

void Jardineiro::adicionarFerramenta(Ferramenta* f) {
    if (f == nullptr) return;
    if (numFerramentas == capacidade) {
        redimensionar();
    }
    ferramentas[numFerramentas] = f;
    numFerramentas++;
    cout << "Ferramenta adquirida: " << f->getTipo() << " [Serie #" << f->getNumSerie() << "]" << endl;
}

bool Jardineiro::pegarFerramenta(int num_serie) {
    for (int i = 0; i < numFerramentas; i++) {
        if (ferramentas[i]->getNumSerie() == num_serie) {
            ferramenta_na_mao = ferramentas[i];
            cout << "Jardineiro colocou na mao: " << ferramenta_na_mao->getTipo()
                 << " [Serie #" << ferramenta_na_mao->getNumSerie() << "]." << endl;
            return true;
        }
    }
    cout << "Erro: Nao transporta nenhuma ferramenta com o numero de serie " << num_serie << "." << endl;
    return false;
}

void Jardineiro::largarFerramenta() {
    if (ferramenta_na_mao == nullptr) {
        cout << "O jardineiro nao tem nenhuma ferramenta na mao." << endl;
        return;
    }
    cout << "Jardineiro largou a ferramenta " << ferramenta_na_mao->getTipo()
         << " [Serie #" << ferramenta_na_mao->getNumSerie() << "] (guardada na mochila)." << endl;
    ferramenta_na_mao = nullptr;
}

void Jardineiro::aplicarFerramentaNaMao(Jardim* jardim) {
    if (jardim == nullptr || !estaNoJardim() || ferramenta_na_mao == nullptr) return;

    ferramenta_na_mao->usar(jardim, linha, coluna);
    cout << "O jardineiro aplicou " << ferramenta_na_mao->getTipo()
         << " [Serie #" << ferramenta_na_mao->getNumSerie() << "] em "
         << jardim->posicaoParaString(linha, coluna) << "." << endl;

    if (ferramenta_na_mao->estaDesgastada()) {
        cout << "A ferramenta " << ferramenta_na_mao->getTipo()
             << " [Serie #" << ferramenta_na_mao->getNumSerie() << "] esgotou-se e foi descartada!" << endl;
        int idx = -1;
        for (int i = 0; i < numFerramentas; i++) {
            if (ferramentas[i] == ferramenta_na_mao) {
                idx = i;
                break;
            }
        }
        delete ferramenta_na_mao;
        ferramenta_na_mao = nullptr;
        if (idx != -1) {
            for (int i = idx; i < numFerramentas - 1; i++) {
                ferramentas[i] = ferramentas[i + 1];
            }
            numFerramentas--;
        }
    }
}

void Jardineiro::listarInventario() const {
    if (numFerramentas == 0) {
        cout << "O jardineiro nao transporta ferramentas." << endl;
        return;
    }
    cout << "--- Ferramentas do Jardineiro ---" << endl;
    for (int i = 0; i < numFerramentas; i++) {
        Ferramenta* f = ferramentas[i];
        bool naMao = (f == ferramenta_na_mao);
        cout << "[Serie #" << f->getNumSerie() << "] " << f->getTipo()
             << " (" << f->getSimbolo() << ") - " << f->getDetalhes()
             << (naMao ? " [NA MAO]" : " [Mochila]") << endl;
    }
}

bool Jardineiro::mover(char direcao, int maxLin, int maxCol) {
    int novaLin = linha;
    int novaCol = coluna;

    if (direcao == 'c') novaLin--;
    else if (direcao == 'b') novaLin++;
    else if (direcao == 'e') novaCol--;
    else if (direcao == 'd') novaCol++;

    if (novaLin >= 0 && novaLin < maxLin && novaCol >= 0 && novaCol < maxCol) {
        linha = novaLin;
        coluna = novaCol;
        return true;
    } else {
        cout << "Movimento invalido: Fora dos limites do jardim." << endl;
        return false;
    }
}

void Jardineiro::entrar(int l, int c) {
    linha = l;
    coluna = c;
}

bool Jardineiro::podeMover() const { return cont_movimentos < Settings::Jardineiro::max_movimentos; }
bool Jardineiro::podePlantar() const { return cont_plantacoes < Settings::Jardineiro::max_plantacoes; }
bool Jardineiro::podeColher() const { return cont_colheitas < Settings::Jardineiro::max_colheitas; }
bool Jardineiro::podeEntrar() const { return cont_entradas < Settings::Jardineiro::max_entradas_saidas; }
bool Jardineiro::podeSair() const { return cont_saidas < Settings::Jardineiro::max_entradas_saidas; }

void Jardineiro::registarMovimento() { cont_movimentos++; }
void Jardineiro::registarPlantacao() { cont_plantacoes++; }
void Jardineiro::registarColheita() { cont_colheitas++; }
void Jardineiro::registarEntrada() { cont_entradas++; }
void Jardineiro::registarSaida() { cont_saidas++; }

void Jardineiro::sair() {
    linha = -1;
    coluna = -1;
    cout << "O Jardineiro saiu do jardim." << endl;
}

void Jardineiro::descansar() {
    cont_movimentos = 0;
    cont_plantacoes = 0;
    cont_colheitas = 0;
    cont_entradas = 0;
    cont_saidas = 0;
}
