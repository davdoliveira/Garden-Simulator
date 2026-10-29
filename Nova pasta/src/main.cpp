#include <iostream>
#include "../include/comandos.h"
using namespace std;

int main() {
    Comando comando;
    string entrada;

    cout << "=== simulador de jardim ===\n";
    cout << "> ";
    while (getline(cin, entrada) && comando.processar(entrada)) {
        // Pede o próximo comando
        cout << "> ";
    }
    return 0;
}
