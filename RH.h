
#include <stdio.h>
#include <stdlib.h>


typedef struct funcionario {
    chaveComposta chave;
    afiliacao filiacao ;
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

typedef struct afiliacao{
    char mae [100];
    char pai [100];

}afiliacao;

typedef struct dadosContratuais{
    data dataContrato;
    int status;
    data dataDesligamento;

}dadosContratuais;

typedef struct chaveComposta{
    char nome[100];
    data dataNascimento;
}chaveComposta;
