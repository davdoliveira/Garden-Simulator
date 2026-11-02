#include "../include/comandos.h"
#include "../include/planta.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

using namespace std;

Comando::~Comando() {
    delete jardim;
    for (auto const& [nome, saveJardim] : saves) {
        delete saveJardim;
    }
    saves.clear();
}

Coordenadas Comando::stringParaPosicao(const std::string& pos) const {
    Coordenadas resultado;
    if (pos.length() != 2) return resultado;
    if ((pos[0] < 'a' || pos[0] > 'z') || (pos[1] < 'a' || pos[1] > 'z')) {
        return resultado;
    }
    resultado.linha = pos[0] - 'a';
    resultado.coluna = pos[1] - 'a';
    if (jardim != nullptr) {
        if (!jardim->posicaoValida(resultado.linha, resultado.coluna)) {
            return resultado;
        }
    }
    resultado.valida = true;
    return resultado;
}

bool Comando::lerInteiroPositivo(const std::string& str, int& valor, bool permitirZero) const {
    try {
        size_t pos = 0;
        int v = std::stoi(str, &pos);
        if (pos != str.length()) return false;
        if (permitirZero ? (v < 0) : (v <= 0)) return false;
        valor = v;
        return true;
    } catch (...) {
        return false;
    }
}

bool Comando::processar(const string& comando) {
    if (comando.empty()) return true;

    const int MAX_TOKENS = 6;
    string tokens[MAX_TOKENS];
    int numTokens = 0;

    stringstream ss(comando);
    string token;
    while (numTokens < MAX_TOKENS && ss >> token) {
        tokens[numTokens] = token;
        numTokens++;
    }
    if (numTokens == 0) return true;

    string nomeComando = tokens[0];

    // --- 1. JARDIM ---
    if (nomeComando == "jardim") {
        if (jardim != nullptr) {
            cout << "Erro: O jardim ja foi criado." << endl;
            return true;
        }
        if (numTokens != 3) {
            cout << "Sintaxe: jardim <linhas> <colunas>" << endl;
            return true;
        }
        int linhas = 0, colunas = 0;
        if (!lerInteiroPositivo(tokens[1], linhas) || !lerInteiroPositivo(tokens[2], colunas)) {
            cout << "Erro: As dimensoes devem ser numeros inteiros positivos." << endl;
            return true;
        }
        if (linhas < 1 || linhas > 26 || colunas < 1 || colunas > 26) {
            cout << "Erro: Dimensoes invalidas (maximo 26x26)." << endl;
            return true;
        }
        jardim = new Jardim(linhas, colunas);
        cout << "Jardim criado com dimensoes " << linhas << "x" << colunas << "." << endl;
        jardim->imprimir();
        return true;
    }

    // --- 2. FIM ---
    if (nomeComando == "fim") {
        if (numTokens != 1) {
            cout << "Sintaxe: fim (sem parametros)" << endl;
            return true;
        }
        cout << "Simulacao encerrada. A libertar recursos..." << endl;
        return false;
    }

    // --- 3. EXECUTA ---
    if (nomeComando == "executa") {
        if (numTokens != 2) {
            cout << "Sintaxe: executa <nome-do-ficheiro>" << endl;
            return true;
        }
        string nomeFicheiro = tokens[1];
        ifstream ficheiro(nomeFicheiro);

        if (!ficheiro.is_open()) {
            cout << "Erro: Nao foi possivel abrir o ficheiro '" << nomeFicheiro << "'." << endl;
            return true;
        }
        cout << ">>> A executar comandos do ficheiro '" << nomeFicheiro << "'..." << endl;
        string linha;
        while (getline(ficheiro, linha)) {
            if (!linha.empty() && linha.back() == '\r') {
                linha.pop_back();
            }
            if (linha.empty()) continue;
            cout << "> " << linha << endl;
            if (!processar(linha)) return false;
        }
        ficheiro.close();
        cout << ">>> Fim da execucao do ficheiro '" << nomeFicheiro << "'." << endl;
        return true;
    }

    // Todos os restantes comandos exigem que o jardim já tenha sido criado
    if (jardim == nullptr) {
        cout << "Erro: O jardim ainda nao foi criado. Execute primeiro 'jardim <n> <n>'." << endl;
        return true;
    }

    // --- 4. AVANCA ---
    if (nomeComando == "avanca") {
        if (numTokens > 2) {
            cout << "Sintaxe: avanca [n]" << endl;
            return true;
        }
        int instantes = 1;
        if (numTokens == 2) {
            if (!lerInteiroPositivo(tokens[1], instantes)) {
                cout << "Erro: O numero de instantes deve ser um inteiro positivo." << endl;
                return true;
            }
        }
        for (int i = 0; i < instantes; i++) {
            cout << "--- Instante " << (jardim->getInstanteAtual() + 1) << " ---" << endl;
            jardim->atualizarInstante();
        }
        jardim->imprimir();
        return true;
    }

    // --- 5. COMANDOS DE LISTAGEM ---
    if (nomeComando == "lplantas") {
        if (numTokens != 1) {
            cout << "Sintaxe: lplantas" << endl;
            return true;
        }
        jardim->listarPlantas();
        return true;
    }

    if (nomeComando == "lplanta") {
        if (numTokens != 2) {
            cout << "Sintaxe: lplanta <l><c>" << endl;
            return true;
        }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) {
            cout << "Erro: Posicao '" << tokens[1] << "' invalida ou fora dos limites." << endl;
            return true;
        }
        jardim->listarPlanta(pos.linha, pos.coluna);
        return true;
    }

    if (nomeComando == "larea") {
        if (numTokens != 1) {
            cout << "Sintaxe: larea" << endl;
            return true;
        }
        jardim->listarArea();
        return true;
    }

    if (nomeComando == "lsolo") {
        if (numTokens < 2 || numTokens > 3) {
            cout << "Sintaxe: lsolo <l><c> [n]" << endl;
            return true;
        }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) {
            cout << "Erro: Posicao '" << tokens[1] << "' invalida ou fora dos limites." << endl;
            return true;
        }
        int raio = 0;
        if (numTokens == 3) {
            if (!lerInteiroPositivo(tokens[2], raio, true)) {
                cout << "Erro: O raio deve ser um inteiro nao negativo." << endl;
                return true;
            }
        }
        jardim->listarSolo(pos.linha, pos.coluna, raio);
        return true;
    }

    if (nomeComando == "lferr") {
        if (numTokens != 1) {
            cout << "Sintaxe: lferr" << endl;
            return true;
        }
        jardim->listarFerramentasJardineiro();
        return true;
    }

    // --- 6. COMANDOS DE AÇÃO ---
    if (nomeComando == "colhe") {
        if (numTokens != 2) {
            cout << "Sintaxe: colhe <l><c>" << endl;
            return true;
        }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) {
            cout << "Erro: Posicao '" << tokens[1] << "' invalida ou fora dos limites." << endl;
            return true;
        }
        if (jardim->removerPlanta(pos.linha, pos.coluna)) {
            jardim->imprimir();
        }
        return true;
    }

    if (nomeComando == "planta") {
        if (numTokens != 3) {
            cout << "Sintaxe: planta <l><c> <tipo>" << endl;
            return true;
        }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) {
            cout << "Erro: Posicao '" << tokens[1] << "' invalida ou fora dos limites." << endl;
            return true;
        }
        if (jardim->adicionarPlanta(tokens[2], pos.linha, pos.coluna)) {
            jardim->imprimir();
        }
        return true;
    }

    if (nomeComando == "larga") {
        if (numTokens != 1) {
            cout << "Sintaxe: larga" << endl;
            return true;
        }
        jardim->largarFerramentaJardineiro();
        return true;
    }

    if (nomeComando == "pega") {
        if (numTokens != 2) {
            cout << "Sintaxe: pega <n>" << endl;
            return true;
        }
        int num_serie = 0;
        if (!lerInteiroPositivo(tokens[1], num_serie)) {
            cout << "Erro: O numero de serie deve ser um inteiro positivo." << endl;
            return true;
        }
        jardim->pegarFerramentaJardineiro(num_serie);
        return true;
    }

    if (nomeComando == "compra") {
        if (numTokens != 2) {
            cout << "Sintaxe: compra <c>" << endl;
            return true;
        }
        jardim->comprarFerramenta(tokens[1]);
        return true;
    }

    // --- 7. MOVIMENTO DO JARDINEIRO ---
    if (nomeComando == "e" || nomeComando == "d" || nomeComando == "c" || nomeComando == "b") {
        if (numTokens != 1) {
            cout << "Sintaxe: " << nomeComando << " (sem parametros)" << endl;
            return true;
        }
        if (jardim->moverJardineiro(nomeComando)) {
            jardim->imprimir();
        }
        return true;
    }

    if (nomeComando == "entra") {
        if (numTokens != 2) {
            cout << "Sintaxe: entra <l><c>" << endl;
            return true;
        }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) {
            cout << "Erro: Posicao '" << tokens[1] << "' invalida ou fora dos limites." << endl;
            return true;
        }
        if (jardim->entrarJardineiro(pos.linha, pos.coluna)) {
            jardim->imprimir();
        }
        return true;
    }

    if (nomeComando == "sai") {
        if (numTokens != 1) {
            cout << "Sintaxe: sai" << endl;
            return true;
        }
        if (jardim->sairJardineiro()) {
            jardim->imprimir();
        }
        return true;
    }

    // --- 8. GRAVA / RECUPERA / APAGA ---
    if (nomeComando == "grava") {
        if (numTokens != 2) {
            cout << "Sintaxe: grava <nome>" << endl;
            return true;
        }
        string nome = tokens[1];
        if (saves.find(nome) != saves.end()) {
            delete saves[nome];
        }
        saves[nome] = new Jardim(*jardim);
        cout << "Estado do jardim gravado sob o nome '" << nome << "'." << endl;
        return true;
    }

    if (nomeComando == "recupera") {
        if (numTokens != 2) {
            cout << "Sintaxe: recupera <nome>" << endl;
            return true;
        }
        string nome = tokens[1];
        auto it = saves.find(nome);
        if (it == saves.end()) {
            cout << "Erro: Nao existe nenhuma gravacao com o nome '" << nome << "'." << endl;
            return true;
        }
        delete jardim;
        jardim = new Jardim(*(it->second));
        delete it->second;
        saves.erase(it);
        cout << "Jardim recuperado da gravacao '" << nome << "' (copia eliminada)." << endl;
        jardim->imprimir();
        return true;
    }

    if (nomeComando == "apaga") {
        if (numTokens != 2) {
            cout << "Sintaxe: apaga <nome>" << endl;
            return true;
        }
        string nome = tokens[1];
        auto it = saves.find(nome);
        if (it != saves.end()) {
            delete it->second;
            saves.erase(it);
            cout << "Gravacao '" << nome << "' apagada com sucesso." << endl;
        } else {
            cout << "Erro: Gravacao '" << nome << "' nao encontrada." << endl;
        }
        return true;
    }

    cout << "Erro: Comando '" << nomeComando << "' desconhecido." << endl;
    return true;
}
