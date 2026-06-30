#include "RH.h"
#include "Bplus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//inicializa o funcionario
funcionario* criar_funcionario(char* nome, int dia, int mes, int ano, char* mae, char* pai ,char*endereco, char* telefone) {

    funcionario* novo = (funcionario*)malloc(sizeof(funcionario));
    if (novo == NULL) {
        return NULL;
    }
    
    // nome e data
    strcpy(novo->chave.nome, nome);
    novo->chave.dataNascimento.dia = dia;
    novo->chave.dataNascimento.mes = mes;
    novo->chave.dataNascimento.ano = ano;
    
    // mae pai
    strcpy(novo->filiacao.mae, mae);
    strcpy(novo->filiacao.pai, pai);
    
    // contato
    strcpy(novo->contato.endereco, endereco);
    strcpy(novo->contato.telefone, telefone);
    
    // inicializa contrato
    novo->contrato.dataContrato.dia = 0;
    novo->contrato.dataContrato.mes = 0;
    novo->contrato.dataContrato.ano = 0;
    novo->contrato.status = 0;
    novo->contrato.dataDesligamento.dia = 0;
    novo->contrato.dataDesligamento.mes = 0;
    novo->contrato.dataDesligamento.ano = 0;

    //zera os pagamentos;
    for (int i = 0; i < 12; i++) {
        novo->historicoPagamentos[i] = 0.0;
    }

    
    return novo;
}

//calbacks
int comparar_chave(const void*a, const void*b){
    const chaveComposta *chave1 = (const chaveComposta *)a;
    const chaveComposta *chave2 = (const chaveComposta *)b;

    int compararNome = strcmp(chave1->nome,chave2->nome);

    if (compararNome != 0) return compararNome; 

    if (chave1->dataNascimento.ano != chave2->dataNascimento.ano) {
        return (chave1->dataNascimento.ano < chave2->dataNascimento.ano) ? -1 : 1;
    }
    if (chave1->dataNascimento.mes != chave2->dataNascimento.mes) {
        return (chave1->dataNascimento.mes < chave2->dataNascimento.mes) ? -1 : 1;
    }
    if (chave1->dataNascimento.dia != chave2->dataNascimento.dia) {
        return (chave1->dataNascimento.dia < chave2->dataNascimento.dia) ? -1 : 1;
    }
    return 0; //data igual

}

// //callback 
// size_t tamanho_chave(const void* chave) {
//     return sizeof(chaveComposta);
// }

// void escrever_chave(const void* chave, void* buffer) {
//     memcpy(buffer, chave, sizeof(chaveComposta));
// }

// void ler_chave(void* destino, const void* buffer) {
//     memcpy(destino, buffer, sizeof(chaveComposta));
// }

//precisa??
// //serializacao funcionario

// void serializar_funcionario(const funcionario* f, void* buffer) {
//     memcpy(buffer, f, sizeof(funcionario));
// }

// //desserializacao funcionario
// void desserializar_funcionario(funcionario* f, const void* buffer) {
//     memcpy(f, buffer, sizeof(funcionario));
// }






int salvar_funcionario(const funcionario *f, int *posicao)
{
    FILE *arquivo;

    if (*posicao == -1) {

        arquivo = fopen("funcionarios.dat", "a+b");

        if (arquivo == NULL)
            return 0;

        fseek(arquivo, 0, SEEK_END);
        *posicao = ftell(arquivo);
    }
    else {
        arquivo = fopen("funcionarios.dat", "rb+");

        if (arquivo == NULL)
            return 0;

        fseek(arquivo, *posicao, SEEK_SET);
    }

    int ok = fwrite(f, sizeof(funcionario), 1, arquivo);

    fclose(arquivo);

    return ok == 1;
}

//manda o arquivo para a memoria

int carregar_funcionario(funcionario* f, int posicao) {
    FILE* arquivo = fopen("funcionarios.dat", "rb");
    if (arquivo == NULL) {
        return 0;
    }
    
    fseek(arquivo, posicao, SEEK_SET);
    size_t lido = fread(f, sizeof(funcionario), 1, arquivo);
    fclose(arquivo);
    
    return (lido == 1);
}

//imprimir o funcionario
void imprimir_funcionario(const funcionario* f) {
    printf("\n========================================\n");
    printf("FICHA CADASTRAL\n");
    printf("========================================\n");
    printf("Nome: %s\n", f->chave.nome);
    printf("Data Nascimento: %02d/%02d/%04d\n",
           f->chave.dataNascimento.dia,
           f->chave.dataNascimento.mes,
           f->chave.dataNascimento.ano);
    printf("Mae: %s\n", f->filiacao.mae);
    printf("Pai: %s\n", f->filiacao.pai);
    printf("Endereco: %s\n", f->contato.endereco);
    printf("Telefone: %s\n", f->contato.telefone);
    printf("Data Contratacao: %02d/%02d/%04d\n",
           f->contrato.dataContrato.dia,
           f->contrato.dataContrato.mes,
           f->contrato.dataContrato.ano);
    printf("Status: %s\n", f->contrato.status ? "ATIVO" : "INATIVO");
    if (!f->contrato.status) {
        printf("Data Desligamento: %02d/%02d/%04d\n",
               f->contrato.dataDesligamento.dia,
               f->contrato.dataDesligamento.mes,
               f->contrato.dataDesligamento.ano);
    }
    printf("========================================\n");
}

void imprimir_funcionario_resumido(const funcionario* f) {
    printf("\n----------------------------------------\n");
    printf("Nome: %s\n", f->chave.nome);
    printf("Data Nascimento: %02d/%02d/%04d\n",
           f->chave.dataNascimento.dia,
           f->chave.dataNascimento.mes,
           f->chave.dataNascimento.ano);
    printf("Status: %s\n", f->contrato.status ? "ATIVO" : "INATIVO");
    printf("----------------------------------------\n");
}



void rh_inserir_funcionario() {

    char nome[100], mae[100], pai[100], endereco[200], telefone[20];
    data dataNasc;
    data dataCont;

    int posicao;
    chaveComposta chave;

    printf("\n========== INSERIR FUNCIONARIO ==========\n");

    // Nome
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    // Data nascimento
    printf("Data de Nascimento (dd/mm/aaaa): ");
    scanf("%d/%d/%d",
          &dataNasc.dia,
          &dataNasc.mes,
          &dataNasc.ano);
    getchar();

    // Monta a chave
    strcpy(chave.nome, nome);
    chave.dataNascimento = dataNasc;

    // Verifica se já existe
    if (buscarChaveNaArvore(&chave, &posicao, comparar_chave) == 1) {

        funcionario f;

        if (carregar_funcionario(&f, posicao)) {

            printf("\nFuncionario ja cadastrado!\n");
            imprimir_funcionario_resumido(&f);

            printf("\nDeseja atualizar os dados? (s/n): ");

            char resp;
            scanf(" %c", &resp);
            getchar();

            if (resp == 's' || resp == 'S') {

                //ATUALIZA DADOS DO FUNCIONARIO
                char buffer[200];

                printf("\n=== ATUALIZACAO ===\n");
                printf("Pressione ENTER para manter o valor atual.\n\n");

                printf("Nome da Mae (%s): ", f.filiacao.mae);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) > 0)
                    strcpy(f.filiacao.mae, buffer);

                printf("Nome do Pai (%s): ", f.filiacao.pai);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) > 0)
                    strcpy(f.filiacao.pai, buffer);

                printf("Endereco (%s): ", f.contato.endereco);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) > 0)
                    strcpy(f.contato.endereco, buffer);

                printf("Telefone (%s): ", f.contato.telefone);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) > 0)
                    strcpy(f.contato.telefone, buffer);

                printf("Status (1-Ativo / 0-Inativo) [%d]: ", f.contrato.status);
                scanf("%d", &f.contrato.status);
                getchar();

                if (f.contrato.status == 0) {
                    printf("Data de desligamento (dd/mm/aaaa): ");
                    scanf("%d/%d/%d",
                        &f.contrato.dataDesligamento.dia,
                        &f.contrato.dataDesligamento.mes,
                        &f.contrato.dataDesligamento.ano);
                    getchar();
                }

                if (salvar_funcionario(&f, &posicao))
                    printf("\nFuncionario atualizado com sucesso!\n");
                else
                    printf("\nErro ao atualizar funcionario.\n");
            }
        }

        return;
    }

    // Dados da mãe
    printf("Nome da Mae: ");
    fgets(mae, sizeof(mae), stdin);
    mae[strcspn(mae, "\n")] = '\0';

    // Dados do pai
    printf("Nome do Pai: ");
    fgets(pai, sizeof(pai), stdin);
    pai[strcspn(pai, "\n")] = '\0';

    // Endereço
    printf("Endereco: ");
    fgets(endereco, sizeof(endereco), stdin);
    endereco[strcspn(endereco, "\n")] = '\0';

    // Telefone
    printf("Telefone: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    // Data contratação
    printf("Data de Contratacao (dd/mm/aaaa): ");
    scanf("%d/%d/%d",
          &dataCont.dia,
          &dataCont.mes,
          &dataCont.ano);
    getchar();

    // Cria o funcionário
    funcionario *novo = criar_funcionario(
            nome,
            dataNasc.dia,
            dataNasc.mes,
            dataNasc.ano,
            mae,
            pai,
            endereco,
            telefone);

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    // Dados contratuais
    novo->contrato.dataContrato = dataCont;
    novo->contrato.status = 1;

    novo->contrato.dataDesligamento.dia = 0;
    novo->contrato.dataDesligamento.mes = 0;
    novo->contrato.dataDesligamento.ano = 0;

    // Salva no arquivo de dados

    posicao = -1;
    if (!salvar_funcionario(novo, &posicao)) {

        printf("Erro ao salvar funcionario.\n");
        free(novo);
        return;
    }

    // Insere a chave na árvore B+
    inserirChaveNaArvore(&novo->chave,posicao,sizeof(chaveComposta),comparar_chave);

    printf("\nFuncionario cadastrado com sucesso!\n");

    imprimir_funcionario_resumido(novo);

    free(novo);
}

void rh_excluir_funcionario() {
    char nome[100];
    int qtd;
    int*posicoes;
    chaveComposta chaveMin,chaveMax;
    funcionario f;

    printf("\n=== EXCLUIR FUNCIONARIO ===\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';


    posicoes = buscarPorNome(nome, posicoes, 100);

    if (qtd == 0) {
        printf("Nenhum funcionario encontrado com esse nome.\n");
        return;
    }

    else if(qtd ==1){
        if (carregar_funcionario(&f, posicoes[0])) {
            imprimir_funcionario(&f);
            printf("Confirma a exclusao? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();
            if (resp != 's' && resp != 'S') {
                printf("Exclusao cancelada.\n");
                return;
            }
            f.contrato.status = 0;  // 0 = INATIVO
    
           //desligamento
            printf("Data de desligamento (dd/mm/aaaa): ");
            scanf("%d/%d/%d", 
                &f.contrato.dataDesligamento.dia,
                &f.contrato.dataDesligamento.mes,
                &f.contrato.dataDesligamento.ano);
            getchar();
            
            //salva
            if (salvar_funcionario(&f, &posicoes[0])) {
                //REMOVE DA ARVORE
                deletarChaveNaArvore(&f.chave, comparar_chave);
                printf("\n Funcionario removido com sucesso!\n");
            } else {
                printf("\n Erro ao remover funcionario.\n");
            }
            
            //SE FOR DINAMICO
            free(posicoes);
        }
    
    } else {

        for(int i = 0; i++; i<qtd){
            if (carregar_funcionario(&f, posicoes[i])) {
                printf("[%d] ", i+1);
                imprimir_funcionario_resumido(&f);
            } else {
                printf("[%d] ERRO: Nao foi possivel carregar o funcionario da posicao %d\n", 
                        i+1, posicoes[i]);
            }
        }
        data dataFuncionario;
        chaveComposta chaveFuncionario;

        printf("Digite a data de nascimento do funcionario que deseja exluir:");
        printf("Data de Nascimento (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &dataFuncionario.dia, &dataFuncionario.mes, &dataFuncionario.ano);
        getchar();

        strcpy(chaveFuncionario.nome, nome);
        chaveFuncionario.dataNascimento = dataFuncionario;

        if (carregar_funcionario(&f, posicoes[0])) {
            imprimir_funcionario(&f);
            printf("Confirma a exclusao? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();
            if (resp != 's' && resp != 'S') {
                printf("Exclusao cancelada.\n");
                return;
            }
            f.contrato.status = 0;  // 0 = INATIVO
    
           //desligamento
            printf("Data de desligamento (dd/mm/aaaa): ");
            scanf("%d/%d/%d", 
                &f.contrato.dataDesligamento.dia,
                &f.contrato.dataDesligamento.mes,
                &f.contrato.dataDesligamento.ano);
            getchar();
            
            //salva
            if (salvar_funcionario(&f, &posicoes[0])) {
                //REMOVE DA ARVORE
                deletarChaveNaArvore(&f.chave, comparar_chave);
                printf("\n Funcionario removido com sucesso!\n");
            } else {
                printf("\n Erro ao remover funcionario.\n");
            }
            
            //SE FOR DINAMICO
            free(posicoes);
        }
    
    }
}


    


void rh_buscar_funcionario() {
    char nome[100];
    funcionario f;
    int qtd;
    int*posicoes;
    chaveComposta chaveMin,chaveMax;
    
    printf("\n=== BUSCAR FUNCIONARIO ===\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';
    
    // Monta chave mínima (nome + data mínima)
    strcpy(chaveMin.nome, nome);
    chaveMin.dataNascimento.dia = 1;
    chaveMin.dataNascimento.mes = 1;
    chaveMin.dataNascimento.ano = 1;
    
    // Monta chave máxima (nome + data máxima)
    strcpy(chaveMax.nome, nome);
    chaveMax.dataNascimento.dia = 31;
    chaveMax.dataNascimento.mes = 12;
    chaveMax.dataNascimento.ano = 9999;

    // Busca todos os registros com esse nome (homônimos)
    posicoes = buscarIntervalo(chaveMax,chaveMin,&qtd);


    
    if (qtd == 0) {
        printf("Nenhum funcionario encontrado com esse nome.\n");
        return;
    }
    
    else if (qtd ==1){
        if (carregar_funcionario(&f, posicoes[0])) {
            imprimir_funcionario(&f);
        } else {
            printf("ERRO: Nao foi possivel carregar o funcionario ");
        }
        
    }else{

        for(int i = 0; i++; i<qtd){
            if (carregar_funcionario(&f, posicoes[i])) {
                printf("[%d] ", i+1);
                imprimir_funcionario_resumido(&f);
            } else {
                printf("[%d] ERRO: Nao foi possivel carregar o funcionario da posicao %d\n", 
                       i+1, posicoes[i]);
            }

        }
        data dataFuncionario;
        chaveComposta chaveFuncionario;

        printf("Digite a data de nascimento do funcionario que deseja buscar:");
        printf("Data de Nascimento (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &dataFuncionario.dia, &dataFuncionario.mes, &dataFuncionario.ano);
        getchar();

        strcpy(chaveFuncionario.nome, nome);
        chaveFuncionario.dataNascimento = dataFuncionario;

        int posicaoFuncionario;

        if (buscarChaveNaArvore(&chaveFuncionario, &posicaoFuncionario, comparar_chave) == 1) {
            funcionario funcionarioImprimir;
            if (carregar_funcionario(&funcionarioImprimir, posicaoFuncionario)) {
                printf("\n Dados do funcionario selecionado\n");
                imprimir_funcionario(&funcionarioImprimir); 
            } else {
                printf("Erro ao carregar os dados do funcionario.\n");
            }
        } else {
            printf("Funcionario com a data informada nao encontrado.\n");
        }
        
        
    }
    
    
}


void rh_listar_intervalo() {


    char nomeA[100], nomeB[100];
    chaveComposta chaveMin,chaveMax;
    int*posicoes;
    int qtd;
    printf("\n=== LISTAGEM POR INTERVALO ===\n");
    printf("Nome (A): ");
    fgets(nomeA, sizeof(nomeA), stdin);
    nomeA[strcspn(nomeA, "\n")] = '\0';
    printf("Nome (B): ");
    fgets(nomeB, sizeof(nomeB), stdin);
    nomeB[strcspn(nomeB, "\n")] = '\0';
    
    //faz as chaves para comparar
    strcpy(chaveMin.nome, nomeA);
    chaveMin.dataNascimento.dia = 0;
    chaveMin.dataNascimento.mes = 0;
    chaveMin.dataNascimento.ano = 0;
    
    strcpy(chaveMax.nome, nomeB);
    chaveMax.dataNascimento.dia = 31;
    chaveMax.dataNascimento.mes = 12;
    chaveMax.dataNascimento.ano = 9999;

    printf("\nFuncionarios no intervalo (%s, %s):\n", nomeA, nomeB);
    printf("----------------------------------------\n");
   
    posicoes = buscarIntervalo(&chaveMin, &chaveMax, &qtd);

    if(qtd == 0)printf("nenhum funcionario");
    else{
        printf("\nTotal: %d funcionario(s) encontrados.\n", qtd);
        for (int i = 0; i < qtd; i++) {
            funcionario f;
            
            // Carrega o funcionário da posição i
            if (carregar_funcionario(&f, posicoes[i])) {
                printf("[%d] ", i+1);
                //FUNCIONARIO RESUMIDO OU NORMAL????
                imprimir_funcionario(&f);
            } else {
                printf("[%d] ERRO: Nao foi possivel carregar o funcionario da posicao %d\n", 
                       i+1, posicoes[i]);
            }
        }
      

    }
    
    printf("----------------------------------------\n");
}



