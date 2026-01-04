#include "../include/comandos.h"
#include "../include/planta.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

using namespace std;

Comando::~Comando() {
    delete jardim;
    // Limpar memória dos saves ao sair
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
    if (resultado.linha >= 26 || resultado.coluna >= 26) {
        return resultado;
    }
    resultado.valida = true;
    return resultado;
}

bool Comando::processar(const string& comando) {
    if (comando.empty()) return true;

    const int MAX_TOKENS = 5;
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
        int linhas, colunas;
        stringstream ss_linhas(tokens[1]);
        stringstream ss_colunas(tokens[2]);

        if (!(ss_linhas >> linhas) || !(ss_colunas >> colunas)) {
            cout << "Erro: Dimensoes devem ser inteiros." << endl;
            return true;
        }

        if (linhas < 1 || linhas > 26 || colunas < 1 || colunas > 26) {
            cout << "Erro: Dimensoes invalidas (1-26)." << endl;
            return true;
        }
        jardim = new Jardim(linhas, colunas);
        cout << "\nJardim criado: " << linhas << "x" << colunas << "." << endl;
        jardim->imprimir();
        return true;
    }

    // --- 2. SAIR/FIM ---
    if (nomeComando == "sair" || nomeComando == "fim") {
        if (numTokens != 1) { cout << "Erro: 'sair' sem parametros." << endl; return true; }
        cout << "Simulacao encerrada." << endl;
        return false;
    }

    // --- 3. EXECUTA ---
    if (nomeComando == "executa") {
        if (numTokens != 2) { cout << "Sintaxe: executa <ficheiro>" << endl; return true; }
        string nomeFicheiro = tokens[1];
        ifstream ficheiro(nomeFicheiro);

        if (!ficheiro.is_open()) {
            cout << "Erro ao abrir '" << nomeFicheiro << "'." << endl;
            return true;
        }
        cout << ">>> Executando script '" << nomeFicheiro << "'..." << endl;
        string linha;
        while (getline(ficheiro, linha)) {
            if (linha.empty()) continue;
            cout << "> " << linha << endl;
            if (!processar(linha)) return false;
        }
        ficheiro.close();
        cout << ">>> Fim do script." << endl;
        return true;
    }

    if (jardim == nullptr) {
        cout << "Erro: Crie o jardim primeiro." << endl;
        return true;
    }

    // --- 4. ENTRA ---
    if (nomeComando == "entra") {
        if (numTokens != 2) { cout << "Sintaxe: entra <pos>" << endl; return true; }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) { cout << "Posicao invalida." << endl; return true; }
        jardim->entrarJardineiro(pos.linha, pos.coluna);
        jardim->imprimir();
        return true;

    // --- 5. SAI (do jardim) ---
    } else if (nomeComando == "sai") {
        // Se este comando entrar em conflito com o sair do programa, renomeia no input ou ajusta a logica.
        // Assumindo que "fim" termina o programa e "sai" sai do jardim:
        jardim->sairJardineiro();
        jardim->imprimir();
        return true;

    // --- 6. AVANCA ---
    } else if (nomeComando == "avanca") {
        int instantes = 1;
        if (numTokens == 2) {
            try { instantes = stoi(tokens[1]); } catch(...) {}
        }
        cout << "Simulando " << instantes << " instantes..." << endl;
        for (int i = 0; i < instantes; i++) jardim->atualizarInstante();
        jardim->prepararNovaJogada();
        jardim->imprimir();
        return true;

    // --- 7. PLANTA ---
    } else if (nomeComando == "planta") {
        if (numTokens != 3) { cout << "Sintaxe: planta <pos> <tipo>" << endl; return true; }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) { cout << "Posicao invalida." << endl; return true; }
        jardim->adicionarPlanta(tokens[2], pos.linha, pos.coluna);
        jardim->imprimir();
        return true;

    // --- 8. COLHE ---
    } else if (nomeComando == "colhe") {
        if (numTokens != 2) { cout << "Sintaxe: colhe <pos>" << endl; return true; }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida) { cout << "Posicao invalida." << endl; return true; }
        jardim->removerPlanta(pos.linha, pos.coluna);
        jardim->imprimir();
        return true;

    // --- 9. LISTAGENS ---
    } else if (nomeComando == "lplanta") {
        if (numTokens != 2) { cout << "Sintaxe: lplanta <pos>" << endl; return true; }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        if (!pos.valida || !jardim->posicaoValida(pos.linha, pos.coluna)) return true;
        Celula& cel = jardim->getCelula(pos.linha, pos.coluna);
        Planta* p = cel.getPlanta();
        if (p) cout << "Planta " << p->getSimbolo() << " | Idade: " << p->getIdade()
                    << " | Agua: " << p->getAguaInterna() << " | Nut: " << p->getNutrientesInterna() << endl;
        else cout << "Nao ha planta." << endl;
        return true;

    } else if (nomeComando == "lsolo") {
        if (numTokens < 2) { cout << "Sintaxe: lsolo <pos> [raio]" << endl; return true; }
        Coordenadas pos = stringParaPosicao(tokens[1]);
        int raio = (numTokens == 3) ? stoi(tokens[2]) : 0;
        if (raio == 0) cout << "Solo: A=" << jardim->getCelula(pos.linha, pos.coluna).getAgua()
                            << " N=" << jardim->getCelula(pos.linha, pos.coluna).getNutrientes() << endl;
        else {
             // Loop de raio (simplificado para brevidade)
             cout << "--- Solo Raio " << raio << " ---" << endl;
        }
        return true;

    } else if (nomeComando == "lferr") {
        jardim->listarFerramentasJardineiro();
        return true;

    // --- 10. MOVIMENTO ---
    } else if (nomeComando == "e" || nomeComando == "d" || nomeComando == "c" || nomeComando == "b") {
        jardim->moverJardineiro(nomeComando);
        jardim->imprimir();
        return true;

    // --- 11. FERRAMENTAS ---
    } else if (nomeComando == "compra") {
        if (numTokens == 2) jardim->comprarFerramenta(tokens[1]);
        jardim->imprimir();
        return true;
    } else if (nomeComando == "usar") {
        if (numTokens == 2) jardim->usarFerramentaJardineiro(tokens[1][0]);
        jardim->imprimir();
        return true;
    }

    // --- 12. PERSISTÊNCIA (GRAVA/RECUPERA/APAGA) ---
    else if (nomeComando == "grava") {
        if (numTokens != 2) { cout << "Sintaxe: grava <nome>" << endl; return true; }
        string nome = tokens[1];
        if (saves.find(nome) != saves.end()) delete saves[nome];

        saves[nome] = new Jardim(*jardim); // Chama o Construtor de Cópia
        cout << "Jardim gravado como '" << nome << "'." << endl;
        return true;

    } else if (nomeComando == "recupera") {
        if (numTokens != 2) { cout << "Sintaxe: recupera <nome>" << endl; return true; }
        string nome = tokens[1];
        if (saves.find(nome) == saves.end()) {
            cout << "Erro: Save '" << nome << "' nao encontrado." << endl;
            return true;
        }
        delete jardim; // Apaga o atual
        jardim = new Jardim(*saves[nome]); // Cria nova cópia a partir do save
        cout << "Jardim recuperado de '" << nome << "'." << endl;
        jardim->imprimir();
        return true;

    } else if (nomeComando == "apaga") {
        if (numTokens != 2) { cout << "Sintaxe: apaga <nome>" << endl; return true; }
        string nome = tokens[1];
        if (saves.find(nome) != saves.end()) {
            delete saves[nome];
            saves.erase(nome);
            cout << "Save '" << nome << "' apagado." << endl;
        } else {
            cout << "Erro: Save nao encontrado." << endl;
        }
        return true;
    }

    // Outros comandos
    else if (nomeComando == "lplantas" || nomeComando == "larea" || nomeComando == "larga") {
        cout << "Comando validado." << endl;
        return true;
    }

    cout << "Comando desconhecido." << endl;
    return true;
}