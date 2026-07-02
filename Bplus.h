#ifndef Bplus_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 4
#define PAGE_SIZE 4096

//Estruturas e constantes

typedef struct {
    int ordem;          // ordem da árvore
    int qtdPaginas;     // quantidade de páginas existentes
    int raiz;           // índice da raiz (-1 = árvore vazia)
    size_t tamChave;    // tamanho, em bytes, de cada chave
} Cabecalho;

typedef struct Pagina{
    void* chave[ORDEM + 1];
    int filho[ORDEM + 2];
    int pai; //pai do vetor de Chave
    int indice; //endereço da página
    int proximaFolha; //endereço irma adjacente
    int qtElementos; //qt elementos ocupados 
    int ehfolha; // 0 == pag interna e 1 == pag folha
    int foiDeletada; //marcador lógico
} Pagina;

/* ============================================================
   CATEGORIA 1: Criação, inicialização e busca de páginas livres
   ============================================================ */

// Cria uma nova página na memória (aloca e retorna ponteiro)
Pagina *criaPagina();

// Inicializa os campos de uma página (índice, tipo folha/interna, etc.)
void inicializarPagina(Pagina *pagina, int indice, int tipo);

// Busca no arquivo uma página que foi marcada como deletada (para reuso)
int buscarPaginaLivre();

// Marca uma página como deletada no disco e libera memória
void destroiPagina(Pagina *p);

/* ============================================================
   CATEGORIA 1.1: Serialização de páginas
   ============================================================ */

// Grava uma página em disco utilizando serialização manual
void gravarPagina(FILE *arquivo, Pagina *pagina, const Cabecalho *cab);

// Lê uma página do disco reconstruindo suas estruturas em memória
void lerPagina(FILE *arquivo, Pagina *pagina, int indice, const Cabecalho *cab);

// Libera a memória alocada para as chaves de uma página
void liberarPagina(Pagina *pagina);

/* ============================================================
   CATEGORIA 2: Ordenação e manipulação de elementos na página
   ============================================================ */

// Ordena os elementos de uma página folha usando insertion sort (função de comparação genérica)
void ordenarPaginaFolha(Pagina *p, int (*comparar)(const void *, const void *));

// Ordena os elementos de uma página interna usando insertion sort (função de comparação genérica)
void ordenarPaginaInterna(Pagina *p, int (*comparar)(const void *, const void *));

// Insere uma chave e seu índice (registro/filho) na página, mantendo ordem e tratando overflow
void inserirElementoNaPagina(Pagina *p, const void *chave, size_t tamChave, int indice, int (*comparar)(const void *, const void *));

// Remove uma chave da página e reorganiza os elementos (retorna 0 se não encontrada, ou outro valor)
int removerElementoDaPagina(Pagina *p, const void *chave, int (*comparar)(const void*, const void*));

/* ============================================================
   CATEGORIA 3: Balanceamento (redistribuição, concatenação, underflow/overflow)
   ============================================================ */

// Tenta redistribuir chaves entre a página e uma irmã (esquerda ou direita) para evitar underflow
int redistribuir(FILE *arquivo, Pagina *pagina, Pagina *pai, int pos, int minimo, int (*comparar)(const void *, const void *));

// Concatena a página com uma irmã (quando redistribuição não é possível)
void concatenar(FILE *arquivo, Pagina *pagina, Pagina *pai, int pos, int (*comparar)(const void*, const void*));

// Verifica se uma página está com underflow e aciona redistribuição ou concatenação
void verificarUnderflow(FILE *arquivo, Pagina *pagina, int (*comparar)(const void*, const void*));

// Verifica se uma página está com overflow e realiza a cisão (split), propagando a chave mediana
void verificarOverflow(Pagina *p, int (*comparar)(const void *, const void *));

/* ============================================================
   CATEGORIA 4: Funções principais da árvore (inicialização, busca, impressão)
   ============================================================ */

void inserirChaveNaArvore(const void *chave, int enderecoRegistro, size_t tamChave, int (*comparar)(const void*, const void*));
   
// Inicializa o arquivo da árvore com um cabeçalho (ordem, tamanho da chave, etc.)
void inicializarArvore(int ordem, int tamChave);


// Retorna a página folha onde a chave deveria estar (navegação a partir da raiz)
Pagina buscarFolha(Cabecalho *header, const void *chave, int (*comparar)(const void *, const void *));

// Busca uma chave específica na árvore e retorna o endereço do registro associado
int buscarChaveNaArvore(const void* chave, int *enderecoRegistro, int (*comparar)(const void*, const void*));

// Busca todas as chaves em um intervalo [chaveMin, chaveMax] e retorna um vetor de endereços
int* buscarChavesIntervalo(const void *chaveMin, const void *chaveMax, int *qtEncontrados, int (*comparar)(const void*, const void*));

// Deleta uma determinada chave da árvore
void deletarChaveNaArvore(const void *chave, int (*comparar)(const void *, const void *));

#endif