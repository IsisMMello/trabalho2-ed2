#include "Bplus.h"
#include <stdio.h>
#include <stdlib.h>

// funções para a página
Pagina *criaPagina();
void inicializarPagina(Pagina *pagina, int indice, int tipo);
void destroiPagina(Pagina *p);
void inserirElemento(Pagina *p, Chave chave);
void removerElemento(Pagina *p, Chave chave);
void verificarOverflow(Pagina *p);
void verificarUnderflow(Pagina *p);
void ordenarPaginaFolha(Pagina *p);
int buscarPaginaLivre(){
    //abre o arquivo pra leitura
    FILE *arquivo = fopen(arquivoArvore, "rb+");
    //pula o cabeçalho
    fseek(arquivo, sizeof(cabeçalho), SEEK_SET);
    //le pagina a pagina até encontrar uma que foi deletada logicamente
    Pagina p;
    int i = 0;
    while(fread(&p, sizeof(Pagina), 1, arquivo)){
        if (p.foiDeletada)
            break;
        i++;
    }
    //fecha o arquivo
    fclose(arquivo);
    //retorna indice da página já deletada
    return i;
}

// funções para a árvore
void inicializarArvore();
void imprimirArvore();
int buscarChave(Chave chave, int *indexPagina);
void inserirChave(Chave chave, int enderecoRegistro);
void deletarChave(Chave chave);
void imprimirChavesIntervalo(Chave inicial, Chave final);