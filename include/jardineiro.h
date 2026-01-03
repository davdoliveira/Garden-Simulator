#ifndef JARDINEIRO_H
#define JARDINEIRO_H

#include <string>

class Ferramenta;
class Jardim;

class Jardineiro {
private:
    int linha, coluna;

    Ferramenta** ferramentas; // Array dinâmico de ponteiros para Ferramenta
    int numFerramentas;       // Número de ferramentas atuais
    int capacidade;           // Tamanho alocado do array 'ferramentas'

    void redimensionar();
    int cont_movimentos = 0;
    int cont_plantacoes = 0;
    int cont_colheitas = 0;
    Jardineiro(const Jardineiro& outro);
    Jardineiro& operator=(const Jardineiro& outro);

public:
    Jardineiro(int lin, int col);
    ~Jardineiro(); // O Destrutor já estava implementado (1/3 da Regra de 3)

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

    // -- FUNÇÕES DE REGRAS ---
    // Retornam true se ainda tiver energia para a ação
    bool podeMover() const;
    bool podePlantar() const;
    bool podeColher() const;

    // Registam que a ação foi feita (incrementam contador)
    void registarMovimento();
    void registarPlantacao();
    void registarColheita();

    // Zera os contadores (chamado no 'avanca')
    void descansar();
};

#endif // JARDINEIRO_H