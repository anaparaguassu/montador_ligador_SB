# Trabalho 1 - Montador

**Autora:**  
Ana Luísa de Souza Paraguassu - 231003442

**Ambiente de Desenvolvimento:**  
- Sistema Operacional: Linux via WSL (Windows Subsystem for Linux)
- Compilador: GCC, utilizando o comando `g++` por se tratar de código em C++

---

## Compilação

Abra o terminal na pasta onde está o arquivo fonte e execute o comando abaixo para compilar o programa:

    g++ montador.cpp -o montador

---

## Modos de Execução

Após a compilação, o programa atua em três modos distintos, dependendo da extensão do arquivo passado como argumento. A execução deve seguir a ordem abaixo:

### 1. Modo Pré-processador

Gera um arquivo pré-processado (`.pre`), realizando a limpeza de comentários, remoção de espaços excedentes, conversão para maiúsculas, resolução de diretivas `EQU` e `IF` e reordenação das seções.

    ./montador arquivo.asm

**Saída gerada:**  
- `arquivo.pre`

---

### 2. Modo Montador

Gera o código objeto (`.obj`) e o arquivo de pendências (`.pen`) utilizando o algoritmo de passagem única com backpatching.

    ./montador arquivo.pre

**Saídas geradas:**  
- `arquivo.obj`
- `arquivo.pen`

---

### 3. Modo Simulador

Executa o código de máquina na memória da máquina hipotética, tratando entradas via teclado e saídas no monitor.

    ./montador arquivo.obj

---

## Funcionalidades Implementadas

- **Passagem única:** resolução de referências futuras por meio de listas encadeadas.
- **Instruções suportadas:** conjunto completo de 14 instruções: `ADD`, `SUB`, `MUL`, `DIV`, `JMP`, `JMPN`, `JMPP`, `JMPZ`, `COPY`, `LOAD`, `STORE`, `INPUT`, `OUTPUT` e `STOP`.
- **Diretivas suportadas:** `CONST`, `SPACE`, `EQU` e `IF`.
- **Tratamento sintático:** suporte a rótulos isolados e formatação rigorosa da instrução `COPY`

---

## Observações

- O fluxo de execução é sequencial: `.asm → .pre → .obj`
- O projeto contempla as etapas de pré-processamento, montagem e simulação de uma máquina hipotética
- O uso de backpatching permite resolver referências futuras sem necessidade de múltiplas passagens