#include "../include/jardineiro.h"
#include "../include/ferramenta.h"
#include "../include/Settings.h"
#include <iostream>
#include <string>

using namespace std;

Jardineiro::Jardineiro(int lin, int col) : linha(lin), coluna(col), capacidade(2), numFerramentas(0) {
    ferramentas = new Ferramenta*[capacidade];
}

// --- NOVO: Implementação do Construtor de Cópia ---
Jardineiro::Jardineiro(const Jardineiro& outro) {
    linha = outro.linha;
    coluna = outro.coluna;
    capacidade = outro.capacidade;
    numFerramentas = outro.numFerramentas;

    // Copiar contadores
    cont_movimentos = outro.cont_movimentos;
    cont_plantacoes = outro.cont_plantacoes;
    cont_colheitas = outro.cont_colheitas;
    cont_entradas_saidas = outro.cont_entradas_saidas;

    // Deep Copy das ferramentas
    ferramentas = new Ferramenta*[capacidade];
    for (int i = 0; i < numFerramentas; i++) {
        // Usa o método duplicar() que adicionaste às ferramentas
        ferramentas[i] = outro.ferramentas[i]->duplicar();
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
    if (numFerramentas == capacidade) {
        redimensionar();
    }
    ferramentas[numFerramentas] = f;
    numFerramentas++;
    cout << "Jardineiro apanhou a ferramenta: " << f->getSimbolo() << endl;
}

void Jardineiro::usarFerramenta(char tipo, Jardim* jardim) {
    if (jardim == nullptr) return;

    string nomeProcurado = "";
    if (tipo == 'g') nomeProcurado = "Regador";
    else if (tipo == 'a') nomeProcurado = "Adubo";
    else if (tipo == 't') nomeProcurado = "TesouraPoda";
    else if (tipo == 'z') nomeProcurado = "FerramentaZ";
    else {
        cout << "Erro: Tipo de ferramenta '" << tipo << "' desconhecido." << endl;
        return;
    }

    int indiceEncontrado = -1;
    for (int i = 0; i < numFerramentas; i++) {
        if (ferramentas[i]->getTipo() == nomeProcurado) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        cout << "Erro: O jardineiro nao tem nenhum " << nomeProcurado << " no inventario." << endl;
        return;
    }

    Ferramenta* f = ferramentas[indiceEncontrado];
    f->usar(jardim, linha, coluna);
    cout << "O jardineiro usou " << nomeProcurado << "." << endl;

    if (f->getUsosRestantes() == 0 && nomeProcurado != "TesouraPoda") {
        cout << "A ferramenta " << nomeProcurado << " partiu-se e foi deitada fora!" << endl;
        delete f;
        for (int i = indiceEncontrado; i < numFerramentas - 1; i++) {
            ferramentas[i] = ferramentas[i + 1];
        }
        numFerramentas--;
    }
}

void Jardineiro::listarInventario() const {
    if (numFerramentas == 0) {
        cout << "O jardineiro nao tem ferramentas." << endl;
        return;
    }
    cout << "--- Inventario do Jardineiro ---" << endl;
    for (int i = 0; i < numFerramentas; i++) {
        Ferramenta* f = ferramentas[i];
        cout << i+1 << ". " << f->getTipo()
             << " (Usos: " << f->getUsosRestantes() << ")" << endl;
    }
}

void Jardineiro::mover(char direcao, int maxLin, int maxCol) {
    int novaLin = linha;
    int novaCol = coluna;

    if (direcao == 'c') novaLin--;
    else if (direcao == 'b') novaLin++;
    else if (direcao == 'e') novaCol--;
    else if (direcao == 'd') novaCol++;

    if (novaLin >= 0 && novaLin < maxLin && novaCol >= 0 && novaCol < maxCol) {
        linha = novaLin;
        coluna = novaCol;
    } else {
        cout << "Movimento invalido: Fora dos limites." << endl;
    }
}

void Jardineiro::entrar(int l, int c) {
    linha = l;
    coluna = c;
}

bool Jardineiro::temFerramenta(const std::string& tipo) const {
    return false;
}

bool Jardineiro::podeMover() const { return cont_movimentos < Settings::Jardineiro::max_movimentos; }
bool Jardineiro::podePlantar() const { return cont_plantacoes < Settings::Jardineiro::max_plantacoes; }
bool Jardineiro::podeColher() const { return cont_colheitas < Settings::Jardineiro::max_colheitas; }
bool Jardineiro::podeEntrarOuSair() const { return cont_entradas_saidas < Settings::Jardineiro::max_entradas_saidas; }

void Jardineiro::registarMovimento() { cont_movimentos++; }
void Jardineiro::registarPlantacao() { cont_plantacoes++; }
void Jardineiro::registarColheita() { cont_colheitas++; }
void Jardineiro::registarEntradaSaida() { cont_entradas_saidas++; }

void Jardineiro::sair() {
    linha = -1;
    coluna = -1;
    cout << "O Jardineiro saiu do jardim." << endl;
}

void Jardineiro::descansar() {
    cont_movimentos = 0;
    cont_plantacoes = 0;
    cont_colheitas = 0;
    cont_entradas_saidas = 0;
    cout << "O Jardineiro descansou. Energias e limites repostos!" << endl;
}