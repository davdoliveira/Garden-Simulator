#include "../include/jardim.h"
#include "../include/roseira.h"
#include "../include/cacto.h"
#include "../include/ervadaninha.h"
#include "../include/plantaexotica.h"
#include "../include/Settings.h"
#include "../include/jardineiro.h"
#include "../include/planta.h"
#include "../include/regador.h"
#include "../include/adubo.h"
#include "../include/tesourapoda.h"
#include "../include/ferramentaz.h"

#include <iostream>

using namespace std;

Jardim::Jardim(int lin, int col) {
    linhas = lin;
    colunas = col;

    // CRIAÇÃO DO JARDINEIRO (Agregação)
    // Inicializa fora do jardim (-1, -1)
    jardineiro = new Jardineiro(-1, -1);

    // Aloca a grelha dinamicamente (Matriz de Objetos)
    grelha = new Celula*[linhas];
    for (int i = 0; i < linhas; i++) {
        grelha[i] = new Celula[colunas];
    }

    // Inicializa água e nutrientes
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            grelha[i][j].setAgua(Settings::Jardim::agua_max);
            grelha[i][j].setNutrientes(Settings::Jardim::nutrientes_max);
        }
    }
}

Jardim::~Jardim() {
    // 1. Libera cada linha (array de celulas)
    for (int i = 0; i < linhas; i++) {
        delete[] grelha[i];
    }
    // 2. Libera o array de ponteiros
    delete[] grelha;

    // 3. Libera o Jardineiro
    delete jardineiro;
}

void Jardim::adicionarPlanta(const std::string& tipo, int linha, int coluna) {
    if (!posicaoValida(linha, coluna)) return;
    // --- VERIFICAÇÃO ---
    // Precisamos do jardineiro para validar a regra
    if (jardineiro != nullptr && !jardineiro->podePlantar()) {
        cout << "O Jardineiro ja plantou o maximo hoje ("
             << Settings::Jardineiro::max_plantacoes << ")." << endl;
        return;
    }

    Celula* cel = &grelha[linha][coluna];

    // Verifica se já tem planta
    if (cel->getPlanta() != nullptr) {
        std::cout << "Erro: A celula ja tem uma planta." << std::endl;
        return;
    }

    Planta* novaPlanta = nullptr;

    // --- FÁBRICA DE PLANTAS ---
    if (tipo == "r") {
        novaPlanta = new Roseira();
    } else if (tipo == "c") {
        novaPlanta = new Cacto();
    } else if (tipo == "e") {
        novaPlanta = new ErvaDaninha();
    } else if (tipo == "x") {
        novaPlanta = new PlantaExotica();
    } else {
        std::cout << "Erro: Tipo de planta desconhecido." << std::endl;
        return;
    }

    cel->setPlanta(novaPlanta);
    std::cout << "Planta '" << tipo << "' adicionada em " << posicaoParaString(linha, coluna) << "." << std::endl;
    if (jardineiro != nullptr) jardineiro->registarPlantacao();
}

void Jardim::removerPlanta(int linha, int coluna) {
    if (!posicaoValida(linha, coluna)) return;

    // --- VERIFICAÇÃO ---
    if (jardineiro != nullptr && !jardineiro->podeColher()) {
        cout << "O Jardineiro ja colheu o maximo hoje." << endl;
        return;
    }
    Celula* cel = &grelha[linha][coluna];

    if (cel->getPlanta() == nullptr) {
        std::cout << "Erro: Nao ha planta para remover nesta posicao." << std::endl;
        return;
    }

    cel->removerPlanta(); // Isto já chama o delete internamente na Celula
    std::cout << "Planta removida de " << posicaoParaString(linha, coluna) << "." << std::endl;
    if (jardineiro != nullptr) jardineiro->registarColheita();
}

void Jardim::imprimir() {
    cout << "\n  ";
    for (int j = 0; j < colunas; j++) {
        cout << static_cast<char>('a' + j) << " ";
    }
    cout << endl;

    for (int i = 0; i < linhas; i++) {
        cout << static_cast<char>('a' + i) << " ";
        for (int j = 0; j < colunas; j++) {
            char simbolo = grelha[i][j].getSimbolo();

            // Verifica se o Jardineiro está nesta posição
            if (jardineiro != nullptr && jardineiro->getLinha() == i && jardineiro->getColuna() == j) {
                cout << "* ";
            } else {
                cout << simbolo << " ";
            }
        }
        cout << endl;
    }
}

void Jardim::atualizarInstante() {
    // Percorre toda a grelha e chama atualizar() para cada planta
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Planta* p = grelha[i][j].getPlanta();
            if (p != nullptr) {
                p->atualizar(this, i, j);
            }
        }
    }
}

bool Jardim::posicaoValida(int lin, int col) const {
    return lin >= 0 && lin < linhas && col >= 0 && col < colunas;
}

Celula& Jardim::getCelula(int lin, int col) {
    return grelha[lin][col]; // Retorna referência para a célula real
}

std::string Jardim::posicaoParaString(int lin, int col) const {
    string s = "";
    s += (char)('a' + lin);
    s += (char)('a' + col);
    return s;
}

void Jardim::moverJardineiro(const std::string& direcao) {
    if (jardineiro == nullptr) return;

    // --- VERIFICAÇÃO ---
    if (!jardineiro->podeMover()) {
        cout << "O Jardineiro esta cansado! (Max movimentos atingido). Use 'avanca' para descansar." << endl;
        return;
    }

    // Se é o primeiro movimento (entrar)...
    if (jardineiro->getLinha() == -1) {
        jardineiro->entrar(0, 0);
        cout << "O Jardineiro entrou no jardim." << endl;
    } else {
        jardineiro->mover(direcao[0], linhas, colunas);
        jardineiro->registarMovimento(); // <--- REGISTA O GASTO
    }

    int l = jardineiro->getLinha();
    int c = jardineiro->getColuna();

    // Pegar endereço da célula
    Celula* cel = &grelha[l][c];

    if (cel->getFerramenta() != nullptr) {
        // 1. O Jardineiro guarda o ponteiro
        jardineiro->adicionarFerramenta(cel->getFerramenta());
        cel->esquecerFerramenta();
        cout << "Ferramenta transferida do chao para o jardineiro." << endl;
    }
}

void Jardim::comprarFerramenta(const std::string& tipo) {
    if (jardineiro == nullptr || jardineiro->getLinha() == -1) {
        std::cout << "Erro: O jardineiro precisa de estar dentro do jardim para comprar." << std::endl;
        return;
    }

    int l = jardineiro->getLinha();
    int c = jardineiro->getColuna();
    Celula* cel = &grelha[l][c]; // Pegar endereço da célula atual

    if (cel->getFerramenta() != nullptr) {
        std::cout << "Erro: Ja existe uma ferramenta nesta posicao. Mova-se para comprar outra." << std::endl;
        return;
    }

    Ferramenta* novaFerramenta = nullptr;

    // FÁBRICA DE FERRAMENTAS
    if (tipo == "g") { // 'g' de reGador
        novaFerramenta = new Regador();
    } else if (tipo == "a") { // 'a' de Adubo
        novaFerramenta = new Adubo();
    } else if (tipo == "t") { // 't' de Tesoura
        novaFerramenta = new TesouraPoda();
    } else if (tipo == "z") { // 'z' de FerramentaZ (Sonda)
        novaFerramenta = new FerramentaZ();
    } else {
        std::cout << "Erro: Tipo de ferramenta invalido." << std::endl;
        return;
    }

    // Coloca a ferramenta no chão
    cel->setFerramenta(novaFerramenta);
    std::cout << "Comprou um " << tipo << "! Ele caiu." << std::endl;

    // TRUQUE: Como a função 'moverJardineiro' é que apanha as coisas,
    // vamos "simular" um movimento nulo para forçar a verificação de apanhar.
    // Mas, por agora, deixamos no chão para veres o símbolo a aparecer.
}

void Jardim::usarFerramentaJardineiro(char tipo) {
    if (jardineiro == nullptr || jardineiro->getLinha() == -1) {
        cout << "O jardineiro nao esta no jardim." << endl;
        return;
    }
    jardineiro->usarFerramenta(tipo, this);
}

void Jardim::listarFerramentasJardineiro() const {
    if (jardineiro != nullptr) {
        jardineiro->listarInventario();
    }
}

void Jardim::prepararNovaJogada() {
    if (jardineiro != nullptr) {
        jardineiro->descansar();
    }
}