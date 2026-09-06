# TrabOEA - Indexação de Arquivos por Tabela de Dispersão (Hash)

Trabalho da disciplina Organização e Estrutura de Arquivos ministrada pelo Prof. Renato Mauro no Centro Federal de Educação Tecnológica Celso Suckow da Fonseca (CEFET/RJ) - Unidade Maracanã.

    Curso: Curso Superior de Tecnologia em Sistemas para Internet (CST-SI)
    Discente: José Américo Rodrigues
    Período: 2016.1 (Junho de 2016)

---

## Visão Geral

A finalidade deste trabalho é o aprendizado prático na manipulação e indexação de grandes volumes de dados não ordenados armazenados em disco através de **tabelas de dispersão (*Hash Table*)** persistidas em arquivo secundário.

O sistema indexa um arquivo binário de CEPs do Brasil não ordenado (`cep.dat`), gerando um arquivo de índice de acesso direto (`cep_h.idx`). Essa técnica permite realizar consultas de endereços com complexidade temporal média **$O(1)$**, eliminando a necessidade de varredura sequencial exaustiva ($O(N)$) em uma base de dados contendo centenas de milhares de registros.

---

## Estruturas de Dados e Algoritmos

### 1. Registro de Dados (`Endereco`)
Modelado em [`cep.h`](file:///home/joserodrigues/GitHub/OEA/cep.h) com tamanho de registro fixo para leitura rápida e acesso direto em disco via `fseek`:
* `endereco`: 73 bytes
* `bairro`: 73 bytes
* `cidade`: 73 bytes
* `nomeEstado`: 73 bytes
* `siglaEstado`: 3 bytes
* `cep`: 9 bytes

### 2. Registro de Índice Hash (`HashTab`)
Modelado em [`hash.h`](file:///home/joserodrigues/GitHub/OEA/hash.h) para mapeamento da chave de busca para a posição do registro em disco:
* `CEP` (`long`): Chave numérica correspondente ao código postal.
* `PosArq` (`long`): Posição/offset em bytes onde o registro de endereço correspondente está alocado dentro de `cep.dat`.
* `Proximo` (`long`): Ponteiro posicional para o próximo registro encadeado na área de colisão (tratamento de colisões por encadeamento em arquivo).

### 3. Parâmetros e Estratégia de Dispersão
* **Dimensão da Tabela (`MAXHASH`)**: `900001` (número primo dimensionado para minimizar colisões na distribuição dos CEPs).
* **Tratamento de Colisões**: Encadeamento externo em arquivo, com controle monitorado de até `MAXCOLISOES 15`.
* **Arquivos Utilizados**:
  * `cep.dat`: Arquivo principal contendo a base bruta de CEPs não ordenados.
  * `cep_h.idx`: Arquivo binário da tabela hash indexada.
  * `cep_h.log`: Arquivo de log e diagnóstico da geração dos índices.

---

## Módulos do Sistema

O projeto é estruturado em linguagem C em quatro módulos complementares:

* **[`main.c`](file:///home/joserodrigues/GitHub/OEA/main.c)**: Ponto de entrada do programa, gerenciamento do ciclo de abertura/fechamento dos arquivos e loop do menu principal.
* **[`cep.c`](file:///home/joserodrigues/GitHub/OEA/cep.c) / [`cep.h`](file:///home/joserodrigues/GitHub/OEA/cep.h)**: Rotinas de baixo nível para manuseio do arquivo de CEPs (`abreCep`, `leCep`, `imprimeCep`, `fechaCep`).
* **[`hash.c`](file:///home/joserodrigues/GitHub/OEA/hash.c) / [`hash.h`](file:///home/joserodrigues/GitHub/OEA/hash.h)**: Implementação da função hash (`calculaHash`), rotina de indexação (`indexaHash`), busca direta (`pesquisaHash`), escrita/leitura posicional no arquivo hash e cálculo de métricas estatísticas (`estatisticasHash`).
* **[`util.c`](file:///home/joserodrigues/GitHub/OEA/util.c) / [`util.h`](file:///home/joserodrigues/GitHub/OEA/util.h)**: Rotinas utilitárias de navegação (menu de terminal, limpeza de tela e pausas).

---

## Funcionalidades do Menu

1. **Consultar CEP**: Busca imediata em tempo constante via índice hash, exibindo os dados completos de logradouro, bairro, cidade e estado.
2. **Verificar Estatísticas**: Métricas da tabela de dispersão (quantidade de registros indexados, total de colisões e taxa de ocupação).
3. **Listar CEPs**: Leitura sequencial do arquivo mestre `cep.dat`.
4. **Listar Tabela Hash**: Inspeção direta das entradas gravadas no arquivo de índice `cep_h.idx`.
5. **Ler Informações de um Registro Hash**: Consulta a uma posição pontual da tabela hash para fins de auditoria acadêmica.

---

## Instruções de Compilação e Linkedição

### Compilação Modular (Padrão GCC / MinGW)

Execute no terminal (dentro da pasta do projeto):

```cmd
gcc.exe -c cep.c -o cep.o
gcc.exe -c hash.c -o hash.o
gcc.exe -c main.c -o main.o
gcc.exe -c util.c -o util.o
gcc.exe cep.o hash.o main.o util.o -o OEA.exe
```

### Compilação Direta (Comando Único)

```cmd
gcc cep.c hash.c main.c util.c -o OEA.exe
```

> [!NOTE]
> Para executar a aplicação, certifique-se de que o arquivo binário `cep.dat` esteja presente na mesma pasta do executável `OEA.exe`.