#include "Bplus.h"
#include <stdio.h>
#include <stdlib.h>

FILE* criarArvore(const char *nomeArquivo, int tamChave, int tamDado) {
    // tamChave = sizeof(ChaveComposta),
    // tamDado = sizeof(Funcionario)


    // Tenta abrir no modo "rb+" (leitura e escrita se o arquivo já existir)
    FILE *arquivo = fopen(nomeArquivo, "rb+");
    if (arquivo != NULL) {
        return arquivo; // Retorna o arquivo aberto para leitura/escrita mantendo a persistência
    }

    // Se retornou NULL, significa que o arquivo não existe. Vamos criá-lo do zero ("wb+").
    arquivo = fopen(nomeArquivo, "wb+");
    if (arquivo == NULL) {
        return NULL;
    }
    // 1. Configura o Cabeçalho Inicial
    CabecalhoArquivo cabecalho;
    cabecalho.raiz_rid = sizeof(CabecalhoArquivo); // A raiz será criada logo após o cabeçalho
    cabecalho.topo_lista_livre = -1;               // Nenhuma página livre ainda
    cabecalho.tamanho_chave = tamChave;
    cabecalho.tamanho_dado = tamDado;

    // Grava o cabeçalho no início do arquivo (posição 0)
    fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);

    // 2. Cria a Raiz Inicial (que nasce como uma folha vazia)
    pagina raiz;
    raiz.ehfolha = 1;       // Toda árvore B+ começa com uma raiz que é folha
    raiz.numChaves = 0;
    raiz.proximaFolha = -1;   // Não tem próxima folha ainda

    // Grava a raiz na posição indicada por cabecalho.raiz_rid

    fseek(arquivo, cabecalho.raiz_rid, SEEK_SET);
    fwrite(&raiz, TAMANHO_PAGINA, 1, arquivo);

    fflush(arquivo);
    return arquivo;
}

int escreverPagina(FILE *arquivo, long rid, const pagina *no) {
    if (arquivo == NULL || no == NULL || rid < 0) {
        fprintf(stderr, "[Erro - Disco] Parâmetros inválidos para escrita.\n");
        return 0;
    }

    //Move o cabeçote de leitura do arquivo para a posição exata do RID
    if (fseek(arquivo, rid, SEEK_SET) != 0) {
        perror("[Erro - Disco] Falha ao posicionar para escrita (fseek)");
        return 0;
    }


    // Grava o nó inteiro. Para garantir que o arquivo fique perfeitamente 
    // alinhado em blocos de 4KB, forçamos a gravação de TAMANHO_PAGINA (4096 bytes)
    size_t itens_gravados = fwrite(no, TAMANHO_PAGINA, 1, arquivo);
    
    if (itens_gravados != 1) {
        fprintf(stderr, "[Erro - Disco] Falha ao gravar o nó no RID %ld.\n", rid);
        return 0;
    }

    // 3. Garante que os dados saiam do cache do sistema operacional e vão direto para o HD/SSD
    fflush(arquivo);
    return 1;
}

int lerPagina(FILE *arquivo, long rid, pagina *buffer_no) {
    if (arquivo == NULL || buffer_no == NULL || rid < 0) {
        fprintf(stderr, "[Erro - Disco] Parâmetros inválidos para leitura.\n");
        return 0;
    }

    // 1. Move o cabeçote do arquivo para a posição do RID
    if (fseek(arquivo, rid, SEEK_SET) != 0) {
        perror("[Erro - Disco] Falha ao posicionar para leitura (fseek)");
        return 0;
    }

    // 2. Puxa do disco exatamente 4096 bytes e joga para dentro da struct na RAM
    size_t itens_lidos = fread(buffer_no, TAMANHO_PAGINA, 1, arquivo);
    
    if (itens_lidos != 1) {
        if (!feof(arquivo)) {
            fprintf(stderr, "[Erro - Disco] Erro físico de leitura no RID %ld.\n", rid);
        }
        return 0;
    }

    return 1;
}