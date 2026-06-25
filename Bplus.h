#define ORDEM 4
#include <stdbool.h>

typedef struct {
    char valor[100];
} Chave;

typedef struct Pagina {
    int Chave[ORDEM + 1];
    int filho[ORDEM + 2];
    int pai; //pai do vetor de Chave
    int indice; //endereço da página
    int proximaFolha; //endereço irma adjacente
    int qtElementos; //qt elementos ocupados 
    int ehfolha;
    int foiDeletada; //marcador lógico
} Pagina;

typedef struct {
    long raiz_rid;          // Onde a raiz da árvore está no arquivo (RID)
    long topo_lista_livre;  // Ponteiro para reuso de nós deletados (isso o GPT que fez, funciona como uma lista emcadeada
                            // é uma lista encadeada de nós livres, cada nó livre tem um ponteiro para o próximo nó livre
    int tamanho_Chave;      // Configurado na inicialização (ex: sizeof(Chave))
    int tamanho_dado;       // Configurado na inicialização (ex: sizeof(Funcionario))
} CabecalhoArquivo;

// funções para a página
Pagina *criaPagina(int ordem);
void inicializarPagina(Pagina *Pagina, int ordem, int index, int tipo);
void destroiPagina(Pagina *p);
void inserirElemento(Pagina *p, Chave chave);
void removerElemento(Pagina *p, Chave chave);
void verificarOverflow(Pagina *p);
void verificarUnderflow(Pagina *p);
void ordenarPaginaFolha(Pagina *p);

// funções para a árvore
void inicializarArvore();
void imprimirArvore();
int buscarChave(Chave chave, int *indexPagina);
void inserirChave(Chave chave, int enderecoRegistro);
void deletarChave(Chave chave);
void imprimirChavesIntervalo(Chave inicial, Chave final);