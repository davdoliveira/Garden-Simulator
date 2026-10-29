#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../include/comandos.h"

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Comando comando;
    string entrada;

    cout << "=== Simulador de Jardim (POO 25/26) ===" << endl;
    cout << "> ";
    while (getline(cin, entrada) && comando.processar(entrada)) {
        cout << "> ";
    }
    return 0;
}
