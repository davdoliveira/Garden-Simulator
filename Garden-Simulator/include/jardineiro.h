#ifndef JARDINEIRO_H
#define JARDINEIRO_H

#include <string>

class Ferramenta;
class Jardim;

class Jardineiro {
private:
    int linha, coluna;
    Ferramenta** ferramentas;
    int numFerramentas;
    int capacidade;
    void redimensionar();

    // Contadores de Fadiga
    int cont_movimentos = 0;
    int cont_plantacoes = 0;
    int cont_colheitas = 0;
    int cont_entradas_saidas = 0;

public:
    Jardineiro(int lin, int col);

    // --- NOVO: Construtor de Cópia (Agora Público) ---
    Jardineiro(const Jardineiro& outro);

    ~Jardineiro();

    // Getters
    int getLinha() const { return linha; }
    int getColuna() const { return coluna; }

    // Movimentação
    void mover(char direcao, int max_lin, int max_col);
    void entrar(int lin, int col);

    // Ferramentas
    void adicionarFerramenta(Ferramenta* f);
    void usarFerramenta(char tipo, Jardim* jardim);
    bool temFerramenta(const std::string& tipo) const;
    void listarInventario() const;

    // Regras de Fadiga
    bool podeMover() const;
    bool podePlantar() const;
    bool podeColher() const;
    bool podeEntrarOuSair() const;

    // Registos
    void registarMovimento();
    void registarPlantacao();
    void registarColheita();
    void registarEntradaSaida();

    void sair();
    void descansar();
};

#endif // JARDINEIRO_H