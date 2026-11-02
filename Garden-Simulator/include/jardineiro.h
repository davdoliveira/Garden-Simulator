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
    Ferramenta* ferramenta_na_mao;

    void redimensionar();

    // Contadores por turno (entre instantes)
    int cont_movimentos = 0;
    int cont_plantacoes = 0;
    int cont_colheitas = 0;
    int cont_entradas = 0;
    int cont_saidas = 0;

public:
    Jardineiro(int lin, int col);
    Jardineiro(const Jardineiro& outro);
    ~Jardineiro();

    int getLinha() const { return linha; }
    int getColuna() const { return coluna; }
    bool estaNoJardim() const { return linha != -1 && coluna != -1; }

    bool mover(char direcao, int max_lin, int max_col);
    void entrar(int lin, int col);
    void sair();

    void adicionarFerramenta(Ferramenta* f);
    bool pegarFerramenta(int num_serie);
    void largarFerramenta();
    void aplicarFerramentaNaMao(Jardim* jardim);
    void listarInventario() const;

    bool podeMover() const;
    bool podePlantar() const;
    bool podeColher() const;
    bool podeEntrar() const;
    bool podeSair() const;

    void registarMovimento();
    void registarPlantacao();
    void registarColheita();
    void registarEntrada();
    void registarSaida();

    void descansar();
};

#endif // JARDINEIRO_H
