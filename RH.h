/*
 * ============================================================
 * ARQUIVO: rh_system.c
 * ============================================================
 * 
 * IMPLEMENTAÇÃO DAS FUNÇÕES CALLBACK E DA LÓGICA DE NEGÓCIO DO RH
 * 
 * ------------------------------------------------------------
 * PASSO 1 – FUNÇÕES CALLBACK PARA A ÁRVORE B+
 * ------------------------------------------------------------
 * 1.1. comparar_chave_composta(const void* a, const void* b)
 *      - Retornar -1, 0 ou 1 (prioridade: nome, depois data_nasc).
 * 1.2. tamanho_registro(const void* chave) – retorna sizeof(ChaveComposta).
 * 1.3. escrever_registro(const void* chave, void* buffer)
 *      - Copiar os campos da struct para o buffer (sem padding).
 * 1.4. ler_registro(void* destino, const void* buffer)
 *      - Copiar do buffer para a struct.
 * 
 * ------------------------------------------------------------
 * PASSO 2 – FUNÇÃO DE INSERIR FUNCIONÁRIO (FLUXO COMPLETO)
 * ------------------------------------------------------------
 * 2.1. Solicitar Nome e Data Nascimento.
 * 2.2. Buscar na árvore se a chave composta já existe.
 * 2.3. Se existir: exibir dados atuais e perguntar se quer atualizar.
 * 2.4. Se não existir: solicitar demais campos, inicializar pagamentos vazio.
 * 2.5. Montar a struct Funcionario e inserir na B+ (chave + RID do dado).
 * 2.6. O RID do dado pode ser o próprio offset do arquivo de dados (ou nó folha).
 * 
 * ------------------------------------------------------------
 * PASSO 3 – FUNÇÃO DE BUSCAR FUNCIONÁRIO (COM DESEMPATE)
 * ------------------------------------------------------------
 * 3.1. Solicitar Nome.
 * 3.2. Buscar todos os registros com esse nome (range com mesmo prefixo).
 * 3.3. Se apenas um: exibir ficha completa.
 * 3.4. Se vários (homônimos): listar nome + data_nasc, pedir para escolher.
 * 3.5. Exibir ficha completa do selecionado (incluindo histórico).
 * 
 * ------------------------------------------------------------
 * PASSO 4 – FUNÇÃO DE EXCLUIR FUNCIONÁRIO (COM CONFIRMAÇÃO)
 * ------------------------------------------------------------
 * 4.1. Seguir fluxo de busca (item 3) para encontrar o registro exato.
 * 4.2. Exibir dados cadastrais (sem histórico) e pedir confirmação (S/N).
 * 4.3. Se confirmado: chamar bplus_delete() com a chave composta.
 * 4.4. Remover também o dado do arquivo de dados (ou marcar como inativo).
 * 
 * ------------------------------------------------------------
 * PASSO 5 – FUNÇÃO DE LISTAR POR INTERVALO (RANGE)
 * ------------------------------------------------------------
 * 5.1. Solicitar Nome A e Nome B (strings delimitadoras).
 * 5.2. Montar chaves compostas com esses nomes (data mínima/máxima).
 * 5.3. Chamar bplus_range_search() para obter lista de RIDs.
 * 5.4. Para cada RID, carregar o funcionário e exibir (nome e data).
 * 
 * ------------------------------------------------------------
 * PASSO 6 – FUNÇÕES AUXILIARES DE E/S E FORMATAÇÃO
 * ------------------------------------------------------------
 * 6.1. ler_string_segura(char* destino, int tamanho).
 * 6.2. ler_data(Data* d) – validar dia/mês/ano.
 * 6.3. imprimir_ficha_completa(Funcionario* f).
 * 6.4. imprimir_historico_pagamentos(Pagamento* hist[12]).
 * 6.5. serializar_funcionario() e desserializar_funcionario() para arquivo de dados.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct funcionario {
    chaveComposta chave;
    char mae[100];
    char pai[100];
    dadosContato contato;
    float historicoPagamentos[12];
    dadosContratuais contrato;
} funcionario;


typedef struct data {
    int dia;
    int mes;
    int ano;
} data;

typedef struct dadosContato {
    char endereco[200];
    char telefone[20];
} dadosContato;


typedef struct dadosContratuais{
    data dataContrato;
    bool status;
    data dataDesligamento;

}dadosContratuais;

typedef struct chaveComposta{
    char nome[100];
    data dataNascimento;
}chaveComposta;
