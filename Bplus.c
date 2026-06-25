/*
 * ============================================================
 * ARQUIVO: Bplus.c
 * ============================================================
 * 
 * IMPLEMENTAÇÃO DO NÚCLEO DA ÁRVORE B+ EM DISCO
 * 
 * ------------------------------------------------------------
 * PASSO 1 – VARIÁVEIS ESTÁTICAS GLOBAIS (ESTADO DA ÁRVORE)
 * ------------------------------------------------------------
 * 1.1. FILE* para o arquivo de índice.
 * 1.2. Cabeçalho carregado em memória (raiz, primeira folha, free_list).
 * 1.3. Callbacks registrados (cmp, size, write, read).
 * 
 * ------------------------------------------------------------
 * PASSO 2 – IMPLEMENTAÇÃO DA INICIALIZAÇÃO E FINALIZAÇÃO
 * ------------------------------------------------------------
 * 2.1. bplus_init() – abre/cria arquivo, lê cabeçalho, valida assinatura.
 * 2.2. bplus_close() – escreve cabeçalho, fecha arquivo.
 * 
 * ------------------------------------------------------------
 * PASSO 3 – IMPLEMENTAÇÃO DA BUSCA POR IGUALDADE
 * ------------------------------------------------------------
 * 3.1. Percorrer da raiz até a folha usando comparação.
 * 3.2. Na folha, varrer chaves e retornar RID do dado.
 * 3.3. Tratar chave não encontrada.
 * 
 * ------------------------------------------------------------
 * PASSO 4 – IMPLEMENTAÇÃO DA BUSCA POR INTERVALO (RANGE)
 * ------------------------------------------------------------
 * 4.1. Encontrar a primeira folha com chave >= limite inferior.
 * 4.2. Percorrer folhas encadeadas até chave > limite superior.
 * 4.3. Coletar todos os RIDs dentro do intervalo.
 * 
 * ------------------------------------------------------------
 * PASSO 5 – IMPLEMENTAÇÃO DA INSERÇÃO (COM SPLIT)
 * ------------------------------------------------------------
 * 5.1. Função recursiva ou iterativa para descer até a folha.
 * 5.2. Inserir chave na folha (ordem mantida via callback).
 * 5.3. Se folha transbordar: dividir (split) e promover chave mediana.
 * 5.4. Subir a chave promovida para o nó pai (recursivamente).
 * 5.5. Se a raiz transbordar: criar nova raiz e aumentar nível.
 * 5.6. Atualizar encadeamento entre folhas (próximo RID).
 * 
 * ------------------------------------------------------------
 * PASSO 6 – IMPLEMENTAÇÃO DA REMOÇÃO (REDISTRIBUIÇÃO / MERGE)
 * ------------------------------------------------------------
 * 6.1. Localizar a folha que contém a chave.
 * 6.2. Remover a chave da folha.
 * 6.3. Se folha ficar abaixo do preenchimento mínimo:
 *      6.3.1. Tentar redistribuir (pegar chave emprestada) do irmão.
 *      6.3.2. Caso contrário, concatenar (merge) com o irmão.
 * 6.4. Subir a correção (ajuste de chave separadora) para os pais.
 * 6.5. Se a raiz ficar vazia, atualizar raiz para o único filho.
 * 
 * ------------------------------------------------------------
 * PASSO 7 – IMPLEMENTAÇÃO DA IMPRESSÃO HIERÁRQUICA
 * ------------------------------------------------------------
 * 7.1. Função recursiva que percorre níveis.
 * 7.2. Imprimir indentação conforme profundidade.
 * 7.3. Mostrar chaves (usando callback de impressão ou formatação).
 * 7.4. Destacar limites entre nós e níveis.
 * 
 * ------------------------------------------------------------
 * PASSO 8 – FUNÇÕES INTERNAS DE MANIPULAÇÃO DE NÓS
 * ------------------------------------------------------------
 * 8.1. criar_no_interno() e criar_no_folha().
 * 8.2. destruir_no() (apenas memória, não disco).
 * 8.3. carregar_no_do_disco(rid) e salvar_no_em_disco(no).
 * 8.4. encontrar_posicao_para_inserir() e encontrar_posicao_para_buscar().
 */

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