/*
 * ============================================================
 * ARQUIVO: Bplus.h
 * ============================================================
 * 
 * INTERFACE PÚBLICA DA ÁRVORE B+ GENÉRICA EM DISCO
 * 
 * ------------------------------------------------------------
 * PASSO 1 – CONSTANTES GLOBAIS
 * ------------------------------------------------------------
 * 1.1. Definir ORDEM da árvore (número máximo de chaves por nó).
 * 1.2. Definir TAMANHO_PAGINA (em bytes) para alinhamento com disco.
 * 1.3. Definir valores para RID_NULO (ponteiro de disco inválido).
 * 
 * ------------------------------------------------------------
 * PASSO 2 – ESTRUTURAS DOS NÓS (REPRESENTAÇÃO EM MEMÓRIA)
 * ------------------------------------------------------------
 * 2.1. Struct do nó INTERNO:
 *      - vetor de chaves (genérico, void*)
 *      - vetor de RIDs (filhos)
 *      - contador de chaves
 *      - RID do próprio nó (para persistência)
 * 2.2. Struct do nó FOLHA:
 *      - vetor de chaves (genérico, void*)
 *      - vetor de RIDs (apontando para os registros de dados)
 *      - RID do próximo nó folha (encadeamento para range scan)
 *      - contador de chaves
 *      - RID do próprio nó
 * 2.3. Struct do CABEÇALHO do arquivo de índice:
 *      - RID da raiz
 *      - RID da primeira folha
 *      - RID da lista de blocos livres
 *      - assinatura/versão do arquivo
 * 
 * ------------------------------------------------------------
 * PASSO 3 – TIPOS DAS FUNÇÕES DE CALLBACK (GENÉRICO)
 * ------------------------------------------------------------
 * 3.1. Definir tipo para comparador: int (*cmp)(const void*, const void*)
 * 3.2. Definir tipo para calculador de tamanho em bytes: size_t (*size)(const void*)
 * 3.3. Definir tipo para serializador: void (*write)(const void*, void* buffer)
 * 3.4. Definir tipo para desserializador: void (*read)(void* destino, const void* buffer)
 * 
 * ------------------------------------------------------------
 * PASSO 4 – PROTÓTIPOS DA API PÚBLICA
 * ------------------------------------------------------------
 * 4.1. Inicializar / abrir arquivo de índice: bplus_init()
 * 4.2. Inserir chave + RID do dado: bplus_insert()
 * 4.3. Buscar por igualdade: bplus_search()
 * 4.4. Busca por intervalo (range): bplus_range_search()
 * 4.5. Remover chave: bplus_delete()
 * 4.6. Imprimir estrutura hierárquica: bplus_print_tree()
 * 4.7. Fechar arquivo e liberar recursos: bplus_close()
 * 
 * ------------------------------------------------------------
 * PASSO 5 – FUNÇÕES AUXILIARES INTERNAS (NÃO EXPORTADAS)
 * ------------------------------------------------------------
 * 5.1. Localizar folha para uma chave (search_leaf)
 * 5.2. Dividir (split) nó interno / folha
 * 5.3. Redistribuir ou concatenar (merge) nós na remoção
 * 5.4. Atualizar cabeçalho do arquivo
 */

#define ORDEM 255
#define TAMANHO_PAGINA 4096
#include <stdbool.h>
#include "Rh.h"


// rid funciona como um ponteiro de memoria em disco, se um dado esta armazenado no bite n do arquivo, o rid é n
// Se o dado não existe, o rid é -1
typedef struct pagina {
    bool ehfolha;
    int numChaves;
    funcionario chaves[ORDEM - 1];
    long rids[ORDEM]; // RIDs dos filhos 
    long proximaFolha;   // Apenas para folhas: Tem que ser long e não void* para armazenar o RID do próximo nó folha
    int foiDeletado;
} pagina;

typedef struct {
    long raiz_rid;          // Onde a raiz da árvore está no arquivo (RID)

    long topo_lista_livre;  // Ponteiro para reuso de nós deletados (isso o GPT que fez, funciona como uma lista emcadeada
                            // é uma lista encadeada de nós livres, cada nó livre tem um ponteiro para o próximo nó livre)
   
    int tamanho_chave;      // Configurado na inicialização (ex: sizeof(ChaveComposta))
    int tamanho_dado;       // Configurado na inicialização (ex: sizeof(Funcionario))
} CabecalhoArquivo;

void criaArvoreBMais(char* nomeArquivo, size_t tamanhoChave, size_t tamanhoDado);
int lerPagina(FILE* arquivo, long rid, pagina *buffer_no);
int escreverPagina(FILE* arquivo, long rid, const pagina *no);