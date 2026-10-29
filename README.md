# Garden Simulator

O **Garden Simulator** é uma aplicação de consola desenvolvida em C++ que simula a gestão e evolução de um ecossistema de jardim ao longo do tempo[cite: 6, 7]. O sistema opera segundo uma lógica baseada em turnos discretos ("instantes"), permitindo a interação através de comandos textuais para manipular o terreno, gerir espécies botânicas e utilizar ferramentas agrícolas[cite: 7, 12].

## Contexto Académico

Este projeto foi desenvolvido no âmbito da unidade curricular de **Programação Orientada a Objetos (POO)** da Licenciatura em Engenharia Informática no **Instituto Superior de Engenharia de Coimbra (ISEC - DEIS)**, referente ao ano letivo 2025/2026[cite: 7, 17].

O objetivo pedagógico central consiste na aplicação prática dos pilares da programação orientada a objetos — encapsulamento, composição, herança e polimorfismo — aliados a uma gestão rigorosa de memória dinâmica em C++[cite: 7, 20, 21]. Por restrição de desenho arquitetural, a estrutura espacial do terreno não recorre a contentores da biblioteca padrão (*Standard Template Library*), sendo implementada através de alocação dinâmica direta[cite: 8, 15, 20].

## Arquitetura e Tecnologias

* **Linguagem e Norma:** C++ (Standard C++20)[cite: 72].
* **Sistema de Build:** CMake (versão mínima 3.20)[cite: 72].
* **Estrutura de Memória e Composição:** A grelha do jardim (`Jardim`) é representada por uma matriz dinâmica bidimensional de objetos `Celula` (`Celula**`), garantindo que apenas a memória estritamente necessária para as dimensões especificadas é alocada[cite: 8, 20]. A classe `Jardim` gere o ciclo de vida do `Jardineiro` e das células por composição, assegurando a libertação integral de recursos nos destrutores e a cópia profunda (*deep copy*) através do construtor de cópia[cite: 20, 80].
* **Hierarquia Polimórfica:**
  * **Plantas (`Planta`):** Classe base abstrata que define a interface comum (`atualizar`, `duplicar`) para as classes derivadas `Cacto`, `Roseira`, `ErvaDaninha` e `PlantaExotica`[cite: 21, 92].
  * **Ferramentas (`Ferramenta`):** Classe base abstrata que gere a atribuição automática de números de série sequenciais e define o comportamento polimórfico (`usar`, `duplicar`) para `Regador`, `Adubo`, `TesouraPoda` e `FerramentaZ`[cite: 10, 21, 82, 96].
* **Configuração Centralizada:** Todos os parâmetros numéricos de simulação encontram-se encapsulados na classe `Settings` (`Settings.h`)[cite: 9, 15, 71].

---

## Regras e Mecânicas de Simulação

### 1. O Jardim e o Solo
* O jardim possui um formato retangular com dimensões definidas no início da simulação, até ao limite máximo de 26x26 posições[cite: 8].
* As coordenadas são identificadas por pares de letras minúsculas correspondentes à linha e à coluna (exemplo: `aa` representa o canto superior esquerdo; `ce` representa a linha 3, coluna 5)[cite: 8]. Na consola, a grelha é contornada por uma régua impressa em letras maiúsculas[cite: 8, 104].
* Cada célula de solo inicia com valores aleatórios de **água** (entre 80 e 100 unidades) e **nutrientes** (entre 40 e 50 unidades)[cite: 9, 71, 104].
* Cada posição pode albergar simultaneamente até 1 planta, 1 ferramenta e o jardineiro[cite: 9]. A sobreposição visual na consola respeita a seguinte ordem de prioridade: **Jardineiro (`*`) > Planta (`c`, `r`, `e`, `x`) > Ferramenta (`g`, `a`, `t`, `z`) > Solo vazio (` `)**[cite: 8, 9, 10, 11, 85, 104].

### 2. O Jardineiro
* O jardineiro pode encontrar-se dentro ou fora do jardim[cite: 9]. Quando posicionado na grelha, é representado pelo carácter `*`[cite: 9, 104].
* Entre cada avanço de instante (por turno), o jardineiro está sujeito aos seguintes limites de ações:
  * **Movimentos:** Até 10 deslocações ortogonais (`c`, `b`, `e`, `d`)[cite: 14, 71].
  * **Entradas e Saídas:** 1 entrada e 1 saída do jardim por turno[cite: 14, 71, 79].
  * **Plantações:** Até 2 novas plantas colocadas por turno[cite: 13, 71].
  * **Colheitas:** Até 5 plantas removidas por turno[cite: 13, 71].
* Ao avançar o tempo (`avanca`), os contadores de ações do turno são reiniciados[cite: 79, 104].

### 3. Comportamento das Espécies de Plantas

| Espécie | Símbolo | Beleza | Comportamento por Instante | Condição de Multiplicação | Condição de Morte e Efeito no Solo |
| :--- | :---: | :---: | :--- | :--- | :--- |
| **Cacto** | `c` | Neutra | Absorve 25% da água do solo e até 5 unidades de nutrientes[cite: 10, 86, 100]. | Água acumulada > 50 e nutrientes > 100 (divide reservas em partes iguais com o novo cacto numa posição vizinha livre)[cite: 11, 86]. | Morre se água no solo > 100 por 3 instantes seguidos ou nutrientes no solo == 0 por mais de 3 instantes seguidos[cite: 10, 86]. Devolve ao solo todos os nutrientes absorvidos em vida[cite: 11, 86]. |
| **Roseira** | `r` | Bonita | Inicia com 25 de água e 25 de nutrientes[cite: 11, 74]. Perde 4 unidades de água e 4 de nutrientes; absorve até 5 de água e 8 de nutrientes do solo[cite: 11, 74, 89]. | Nutrientes acumulados > 100 numa célula vizinha livre (a nova inicia com 25 nutrientes e metade da água; a original fica com 100 nutrientes e metade da água)[cite: 11, 74]. | Morre se água interna <= 0, nutrientes internos <= 0, nutrientes >= 200, ou se todas as células ortogonais válidas estiverem ocupadas por plantas[cite: 11, 74]. Deixa no solo metade da água e dos nutrientes absorvidos em vida[cite: 11, 74]. |
| **Erva Daninha** | `e` | Feia | Inicia com 5 de água e 5 de nutrientes[cite: 11, 83]. Absorve até 1 unidade de água e 1 de nutrientes do solo[cite: 11, 83, 97]. | Nutrientes > 30 e mínimo de 5 instantes desde a última multiplicação[cite: 11, 83]. Invade uma célula vizinha, eliminando qualquer planta lá presente[cite: 11, 83]. | Morre automaticamente ao atingir 60 instantes de idade[cite: 11, 83]. |
| **Planta Exótica** | `x` | Bonita | Inicia com 20 de água e 20 de nutrientes[cite: 76]. A cada 5 instantes consome 10 unidades da sua água interna para injetar 15 unidades de nutrientes no solo[cite: 11, 76, 91]. | Não aplicável[cite: 76, 91]. | Morre se a água interna ou nutrientes internos chegarem a 0[cite: 76]. |

### 4. Ferramentas
* No momento de criação do jardim, são geradas 3 ferramentas aleatórias em posições aleatórias[cite: 9, 104]. Sempre que o jardineiro se move para uma posição que contém uma ferramenta, esta é recolhida automaticamente para o seu inventário e uma nova ferramenta aleatória surge no jardim[cite: 9, 10, 104].
* O jardineiro pode transportar um número ilimitado de ferramentas na mochila, mas apenas uma pode estar equipada "na mão"[cite: 9, 79]. A ferramenta equipada atua automaticamente sobre a posição atual do jardineiro a cada passagem de instante[cite: 9, 104].
* **Tipos de Ferramentas:**
  * **Regador (`g`):** Capacidade inicial de 200 unidades de água[cite: 10, 71, 75]. Aumenta em 10 unidades a água do solo por instante até se esgotar e ser descartado[cite: 10, 75].
  * **Pacote de Adubo (`a`):** Capacidade inicial de 100 unidades[cite: 10, 71, 87]. Aumenta em 10 unidades os nutrientes do solo por instante até se esgotar e ser descartado[cite: 10, 87].
  * **Tesoura de Poda (`t`):** Durabilidade infinita[cite: 10, 73, 88]. Elimina plantas classificadas como "Feias" (como a Erva Daninha) presentes na posição do jardineiro[cite: 10, 73, 88].
  * **FerramentaZ (`z`):** Possui 3 utilizações[cite: 81]. Ao atuar, limpa todas as plantas localizadas nas 4 posições ortogonalmente adjacentes ao jardineiro[cite: 10, 81].

---

## Instruções de Utilização e Comandos

A interação decorre através da introdução de comandos de texto em letras minúsculas[cite: 12]. O comando `jardim <linhas> <colunas>` (ou `executa <ficheiro>`) deve obrigatoriamente preceder todos os restantes comandos de jogo[cite: 14, 84].

### Configuração, Tempo e Sistema
* `jardim <linhas> <colunas>` — Cria o espaço do jardim com as dimensões indicadas (entre 1 e 26)[cite: 8, 14, 84]. Só pode ser executado uma única vez[cite: 14, 84].
* `avanca [n]` — Avança `n` instantes temporais na simulação (assume `1` caso o parâmetro seja omitido)[cite: 12, 84].
* `grava <nome>` — Guarda em memória uma cópia integral (*deep copy*) do estado atual do jardim associada ao identificador `<nome>`[cite: 14, 80, 84].
* `recupera <nome>` — Restaura o estado do jardim a partir da gravação indicada e remove essa gravação da memória[cite: 14, 84].
* `apaga <nome>` — Elimina da memória a gravação associada ao identificador `<nome>`[cite: 14, 84].
* `executa <nome-do-ficheiro>` — Lê e executa sequencialmente comandos a partir de um ficheiro de texto[cite: 14, 84].
* `fim` — Encerra a simulação e liberta toda a memória alocada dinamicamente[cite: 15, 84].

### Movimentação do Jardineiro
* `entra <l><c>` — Coloca o jardineiro na coordenada `<l><c>` (ou teletransporta-o, caso já se encontre dentro do jardim)[cite: 14, 84, 104].
* `sai` — Retira o jardineiro da área do jardim[cite: 14, 84].
* `c`, `b`, `e`, `d` — Desloca o jardineiro uma célula para cima (`c`), baixo (`b`), esquerda (`e`) ou direita (`d`)[cite: 14, 79, 84].

### Ações de Plantação e Ferramentas
* `planta <l><c> <tipo>` — Planta uma espécie (`c`, `r`, `e`, `x`) na coordenada indicada[cite: 13, 84, 104].
* `colhe <l><c>` — Remove a planta presente na coordenada `<l><c>`[cite: 13, 84].
* `compra <tipo>` — Adquire uma ferramenta (`g`, `a`, `t`, `z`) e adiciona-a diretamente ao inventário do jardineiro[cite: 13, 84, 104].
* `pega <n>` — Equipa na mão do jardineiro a ferramenta do inventário com o número de série `<n>`[cite: 13, 79, 84].
* `larga` — Guarda na mochila a ferramenta que o jardineiro tem atualmente na mão[cite: 13, 79, 84].

### Consulta e Listagem de Informação
* `lplantas` — Lista todas as plantas vivas no jardim, detalhando as suas propriedades internas e o estado do solo respetivo[cite: 12, 84, 104].
* `lplanta <l><c>` — Exibe os atributos detalhados da planta situada na posição `<l><c>`[cite: 12, 84, 104].
* `larea` — Lista todas as células não vazias do jardim e o respetivo conteúdo[cite: 13, 84, 104].
* `lsolo <l><c> [n]` — Apresenta os detalhes do solo e elementos presentes na coordenada `<l><c>`[cite: 13, 84, 104]. Caso seja especificado o inteiro `n`, lista todas as células compreendidas num quadrado de raio `n` centrado nessa posição[cite: 13, 84, 104].
* `lferr` — Lista todas as ferramentas transportadas pelo jardineiro, indicando o número de série, estado de conservação e qual se encontra equipada na mão[cite: 13, 79, 84].

---

## Compilação e Execução

O projeto pode ser compilado através do **CMake** (em ambientes como o CLion ou terminal) ou diretamente através de um compilador compatível com a norma **C++20** (como `g++` ou `clang++`)[cite: 15, 72].

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