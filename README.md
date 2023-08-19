# Calculadora em Notação RPN (Polonesa Reversa)

Este é um programa desenvolvido em C++ para avaliar expressões aritméticas em notação RPN (Polonesa Reversa) a partir de um arquivo de texto. O programa lê o arquivo e calcula o resultado das expressões definidas no formato RPN. Além disso, ele suporta três comandos especiais: (N RES), (V MEM) e (MEM).

## Requisitos

- Compilador C++ (Suporte a C++11 ou superior)
- Um arquivo de texto contendo as expressões em notação RPN

## Formato do Arquivo de Entrada

O arquivo de entrada deve conter expressões aritméticas em notação RPN, uma por linha, seguindo o formato a seguir:

- Adição: `+` no formato `(+ A B)`;
- Subtração: `-` no formato `(- A B)`;
- Multiplicação: `*` no formato `(* A B)`;
- Divisão Real: `|` no formato `(| A B)`;
- Divisão de inteiros: `/` no formato `(/ A B)`;
- Resto da Divisão de Inteiros: `%` no formato `(% A B)`;
- Potenciação: `^` no formato `(^ A B)`;
- Radiciação (Raiz Quadrada): `?` no formato `(? A)`;
- Inversão de Sinal: `@` no formato `(@ A)`.

Onde `A` e `B` representam números reais ou inteiros. Use o ponto (`.`) para indicar a vírgula decimal. As expressões podem ser aninhadas para criar novas expressões.

### Comandos Especiais

Além das expressões, o arquivo de entrada pode conter três comandos especiais:

- `(N RES)`: Retorna o resultado da expressão que está `N` linhas antes, onde `N` é um número inteiro não negativo.
- `(V MEM)`: Armazena um valor, `V`, em um espaço de memória chamado de MEM, capaz de armazenar um valor real.
- `(MEM)`: Retorna o valor armazenado na memória. Se a memória não tiver sido usada, devolve o número real zero.

## Compilando o Programa

Você pode compilar o programa usando um compilador C++ compatível com C++11 ou superior. Use o seguinte comando:

```shell
g++ -std=c++11 calculadora_rpn.cpp -o calculadora_rpn
```

## Executando o Programa

Após compilar o programa, você pode executá-lo da seguinte maneira:

```shell
./calculadora_rpn arquivo_de_entrada.txt
```

Substitua `arquivo_de_entrada.txt` pelo nome do arquivo que contém as expressões em notação RPN que deseja avaliar.

No repl.it:
```repl
./main teste1.txt
```
## Exemplo de Uso

Suponha que você tenha um arquivo chamado `expressoes.txt` com o seguinte conteúdo:

```
(+ 5 3)
(* 2 4) 
(/ 3 2)
```

Você pode executar o programa da seguinte forma:

```shell
./calculadora_rpn expressoes.txt
```

O programa calculará as expressões e exibirá os resultados:

```
L1: 8.0
L2: 8.0
L3: 1.5
```

Os resultados das expressões são exibidos no formato "Lx: resultado", onde "x" é o número da linha da expressão no arquivo de entrada.

## Observações

- Todas as operações são executadas sobre números reais de precisão dupla, exceto a divisão de inteiros e a operação de Resto da Divisão de Inteiros.
- O expoente da operação de potenciação será sempre um inteiro positivo.
- Certifique-se de que o arquivo de entrada esteja formatado corretamente de acordo com as regras mencionadas.

Este programa foi desenvolvido como parte de um projeto da disciplina "Linguagens Formais e Compiladores" sob a orientação do professor Frank Alcantara.