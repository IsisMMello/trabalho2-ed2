#ifndef RH_H
#define RH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============ ESTRUTURAS DE DADOS ============

typedef struct data {
    int dia;
    int mes;
    int ano;
} data;

typedef struct dadosContato {
    char endereco[200];
    char telefone[20];
} dadosContato;

typedef struct afiliacao {
    char mae[100];
    char pai[100];
} afiliacao;

typedef struct dadosContratuais {
    data dataContrato;
    int status;          // 1 = Ativo, 0 = Inativo
    data dataDesligamento;
} dadosContratuais;

typedef struct chaveComposta {
    char nome[100];
    data dataNascimento;
} chaveComposta;

typedef struct funcionario {
    chaveComposta chave;
    afiliacao filiacao;
    dadosContato contato;
    double historicoPagamentos[12];
    dadosContratuais contrato;
} funcionario;


// ============ FUNÇÕES DE CRIAÇÃO ============

funcionario* criar_funcionario(char* nome, int dia, int mes, int ano, char* mae, char* pai, char* endereco, char* telefone);

// ============ FUNÇÕES DE CALLBACK ============

int compararPorChaveComposta(const void* a, const void* b);

// ============ FUNÇÕES DE PERSISTÊNCIA ============

int salvar_funcionario(const funcionario* f, int* posicao);
int carregar_funcionario(funcionario* f, int posicao);
void atualizar_funcionario(funcionario *f, int posicao);

// ============ FUNÇÕES DE IMPRESSÃO ============

void imprimir_funcionario(const funcionario* f,int opcao);
void imprimir_funcionario_resumido(const funcionario* f);
void imprimirArvore();

// ============ FUNÇÕES DO SISTEMA RH ============

void rh_inserir_funcionario(void);
void rh_buscar_funcionario(void);
void rh_excluir_funcionario(void);
void rh_listar_intervalo(void);


#endif 