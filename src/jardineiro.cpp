#include "../include/jardineiro.h"
#include "../include/ferramenta.h"
#include "../include/Settings.h"
#include <iostream>
#include <string>

using namespace std;

Jardineiro::Jardineiro(int lin, int col) : linha(lin), coluna(col), capacidade(2), numFerramentas(0) {
    // Começa com capacidade para 2 ferramentas (exemplo)
    ferramentas = new Ferramenta*[capacidade];
}

Jardineiro::~Jardineiro() {
    for (int i = 0; i < numFerramentas; i++) {
        delete ferramentas[i];
    }
    delete[] ferramentas;
}

// Função auxiliar para aumentar o array se estiver cheio
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

    // 1. Converter o char do comando ('g') para o Nome da Classe ("Regador")
    string nomeProcurado = "";
    if (tipo == 'g') nomeProcurado = "Regador";
    else if (tipo == 'a') nomeProcurado = "Adubo";
    else if (tipo == 't') nomeProcurado = "TesouraPoda";
    else if (tipo == 'z') nomeProcurado = "FerramentaZ";
    else {
        cout << "Erro: Tipo de ferramenta '" << tipo << "' desconhecido." << endl;
        return;
    }

    // 2. Procurar no inventário
    int indiceEncontrado = -1;
    for (int i = 0; i < numFerramentas; i++) {
        if (ferramentas[i]->getTipo() == nomeProcurado) {
            indiceEncontrado = i;
            break; // Encontrou a primeira disponível desse tipo
        }
    }

    if (indiceEncontrado == -1) {
        cout << "Erro: O jardineiro nao tem nenhum " << nomeProcurado << " no inventario." << endl;
        return;
    }

    // 3. Usar a ferramenta
    Ferramenta* f = ferramentas[indiceEncontrado];
    f->usar(jardim, linha, coluna); // Ação real (regar, adubar, etc.)

    cout << "O jardineiro usou " << nomeProcurado << "." << endl;

    // 4. Verificar se partiu (Gestão de Memória)
    // Se a ferramenta for TesouraPoda, os usos são 0 (infinito), nunca entra aqui
    if (f->getUsosRestantes() == 0 && nomeProcurado != "TesouraPoda") {
        cout << "A ferramenta " << nomeProcurado << " partiu-se e foi deitada fora!" << endl;

        delete f; // Liberta a memória da ferramenta

        // Remover do array (Shift Left)
        for (int i = indiceEncontrado; i < numFerramentas - 1; i++) {
            ferramentas[i] = ferramentas[i + 1];
        }
        numFerramentas--;
    }
}

// LÓGICA DE MOVIMENTO
void Jardineiro::mover(char direcao, int maxLin, int maxCol) {
    int novaLin = linha;
    int novaCol = coluna;

    if (direcao == 'c') novaLin--;
    else if (direcao == 'b') novaLin++;
    else if (direcao == 'e') novaCol--;
    else if (direcao == 'd') novaCol++;

    // Verifica se está dentro do jardim
    if (novaLin >= 0 && novaLin < maxLin && novaCol >= 0 && novaCol < maxCol) {
        linha = novaLin;
        coluna = novaCol;
    } else {
        cout << "Movimento invalido: Fora dos limites." << endl;
    }
}

// Outros métodos (se necessário manter compatibilidade com o .h)
void Jardineiro::entrar(int l, int c) { linha = l; coluna = c; }
bool Jardineiro::temFerramenta(const std::string& tipo) const { return false; }

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

bool Jardineiro::podeMover() const {
    return cont_movimentos < Settings::Jardineiro::max_movimentos; // Geralmente 10
}

bool Jardineiro::podePlantar() const {
    return cont_plantacoes < Settings::Jardineiro::max_plantacoes; // Geralmente 2
}

bool Jardineiro::podeColher() const {
    return cont_colheitas < Settings::Jardineiro::max_colheitas; // Geralmente 5
}

void Jardineiro::registarMovimento() {
    cont_movimentos++;
}

void Jardineiro::registarPlantacao() {
    cont_plantacoes++;
}

void Jardineiro::registarColheita() {
    cont_colheitas++;
}

void Jardineiro::descansar() {
    cont_movimentos = 0;
    cont_plantacoes = 0;
    cont_colheitas = 0;
    cout << "O Jardineiro descansou. Energias repostas!" << endl;
}