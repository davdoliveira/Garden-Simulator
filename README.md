# Garden Simulator

O **Garden Simulator** é uma aplicação de consola desenvolvida em C++ que simula a gestão e evolução de um ecossistema de jardim ao longo do tempo. O sistema opera segundo uma lógica baseada em turnos discretos ("instantes"), permitindo a interação através de comandos textuais para manipular o terreno, gerir espécies botânicas e utilizar ferramentas agrícolas.

## Contexto Académico

Este projeto foi desenvolvido no âmbito da unidade curricular de **Programação Orientada a Objetos (POO)** da Licenciatura em Engenharia Informática no **Instituto Superior de Engenharia de Coimbra (ISEC - DEIS)**, referente ao ano letivo 2025/2026.

O objetivo pedagógico central consiste na aplicação prática dos pilares da programação orientada a objetos — encapsulamento, composição, herança e polimorfismo — aliados a uma gestão rigorosa de memória dinâmica em C++. Por restrição de desenho arquitetural, a estrutura espacial do terreno não recorre a contentores da biblioteca padrão, sendo implementada através de alocação dinâmica direta.

## Arquitetura e Tecnologias

* **Linguagem e Norma:** C++ 
* **Estrutura de Memória e Composição:** A grelha do jardim (`Jardim`) é representada por uma matriz dinâmica bidimensional de objetos `Celula` (`Celula**`), garantindo que apenas a memória estritamente necessária para as dimensões especificadas é alocada. A classe `Jardim` gere o ciclo de vida do `Jardineiro` e das células por composição, assegurando a libertação integral de recursos nos destrutores e a cópia profunda (*deep copy*) através do construtor de cópia.
* **Hierarquia Polimórfica:**
  * **Plantas (`Planta`):** Classe base abstrata que define a interface comum (`atualizar`, `duplicar`) para as classes derivadas `Cacto`, `Roseira`, `ErvaDaninha` e `PlantaExotica`.
  * **Ferramentas (`Ferramenta`):** Classe base abstrata que gere a atribuição automática de números de série sequenciais e define o comportamento polimórfico (`usar`, `duplicar`) para `Regador`, `Adubo`, `TesouraPoda` e `FerramentaZ`.
* **Configuração Centralizada:** Todos os parâmetros numéricos de simulação encontram-se encapsulados na classe `Settings` (`Settings.h`).

---

## Regras e Mecânicas de Simulação

### 1. O Jardim e o Solo
* O jardim possui um formato retangular com dimensões definidas no início da simulação, até ao limite máximo de 26x26 posições.
* As coordenadas são identificadas por pares de letras minúsculas correspondentes à linha e à coluna (exemplo: `aa` representa o canto superior esquerdo; `ce` representa a linha 3, coluna 5). Na consola, a grelha é contornada por uma régua impressa em letras maiúsculas.
* Cada célula de solo inicia com valores aleatórios de **água** (entre 80 e 100 unidades) e **nutrientes** (entre 40 e 50 unidades).
* Cada posição pode albergar simultaneamente até 1 planta, 1 ferramenta e o jardineiro. A sobreposição visual na consola respeita a seguinte ordem de prioridade: **Jardineiro (`*`) > Planta (`c`, `r`, `e`, `x`) > Ferramenta (`g`, `a`, `t`, `z`) > Solo vazio (` `)**.

### 2. O Jardineiro
* O jardineiro pode encontrar-se dentro ou fora do jardim. Quando posicionado na grelha, é representado pelo carácter `*`.
* Entre cada avanço de instante (por turno), o jardineiro está sujeito aos seguintes limites de ações:
  * **Movimentos:** Até 10 deslocações ortogonais (`c`, `b`, `e`, `d`).
  * **Entradas e Saídas:** 1 entrada e 1 saída do jardim por turno.
  * **Plantações:** Até 2 novas plantas colocadas por turno.
  * **Colheitas:** Até 5 plantas removidas por turno.
* Ao avançar o tempo (`avanca`), os contadores de ações do turno são reiniciados.

### 3. Comportamento das Espécies de Plantas

| Espécie | Símbolo | Beleza | Comportamento por Instante | Condição de Multiplicação | Condição de Morte e Efeito no Solo |
| :--- | :---: | :---: | :--- | :--- | :--- |
| **Cacto** | `c` | Neutra | Absorve 25% da água do solo e até 5 unidades de nutrientes. | Água acumulada > 50 e nutrientes > 100 (divide reservas em partes iguais com o novo cacto numa posição vizinha livre). | Morre se água no solo > 100 por 3 instantes seguidos ou nutrientes no solo == 0 por mais de 3 instantes seguidos. Devolve ao solo todos os nutrientes absorvidos em vida. |
| **Roseira** | `r` | Bonita | Inicia com 25 de água e 25 de nutrientes. Perde 4 unidades de água e 4 de nutrientes; absorve até 5 de água e 8 de nutrientes do solo. | Nutrientes acumulados > 100 numa célula vizinha livre (a nova inicia com 25 nutrientes e metade da água; a original fica com 100 nutrientes e metade da água). | Morre se água interna <= 0, nutrientes internos <= 0, nutrientes >= 200, ou se todas as células ortogonais válidas estiverem ocupadas por plantas. Deixa no solo metade da água e dos nutrientes absorvidos em vida. |
| **Erva Daninha** | `e` | Feia | Inicia com 5 de água e 5 de nutrientes. Absorve até 1 unidade de água e 1 de nutrientes do solo. | Nutrientes > 30 e mínimo de 5 instantes desde a última multiplicação. Invade uma célula vizinha, eliminando qualquer planta lá presente. | Morre automaticamente ao atingir 60 instantes de idade. |
| **Planta Exótica** | `x` | Bonita | Inicia com 20 de água e 20 de nutrientes. A cada 5 instantes consome 10 unidades da sua água interna para injetar 15 unidades de nutrientes no solo. | Não aplicável. | Morre se a água interna ou nutrientes internos chegarem a 0. |

### 4. Ferramentas
* No momento de criação do jardim, são geradas 3 ferramentas aleatórias em posições aleatórias. Sempre que o jardineiro se move para uma posição que contém uma ferramenta, esta é recolhida automaticamente para o seu inventário e uma nova ferramenta aleatória surge no jardim.
* O jardineiro pode transportar um número ilimitado de ferramentas na mochila, mas apenas uma pode estar equipada "na mão". A ferramenta equipada atua automaticamente sobre a posição atual do jardineiro a cada passagem de instante.
* **Tipos de Ferramentas:**
  * **Regador (`g`):** Capacidade inicial de 200 unidades de água. Aumenta em 10 unidades a água do solo por instante até se esgotar e ser descartado.
  * **Pacote de Adubo (`a`):** Capacidade inicial de 100 unidades. Aumenta em 10 unidades os nutrientes do solo por instante até se esgotar e ser descartado.
  * **Tesoura de Poda (`t`):** Durabilidade infinita. Elimina plantas classificadas como "Feias" (como a Erva Daninha) presentes na posição do jardineiro.
  * **FerramentaZ (`z`):** Possui 3 utilizações. Ao atuar, limpa todas as plantas localizadas nas 4 posições ortogonalmente adjacentes ao jardineiro.

---

## Instruções de Utilização e Comandos

A interação decorre através da introdução de comandos de texto em letras minúsculas. O comando `jardim <linhas> <colunas>` (ou `executa <ficheiro>`) deve obrigatoriamente preceder todos os restantes comandos de jogo.

### Configuração, Tempo e Sistema
* `jardim <linhas> <colunas>` — Cria o espaço do jardim com as dimensões indicadas (entre 1 e 26). Só pode ser executado uma única vez.
* `avanca [n]` — Avança `n` instantes temporais na simulação (assume `1` caso o parâmetro seja omitido).
* `grava <nome>` — Guarda em memória uma cópia integral (*deep copy*) do estado atual do jardim associada ao identificador `<nome>`.
* `recupera <nome>` — Restaura o estado do jardim a partir da gravação indicada e remove essa gravação da memória.
* `apaga <nome>` — Elimina da memória a gravação associada ao identificador `<nome>`.
* `executa <nome-do-ficheiro>` — Lê e executa sequencialmente comandos a partir de um ficheiro de texto.
* `fim` — Encerra a simulação e liberta toda a memória alocada dinamicamente.

### Movimentação do Jardineiro
* `entra <l><c>` — Coloca o jardineiro na coordenada `<l><c>` (ou teletransporta-o, caso já se encontre dentro do jardim).
* `sai` — Retira o jardineiro da área do jardim.
* `c`, `b`, `e`, `d` — Desloca o jardineiro uma célula para cima (`c`), baixo (`b`), esquerda (`e`) ou direita (`d`).

### Ações de Plantação e Ferramentas
* `planta <l><c> <tipo>` — Planta uma espécie (`c`, `r`, `e`, `x`) na coordenada indicada.
* `colhe <l><c>` — Remove a planta presente na coordenada `<l><c>`.
* `compra <tipo>` — Adquire uma ferramenta (`g`, `a`, `t`, `z`) e adiciona-a diretamente ao inventário do jardineiro.
* `pega <n>` — Equipa na mão do jardineiro a ferramenta do inventário com o número de série `<n>`.
* `larga` — Guarda na mochila a ferramenta que o jardineiro tem atualmente na mão.

### Consulta e Listagem de Informação
* `lplantas` — Lista todas as plantas vivas no jardim, detalhando as suas propriedades internas e o estado do solo respetivo.
* `lplanta <l><c>` — Exibe os atributos detalhados da planta situada na posição `<l><c>`.
* `larea` — Lista todas as células não vazias do jardim e o respetivo conteúdo.
* `lsolo <l><c> [n]` — Apresenta os detalhes do solo e elementos presentes na coordenada `<l><c>`. Caso seja especificado o inteiro `n`, lista todas as células compreendidas num quadrado de raio `n` centrado nessa posição.
* `lferr` — Lista todas as ferramentas transportadas pelo jardineiro, indicando o número de série, estado de conservação e qual se encontra equipada na mão.

---

## Compilação e Execução

O projeto pode ser compilado através do **CMake** (em ambientes como o CLion ou terminal) ou diretamente através de um compilador compatível com a norma **C++20** (como `g++` ou `clang++`).

### Opção 1: Utilizando CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
./tp
```

### Opção 2: Utilizando g++ diretamente na linha de comandos
```bash
g++ -std=c++20 -Wall -Wextra -Werror -I include src/*.cpp -o simulador_jardim
./simulador_jardim
```