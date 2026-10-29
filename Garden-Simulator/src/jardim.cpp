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
    jardineiro = new Jardineiro(-1, -1);
    grelha = new Celula*[linhas];
    for (int i = 0; i < linhas; i++) {
        grelha[i] = new Celula[colunas];
    }
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            grelha[i][j].setAgua(Settings::Jardim::agua_max);
            grelha[i][j].setNutrientes(Settings::Jardim::nutrientes_max);
        }
    }
}

// --- NOVO: Construtor de Cópia ---
Jardim::Jardim(const Jardim& outro) {
    linhas = outro.linhas;
    colunas = outro.colunas;

    // Copiar Jardineiro
    if (outro.jardineiro != nullptr) {
        jardineiro = new Jardineiro(*outro.jardineiro);
    } else {
        jardineiro = nullptr;
    }

    // Alocar nova grelha
    grelha = new Celula*[linhas];
    for (int i = 0; i < linhas; i++) {
        grelha[i] = new Celula[colunas];
    }

    // Copiar conteúdo de cada célula (Deep Copy)
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            grelha[i][j].setAgua(outro.grelha[i][j].getAgua());
            grelha[i][j].setNutrientes(outro.grelha[i][j].getNutrientes());

            // Clonar Planta
            Planta* pOriginal = outro.grelha[i][j].getPlanta();
            if (pOriginal != nullptr) {
                grelha[i][j].setPlanta(pOriginal->duplicar());
            }

            // Clonar Ferramenta do chão
            Ferramenta* fOriginal = outro.grelha[i][j].getFerramenta();
            if (fOriginal != nullptr) {
                grelha[i][j].setFerramenta(fOriginal->duplicar());
            }
        }
    }
}

Jardim::~Jardim() {
    for (int i = 0; i < linhas; i++) {
        delete[] grelha[i];
    }
    delete[] grelha;
    delete jardineiro;
}

void Jardim::adicionarPlanta(const std::string& tipo, int linha, int coluna) {
    if (!posicaoValida(linha, coluna)) return;
    if (jardineiro != nullptr && !jardineiro->podePlantar()) {
         cout << "O Jardineiro ja plantou o maximo hoje." << endl;
         return;
    }
    Celula* cel = &grelha[linha][coluna];
    if (cel->getPlanta() != nullptr) {
        cout << "Erro: A celula ja tem uma planta." << endl;
        return;
    }
    Planta* novaPlanta = nullptr;
    if (tipo == "r") novaPlanta = new Roseira();
    else if (tipo == "c") novaPlanta = new Cacto();
    else if (tipo == "e") novaPlanta = new ErvaDaninha();
    else if (tipo == "x") novaPlanta = new PlantaExotica();
    else {
        cout << "Erro: Tipo desconhecido." << endl;
        return;
    }
    cel->setPlanta(novaPlanta);
    cout << "Planta '" << tipo << "' adicionada em " << posicaoParaString(linha, coluna) << "." << endl;
    if (jardineiro != nullptr) jardineiro->registarPlantacao();
}

void Jardim::removerPlanta(int linha, int coluna) {
    if (!posicaoValida(linha, coluna)) return;
    if (jardineiro != nullptr && !jardineiro->podeColher()) {
         cout << "O Jardineiro ja colheu o maximo hoje." << endl;
         return;
    }
    Celula* cel = &grelha[linha][coluna];
    if (cel->getPlanta() == nullptr) {
        cout << "Erro: Nao ha planta para remover." << endl;
        return;
    }
    cel->removerPlanta();
    cout << "Planta removida de " << posicaoParaString(linha, coluna) << "." << endl;
    if (jardineiro != nullptr) jardineiro->registarColheita();
}

void Jardim::imprimir() {
    cout << "\n  ";
    for (int j = 0; j < colunas; j++) cout << static_cast<char>('a' + j) << " ";
    cout << endl;
    for (int i = 0; i < linhas; i++) {
        cout << static_cast<char>('a' + i) << " ";
        for (int j = 0; j < colunas; j++) {
            char simbolo = grelha[i][j].getSimbolo();
            if (jardineiro != nullptr && jardineiro->getLinha() == i && jardineiro->getColuna() == j)
                cout << "* ";
            else
                cout << simbolo << " ";
        }
        cout << endl;
    }
}

void Jardim::atualizarInstante() {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Planta* p = grelha[i][j].getPlanta();
            if (p != nullptr) p->atualizar(this, i, j);
        }
    }
}

bool Jardim::posicaoValida(int lin, int col) const {
    return lin >= 0 && lin < linhas && col >= 0 && col < colunas;
}

Celula& Jardim::getCelula(int lin, int col) { return grelha[lin][col]; }

std::string Jardim::posicaoParaString(int lin, int col) const {
    string s = "";
    s += (char)('a' + lin);
    s += (char)('a' + col);
    return s;
}

void Jardim::moverJardineiro(const std::string& direcao) {
    if (jardineiro == nullptr) return;
    if (!direcao.empty()) {
        if (!jardineiro->podeMover()) {
            cout << "O Jardineiro esta cansado!" << endl;
            return;
        }
        if (jardineiro->getLinha() == -1) {
             if (!jardineiro->podeEntrarOuSair()) {
                cout << "Regra violada: Limite de entrada/saida." << endl;
                return;
            }
            jardineiro->entrar(0, 0);
            jardineiro->registarEntradaSaida();
            cout << "O Jardineiro entrou no jardim." << endl;
        } else {
            jardineiro->mover(direcao[0], linhas, colunas);
            jardineiro->registarMovimento();
        }
    }
    if (jardineiro->getLinha() != -1) {
        int l = jardineiro->getLinha();
        int c = jardineiro->getColuna();
        Celula* cel = &grelha[l][c];
        if (cel->getFerramenta() != nullptr) {
            jardineiro->adicionarFerramenta(cel->getFerramenta());
            cel->esquecerFerramenta();
            cout << "Ferramenta apanhada." << endl;
        }
    }
}

void Jardim::comprarFerramenta(const std::string& tipo) {
    if (jardineiro == nullptr || jardineiro->getLinha() == -1) {
        cout << "O jardineiro precisa de estar dentro do jardim." << endl;
        return;
    }
    int l = jardineiro->getLinha();
    int c = jardineiro->getColuna();
    Celula* cel = &grelha[l][c];
    if (cel->getFerramenta() != nullptr) {
        cout << "Ja existe uma ferramenta aqui." << endl;
        return;
    }
    Ferramenta* novaFerramenta = nullptr;
    if (tipo == "g") novaFerramenta = new Regador();
    else if (tipo == "a") novaFerramenta = new Adubo();
    else if (tipo == "t") novaFerramenta = new TesouraPoda();
    else if (tipo == "z") novaFerramenta = new FerramentaZ();
    else { cout << "Tipo invalido." << endl; return; }
    cel->setFerramenta(novaFerramenta);
    cout << "Comprou um " << tipo << "." << endl;
    moverJardineiro("");
}

void Jardim::usarFerramentaJardineiro(char tipo) {
    if (jardineiro == nullptr || jardineiro->getLinha() == -1) {
        cout << "O jardineiro nao esta no jardim." << endl;
        return;
    }
    jardineiro->usarFerramenta(tipo, this);
}

void Jardim::listarFerramentasJardineiro() const {
    if (jardineiro != nullptr) jardineiro->listarInventario();
}

void Jardim::prepararNovaJogada() {
    if (jardineiro != nullptr) jardineiro->descansar();
}

void Jardim::entrarJardineiro(int linha, int coluna) {
    if (jardineiro == nullptr) return;
    if (!jardineiro->podeEntrarOuSair()) {
        cout << "Regra violada: Limite de entrada/saida." << endl;
        return;
    }
    if (!posicaoValida(linha, coluna)) {
        cout << "Erro: Posicao fora do jardim." << endl;
        return;
    }
    jardineiro->entrar(linha, coluna);
    jardineiro->registarEntradaSaida();
    cout << "O Jardineiro entrou em " << posicaoParaString(linha, coluna) << "." << endl;
    moverJardineiro("");
}

void Jardim::sairJardineiro() {
    if (jardineiro == nullptr) return;
    if (jardineiro->getLinha() == -1) {
        cout << "Ja esta fora." << endl;
        return;
    }
    if (!jardineiro->podeEntrarOuSair()) {
        cout << "Regra violada: Limite de entrada/saida." << endl;
        return;
    }
    jardineiro->sair();
    jardineiro->registarEntradaSaida();
}