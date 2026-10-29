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
#include <cstdlib>

using namespace std;

Ferramenta* Jardim::criarFerramentaAleatoria() {
    int r = rand() % 4;
    if (r == 0) return new Regador();
    if (r == 1) return new Adubo();
    if (r == 2) return new TesouraPoda();
    return new FerramentaZ();
}

void Jardim::colocarFerramentaAleatoria() {
    int total = linhas * colunas;
    for (int tentativa = 0; tentativa < total * 2; tentativa++) {
        int l = rand() % linhas;
        int c = rand() % colunas;
        if (grelha[l][c].getFerramenta() == nullptr &&
            !(jardineiro != nullptr && jardineiro->getLinha() == l && jardineiro->getColuna() == c)) {
            grelha[l][c].setFerramenta(criarFerramentaAleatoria());
            return;
        }
    }
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (grelha[i][j].getFerramenta() == nullptr) {
                grelha[i][j].setFerramenta(criarFerramentaAleatoria());
                return;
            }
        }
    }
}

void Jardim::verificarApanhaFerramenta() {
    if (jardineiro == nullptr || !jardineiro->estaNoJardim()) return;
    int l = jardineiro->getLinha();
    int c = jardineiro->getColuna();
    Celula& cel = grelha[l][c];
    if (cel.getFerramenta() != nullptr) {
        Ferramenta* f = cel.getFerramenta();
        cel.esquecerFerramenta();
        jardineiro->adicionarFerramenta(f);
        colocarFerramentaAleatoria();
    }
}

Jardim::Jardim(int lin, int col) : linhas(lin), colunas(col), instante_atual(0) {
    jardineiro = new Jardineiro(-1, -1);
    grelha = new Celula*[linhas];
    for (int i = 0; i < linhas; i++) {
        grelha[i] = new Celula[colunas];
    }

    int range_agua = Settings::Jardim::agua_max - Settings::Jardim::agua_min + 1;
    int range_nut = Settings::Jardim::nutrientes_max - Settings::Jardim::nutrientes_min + 1;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int agua_ini = Settings::Jardim::agua_min + (rand() % range_agua);
            int nut_ini = Settings::Jardim::nutrientes_min + (rand() % range_nut);
            grelha[i][j].setAgua(agua_ini);
            grelha[i][j].setNutrientes(nut_ini);
        }
    }

    for (int k = 0; k < 3; k++) {
        colocarFerramentaAleatoria();
    }
}

Jardim::Jardim(const Jardim& outro) {
    linhas = outro.linhas;
    colunas = outro.colunas;
    instante_atual = outro.instante_atual;

    if (outro.jardineiro != nullptr) {
        jardineiro = new Jardineiro(*outro.jardineiro);
    } else {
        jardineiro = nullptr;
    }

    grelha = new Celula*[linhas];
    for (int i = 0; i < linhas; i++) {
        grelha[i] = new Celula[colunas];
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            grelha[i][j].setAgua(outro.grelha[i][j].getAgua());
            grelha[i][j].setNutrientes(outro.grelha[i][j].getNutrientes());

            Planta* pOriginal = outro.grelha[i][j].getPlanta();
            if (pOriginal != nullptr) {
                grelha[i][j].setPlanta(pOriginal->duplicar());
            }

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

bool Jardim::adicionarPlanta(const std::string& tipo, int linha, int coluna) {
    if (!posicaoValida(linha, coluna)) {
        cout << "Erro: Posicao fora dos limites do jardim." << endl;
        return false;
    }
    if (jardineiro != nullptr && !jardineiro->podePlantar()) {
        cout << "Erro: O jardineiro ja atingiu o limite de " << Settings::Jardineiro::max_plantacoes << " plantacoes neste turno." << endl;
        return false;
    }
    Celula* cel = &grelha[linha][coluna];
    if (cel->getPlanta() != nullptr) {
        cout << "Erro: A posicao " << posicaoParaString(linha, coluna) << " ja esta ocupada por uma planta." << endl;
        return false;
    }
    Planta* novaPlanta = nullptr;
    if (tipo == "r") novaPlanta = new Roseira();
    else if (tipo == "c") novaPlanta = new Cacto();
    else if (tipo == "e") novaPlanta = new ErvaDaninha();
    else if (tipo == "x") novaPlanta = new PlantaExotica();
    else {
        cout << "Erro: Tipo de planta '" << tipo << "' desconhecido (use c, r, e, x)." << endl;
        return false;
    }
    cel->setPlanta(novaPlanta);
    if (jardineiro != nullptr) jardineiro->registarPlantacao();
    cout << "Planta '" << tipo << "' colocada em " << posicaoParaString(linha, coluna) << "." << endl;
    return true;
}

bool Jardim::removerPlanta(int linha, int coluna) {
    if (!posicaoValida(linha, coluna)) {
        cout << "Erro: Posicao fora dos limites do jardim." << endl;
        return false;
    }
    if (jardineiro != nullptr && !jardineiro->podeColher()) {
        cout << "Erro: O jardineiro ja atingiu o limite de " << Settings::Jardineiro::max_colheitas << " colheitas neste turno." << endl;
        return false;
    }
    Celula* cel = &grelha[linha][coluna];
    if (cel->getPlanta() == nullptr) {
        cout << "Erro: Nao existe nenhuma planta em " << posicaoParaString(linha, coluna) << " para colher." << endl;
        return false;
    }
    cel->removerPlanta();
    if (jardineiro != nullptr) jardineiro->registarColheita();
    cout << "Planta colhida de " << posicaoParaString(linha, coluna) << "." << endl;
    return true;
}

void Jardim::imprimir() const {
    cout << "\n  ";
    for (int j = 0; j < colunas; j++) {
        cout << static_cast<char>('A' + j);
    }
    cout << " " << endl;

    for (int i = 0; i < linhas; i++) {
        cout << static_cast<char>('A' + i) << " ";
        for (int j = 0; j < colunas; j++) {
            if (jardineiro != nullptr && jardineiro->getLinha() == i && jardineiro->getColuna() == j) {
                cout << '*';
            } else {
                cout << grelha[i][j].getSimbolo();
            }
        }
        cout << " " << static_cast<char>('A' + i) << endl;
    }

    cout << "  ";
    for (int j = 0; j < colunas; j++) {
        cout << static_cast<char>('A' + j);
    }
    cout << " \n" << endl;
}

void Jardim::atualizarInstante() {
    instante_atual++;

    // Resetar flag de ação nas plantas existentes no início do instante
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Planta* p = grelha[i][j].getPlanta();
            if (p != nullptr) {
                p->setJaAgiu(false);
            }
        }
    }

    // 1. As plantas atuam (ignorando plantas recém-multiplicadas neste instante)
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Planta* p = grelha[i][j].getPlanta();
            if (p != nullptr && !p->getJaAgiu()) {
                p->setJaAgiu(true);
                p->atualizar(this, i, j);
            }
        }
    }

    // 2. A ferramenta na mão do jardineiro atua automaticamente uma vez por instante
    if (jardineiro != nullptr) {
        jardineiro->aplicarFerramentaNaMao(this);
        jardineiro->descansar();
    }
}

bool Jardim::posicaoValida(int lin, int col) const {
    return lin >= 0 && lin < linhas && col >= 0 && col < colunas;
}

Celula& Jardim::getCelula(int lin, int col) { return grelha[lin][col]; }
const Celula& Jardim::getCelula(int lin, int col) const { return grelha[lin][col]; }

std::string Jardim::posicaoParaString(int lin, int col) const {
    string s = "";
    s += static_cast<char>('a' + lin);
    s += static_cast<char>('a' + col);
    return s;
}

bool Jardim::moverJardineiro(const std::string& direcao) {
    if (jardineiro == nullptr) return false;
    if (!jardineiro->estaNoJardim()) {
        cout << "Erro: O jardineiro nao se encontra no jardim (use 'entra <l><c>')." << endl;
        return false;
    }
    if (!jardineiro->podeMover()) {
        cout << "Erro: O jardineiro ja realizou o maximo de " << Settings::Jardineiro::max_movimentos << " movimentos neste turno." << endl;
        return false;
    }
    if (jardineiro->mover(direcao[0], linhas, colunas)) {
        jardineiro->registarMovimento();
        verificarApanhaFerramenta();
        return true;
    }
    return false;
}

bool Jardim::comprarFerramenta(const std::string& tipo) {
    if (jardineiro == nullptr) return false;
    Ferramenta* novaFerramenta = nullptr;
    if (tipo == "g") novaFerramenta = new Regador();
    else if (tipo == "a") novaFerramenta = new Adubo();
    else if (tipo == "t") novaFerramenta = new TesouraPoda();
    else if (tipo == "z") novaFerramenta = new FerramentaZ();
    else {
        cout << "Erro: Tipo de ferramenta '" << tipo << "' invalido (use g, a, t, z)." << endl;
        return false;
    }
    jardineiro->adicionarFerramenta(novaFerramenta);
    return true;
}

bool Jardim::pegarFerramentaJardineiro(int num_serie) {
    if (jardineiro == nullptr) return false;
    return jardineiro->pegarFerramenta(num_serie);
}

void Jardim::largarFerramentaJardineiro() {
    if (jardineiro != nullptr) {
        jardineiro->largarFerramenta();
    }
}

void Jardim::listarFerramentasJardineiro() const {
    if (jardineiro != nullptr) jardineiro->listarInventario();
}

void Jardim::listarPlantas() const {
    bool encontrou = false;
    cout << "--- Plantas no Jardim ---" << endl;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Planta* p = grelha[i][j].getPlanta();
            if (p != nullptr) {
                encontrou = true;
                cout << "[" << posicaoParaString(i, j) << "] " << p->getNomeEspecie()
                     << " ('" << p->getSimbolo() << "', Beleza: " << p->getBeleza() << ")"
                     << " | Idade: " << p->getIdade()
                     << " | Agua Interna: " << p->getAguaInterna()
                     << " | Nutrientes Internos: " << p->getNutrientesInterna()
                     << " || Solo -> Agua: " << grelha[i][j].getAgua()
                     << ", Nutrientes: " << grelha[i][j].getNutrientes() << endl;
            }
        }
    }
    if (!encontrou) {
        cout << "Nao existem plantas no jardim." << endl;
    }
}

void Jardim::listarPlanta(int lin, int col) const {
    if (!posicaoValida(lin, col)) {
        cout << "Erro: Posicao fora dos limites do jardim." << endl;
        return;
    }
    const Celula& cel = grelha[lin][col];
    Planta* p = cel.getPlanta();
    if (p != nullptr) {
        cout << "[" << posicaoParaString(lin, col) << "] " << p->getNomeEspecie()
             << " ('" << p->getSimbolo() << "', Beleza: " << p->getBeleza() << ")"
             << " | Idade: " << p->getIdade()
             << " | Agua Interna: " << p->getAguaInterna()
             << " | Nutrientes Internos: " << p->getNutrientesInterna()
             << " || Solo -> Agua: " << cel.getAgua()
             << ", Nutrientes: " << cel.getNutrientes() << endl;
    } else {
        cout << "Nao existe nenhuma planta na posicao " << posicaoParaString(lin, col) << "." << endl;
    }
}

void Jardim::listarArea() const {
    bool encontrou = false;
    cout << "--- Posicoes Nao Vazias no Jardim ---" << endl;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            const Celula& cel = grelha[i][j];
            bool temJardineiro = (jardineiro != nullptr && jardineiro->getLinha() == i && jardineiro->getColuna() == j);
            if (cel.getPlanta() != nullptr || cel.getFerramenta() != nullptr || temJardineiro) {
                encontrou = true;
                listarSolo(i, j, 0);
            }
        }
    }
    if (!encontrou) {
        cout << "Todas as posicoes do jardim estao vazias." << endl;
    }
}

void Jardim::listarSolo(int lin, int col, int raio) const {
    if (!posicaoValida(lin, col)) {
        cout << "Erro: Posicao fora dos limites do jardim." << endl;
        return;
    }
    int l_min = (lin - raio < 0) ? 0 : lin - raio;
    int l_max = (lin + raio >= linhas) ? linhas - 1 : lin + raio;
    int c_min = (col - raio < 0) ? 0 : col - raio;
    int c_max = (col + raio >= colunas) ? colunas - 1 : col + raio;

    for (int i = l_min; i <= l_max; i++) {
        for (int j = c_min; j <= c_max; j++) {
            const Celula& cel = grelha[i][j];
            cout << "Solo [" << posicaoParaString(i, j) << "] -> Agua: " << cel.getAgua()
                 << ", Nutrientes: " << cel.getNutrientes();
            if (jardineiro != nullptr && jardineiro->getLinha() == i && jardineiro->getColuna() == j) {
                cout << " | Jardineiro (*)";
            }
            if (cel.getPlanta() != nullptr) {
                Planta* p = cel.getPlanta();
                cout << " | Planta: " << p->getNomeEspecie() << " ('" << p->getSimbolo()
                     << "', Agua: " << p->getAguaInterna()
                     << ", Nut: " << p->getNutrientesInterna()
                     << ", Idade: " << p->getIdade() << ")";
            }
            if (cel.getFerramenta() != nullptr) {
                Ferramenta* f = cel.getFerramenta();
                cout << " | Ferramenta: " << f->getTipo() << " ('" << f->getSimbolo()
                     << "', Serie #" << f->getNumSerie() << ", " << f->getDetalhes() << ")";
            }
            cout << endl;
        }
    }
}

bool Jardim::entrarJardineiro(int linha, int coluna) {
    if (jardineiro == nullptr) return false;
    if (!posicaoValida(linha, coluna)) {
        cout << "Erro: Posicao fora dos limites do jardim." << endl;
        return false;
    }
    if (!jardineiro->podeEntrar()) {
        cout << "Erro: O jardineiro so pode entrar no jardim " << Settings::Jardineiro::max_entradas_saidas << " vez por turno." << endl;
        return false;
    }
    jardineiro->entrar(linha, coluna);
    jardineiro->registarEntrada();
    cout << "O Jardineiro entrou na posicao " << posicaoParaString(linha, coluna) << "." << endl;
    verificarApanhaFerramenta();
    return true;
}

bool Jardim::sairJardineiro() {
    if (jardineiro == nullptr) return false;
    if (!jardineiro->estaNoJardim()) {
        cout << "Erro: O jardineiro ja se encontra fora do jardim." << endl;
        return false;
    }
    if (!jardineiro->podeSair()) {
        cout << "Erro: O jardineiro so pode sair do jardim " << Settings::Jardineiro::max_entradas_saidas << " vez por turno." << endl;
        return false;
    }
    jardineiro->sair();
    jardineiro->registarSaida();
    return true;
}
