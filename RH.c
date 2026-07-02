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
    novo->contrato.status = 1;
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
int compararPorChaveComposta(const void*a, const void*b){
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

int salvar_funcionario(const funcionario *f, int *posicao)
{
    FILE *arquivo;
    // O tamanho total da soma dos campos sem padding é 738 bytes
    const size_t TAMANHO_REGISTRO_DISCO = 738; 
    
    // Cria um buffer (um array de bytes) na RAM para montar o registro
    unsigned char buffer[738];
    size_t offset = 0; // Controla onde estamos colando no buffer

    // --- COLA CAMPO POR CAMPO NO BUFFER USANDO MEMCPY ---
    memcpy(buffer + offset, f->chave.nome, 100);
    offset += 100;

    memcpy(buffer + offset, &f->chave.dataNascimento, sizeof(data));
    offset += sizeof(data);
    
    memcpy(buffer + offset, f->filiacao.mae, 100);
    offset += 100;
    
    memcpy(buffer + offset, f->filiacao.pai, 100);
    offset += 100;
    
    memcpy(buffer + offset, f->contato.endereco, 200);
    offset += 200;
    
    memcpy(buffer + offset, f->contato.telefone, 20);
    offset += 20;
    
    memcpy(buffer + offset, f->historicoPagamentos, sizeof(double) * 12);
    offset += sizeof(double) * 12;
    
    memcpy(buffer + offset, &f->contrato, sizeof(dadosContratuais));
    // offset += sizeof(dadosContratuais); // Último campo, não precisa somar mais

    // --- PARTE DA GRAVAÇÃO NO ARQUIVO ---
    if (*posicao == -1) {
        arquivo = fopen("funcionarios.dat", "a+b");
        if (arquivo == NULL) return 0;

        fseek(arquivo, 0, SEEK_END);
        long tamanho = ftell(arquivo);
        *posicao = tamanho / TAMANHO_REGISTRO_DISCO;
    }
    else {
        arquivo = fopen("funcionarios.dat", "rb+");
        if (arquivo == NULL) return 0;

        fseek(arquivo, (*posicao) * TAMANHO_REGISTRO_DISCO, SEEK_SET);
    }

    // Grava o buffer compacto de uma vez só no arquivo
    int ok = fwrite(buffer, TAMANHO_REGISTRO_DISCO, 1, arquivo);
    fclose(arquivo);

    return ok == 1;
}
//manda o arquivo para a memoria

int carregar_funcionario(funcionario* f, int posicao) {
    FILE* arquivo = fopen("funcionarios.dat", "rb");
    if (arquivo == NULL) 
        return 0;
    
    const size_t TAMANHO_REGISTRO_DISCO = 738;
    fseek(arquivo, posicao * TAMANHO_REGISTRO_DISCO, SEEK_SET);
    
    // --- LÊ CAMPO A CAMPO ---
    size_t itensLidos = 0;
    
    itensLidos += fread(f->chave.nome, sizeof(char), 100, arquivo);
    itensLidos += fread(&f->chave.dataNascimento, sizeof(data), 1, arquivo);
    
    itensLidos += fread(f->filiacao.mae, sizeof(char), 100, arquivo);
    itensLidos += fread(f->filiacao.pai, sizeof(char), 100, arquivo);
    
    itensLidos += fread(f->contato.endereco, sizeof(char), 200, arquivo);
    itensLidos += fread(f->contato.telefone, sizeof(char), 20, arquivo);
    
    itensLidos += fread(f->historicoPagamentos, sizeof(double), 12, arquivo);
    itensLidos += fread(&f->contrato, sizeof(dadosContratuais), 1, arquivo);
    
    fclose(arquivo);
    
    // Se leu todos os blocos com sucesso (8 blocos no total)
    return (itensLidos == 247); // 100+1+100+100+200+20+12+1 = total de leituras individuais bem-sucedidas
}

//imprimir o funcionario
void imprimir_funcionario(const funcionario* f,int opcao) {
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
    if(opcao == 1){
        printf("Pagamentos: ");
        printf("[ |");
        for(int i = 0; i<11;i++){
            printf("%lf | ", f->historicoPagamentos[i]);

        }
        printf("]");
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

void atualizar_funcionario(funcionario *f, int posicao){

    char buffer[200];

    printf("\n=== ATUALIZACAO ===\n");
    printf("Pressione ENTER para manter o valor atual.\n\n");

    printf("Nome da Mae (%s): ", f->filiacao.mae);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strcpy(f->filiacao.mae, buffer);

    printf("Nome do Pai (%s): ", f->filiacao.pai);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strcpy(f->filiacao.pai, buffer);

    printf("Endereco (%s): ", f->contato.endereco);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strcpy(f->contato.endereco, buffer);

    printf("Telefone (%s): ", f->contato.telefone);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
        strcpy(f->contato.telefone, buffer);

    printf("Status (1-Ativo / 0-Inativo) [%d]: ", f->contrato.status);
    scanf("%d", &f->contrato.status);
    getchar();

    if (f->contrato.status == 0) {
        printf("Data de desligamento (dd/mm/aaaa): ");
        scanf("%d/%d/%d",
            &f->contrato.dataDesligamento.dia,
            &f->contrato.dataDesligamento.mes,
            &f->contrato.dataDesligamento.ano);
        getchar();
    }
    char opcao;
    int opcaoMes;
    printf("Deseja inserir/atualizar pagamento? s/n");
    scanf("%c", &opcao);
    if(opcao == 's'|| opcao == 'S'){
        printf("Digite o mes do pagamento: (1 a 12) ");
        scanf("%d",&opcaoMes);
        printf("Digite o valor do pagamento.");
        scanf("%lf",&f->historicoPagamentos[opcaoMes-1]);

    }


    if (salvar_funcionario(f, &posicao))
        printf("\nFuncionario atualizado com sucesso!\n");
    else
        printf("\nErro ao atualizar funcionario.\n");


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

    // Verifica se já existe - CORRIGIDO: compararPorChaveComposta
    if (buscarChaveNaArvore(&chave, &posicao, compararPorChaveComposta) == 1) {

        funcionario f;

        if (carregar_funcionario(&f, posicao)) {

            printf("\nFuncionario ja cadastrado!\n");
            imprimir_funcionario(&f,1);

            printf("\nDeseja atualizar os dados? (s/n): ");

            char resp;
            scanf(" %c", &resp);
            getchar();

            if (resp == 's' || resp == 'S') {

                //ATUALIZA DADOS DO FUNCIONARIO
                atualizar_funcionario(&f,posicao);
                return;
            }
        }

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
    funcionario *novo = criar_funcionario(nome, dataNasc.dia,  dataNasc.mes, dataNasc.ano, mae, pai, endereco, telefone);

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

    // CORRIGIDO: Passa a variável local '&chave' em vez de '&novo->chave'
    inserirChaveNaArvore(&chave, posicao, sizeof(chaveComposta), compararPorChaveComposta);

    printf("\nFuncionario cadastrado com sucesso!\n");
    imprimir_funcionario(novo,1);
    free(novo);
}

void rh_excluir_funcionario() {
    char nome[100];
    int qtd;
    int* posicoes;
    chaveComposta chaveMin, chaveMax;
    funcionario f;

    printf("\n=== EXCLUIR FUNCIONARIO ===\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    // Monta chave mínima e máxima para buscar todos com este nome
    strcpy(chaveMin.nome, nome);
    chaveMin.dataNascimento.dia = 1;
    chaveMin.dataNascimento.mes = 1;
    chaveMin.dataNascimento.ano = 1000;
    
    strcpy(chaveMax.nome, nome);
    chaveMax.dataNascimento.dia = 31;
    chaveMax.dataNascimento.mes = 12;
    chaveMax.dataNascimento.ano = 9999;

    // CORRIGIDO: compararPorChaveComposta
    posicoes = buscarChavesIntervalo(&chaveMin, &chaveMax, &qtd, compararPorChaveComposta);

    if (qtd == 0) {
        printf("Nenhum funcionario encontrado com esse nome.\n");
        free(posicoes);
        return;
    }

    else if(qtd == 1) {
        if (carregar_funcionario(&f, posicoes[0])) {
            imprimir_funcionario(&f,0);
            printf("Confirma a exclusao? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();
            if (resp != 's' && resp != 'S') {
                printf("Exclusao cancelada.\n");
                free(posicoes);
                return;
            }
            f.contrato.status = 0;
    
            printf("Data de desligamento (dd/mm/aaaa): ");
            scanf("%d/%d/%d", 
                &f.contrato.dataDesligamento.dia,
                &f.contrato.dataDesligamento.mes,
                &f.contrato.dataDesligamento.ano);
            getchar();
            
            if (salvar_funcionario(&f, &posicoes[0])) {
                // CORRIGIDO: compararPorChaveComposta
                deletarChaveNaArvore(&f.chave, compararPorChaveComposta);
                printf("\n Funcionario removido com sucesso!\n");
            } else {
                printf("\n Erro ao remover funcionario.\n");
            }
        }
        free(posicoes);
    } else {
        for(int i = 0; i < qtd; i++) {
            if (carregar_funcionario(&f, posicoes[i])) {
                printf("[%d] ", i+1);
                imprimir_funcionario(&f,0);
            } else {
                printf("[%d] ERRO: Nao foi possivel carregar o funcionario da posicao %d\n", 
                        i+1, posicoes[i]);
            }
        }
        
        data dataFuncionario;
        chaveComposta chaveFuncionario;

        printf("Digite a data de nascimento do funcionario que deseja excluir:");
        printf("Data de Nascimento (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &dataFuncionario.dia, &dataFuncionario.mes, &dataFuncionario.ano);
        getchar();

        strcpy(chaveFuncionario.nome, nome);
        chaveFuncionario.dataNascimento = dataFuncionario;

        int posicaoFuncionario;
        
        // CORRIGIDO: compararPorChaveComposta
        if (buscarChaveNaArvore(&chaveFuncionario, &posicaoFuncionario, compararPorChaveComposta) == 1) {
            if (carregar_funcionario(&f, posicaoFuncionario)) {
                imprimir_funcionario(&f,0);
                printf("Confirma a exclusao? (s/n): ");
                char resp;
                scanf(" %c", &resp);
                getchar();
                if (resp != 's' && resp != 'S') {
                    printf("Exclusao cancelada.\n");
                    free(posicoes);
                    return;
                }
                f.contrato.status = 0;
        
                printf("Data de desligamento (dd/mm/aaaa): ");
                scanf("%d/%d/%d", 
                    &f.contrato.dataDesligamento.dia,
                    &f.contrato.dataDesligamento.mes,
                    &f.contrato.dataDesligamento.ano);
                getchar();
                
                if (salvar_funcionario(&f, &posicaoFuncionario)) {
                    // CORRIGIDO: compararPorChaveComposta
                    deletarChaveNaArvore(&f.chave, compararPorChaveComposta);
                    printf("\n Funcionario removido com sucesso!\n");
                } else {
                    printf("\n Erro ao remover funcionario.\n");
                }
            }
        } else {
            printf("Funcionario com a data informada nao encontrado.\n");
        }
        free(posicoes);
    }
}

void rh_buscar_funcionario() {
    char nome[100];
    funcionario f;
    int qtd;
    int* posicoes;
    chaveComposta chaveMin, chaveMax;
    
    printf("\n=== BUSCAR FUNCIONARIO ===\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';
    
    strcpy(chaveMin.nome, nome);
    chaveMin.dataNascimento.dia = 1;
    chaveMin.dataNascimento.mes = 1;
    chaveMin.dataNascimento.ano = 1000;
    
    strcpy(chaveMax.nome, nome);
    chaveMax.dataNascimento.dia = 31;
    chaveMax.dataNascimento.mes = 12;
    chaveMax.dataNascimento.ano = 9999;

    // CORRIGIDO: compararPorChaveComposta
    posicoes = buscarChavesIntervalo(&chaveMin, &chaveMax, &qtd, compararPorChaveComposta);
    
    if (qtd == 0) {
        printf("Nenhum funcionario encontrado com esse nome.\n");
        free(posicoes);
        return;
    }
    
    else if (qtd == 1) {
        if (carregar_funcionario(&f, posicoes[0])) {
            imprimir_funcionario(&f,1);
        } else {
            printf("ERRO: Nao foi possivel carregar o funcionario ");
        }
        free(posicoes);
    } else {
        for(int i = 0; i < qtd; i++) {
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

        // CORRIGIDO: compararPorChaveComposta
        if (buscarChaveNaArvore(&chaveFuncionario, &posicaoFuncionario, compararPorChaveComposta) == 1) {
            funcionario funcionarioImprimir;
            if (carregar_funcionario(&funcionarioImprimir, posicaoFuncionario)) {
                printf("\n Dados do funcionario selecionado\n");
                imprimir_funcionario(&funcionarioImprimir,1); 
            } else {
                printf("Erro ao carregar os dados do funcionario.\n");
            }
        } else {
            printf("Funcionario com a data informada nao encontrado.\n");
        }
        free(posicoes);
    }
}

void rh_listar_intervalo() {
    char nomeA[100], nomeB[100];
    chaveComposta chaveMin, chaveMax;
    int* posicoes;
    int qtd;
    
    printf("\n=== LISTAGEM POR INTERVALO ===\n");
    printf("Nome (A): ");
    fgets(nomeA, sizeof(nomeA), stdin);
    nomeA[strcspn(nomeA, "\n")] = '\0';
    printf("Nome (B): ");
    fgets(nomeB, sizeof(nomeB), stdin);
    nomeB[strcspn(nomeB, "\n")] = '\0';
    
    strcpy(chaveMin.nome, nomeA);
    chaveMin.dataNascimento.dia = 1;
    chaveMin.dataNascimento.mes = 1;
    chaveMin.dataNascimento.ano = 1000;
    
    strcpy(chaveMax.nome, nomeB);
    chaveMax.dataNascimento.dia = 31;
    chaveMax.dataNascimento.mes = 12;
    chaveMax.dataNascimento.ano = 9999;

    printf("\nFuncionarios no intervalo (%s, %s):\n", nomeA, nomeB);
    printf("----------------------------------------\n");
   

    posicoes = buscarChavesIntervalo(&chaveMin, &chaveMax, &qtd, compararPorChaveComposta);

    if(qtd == 0) {
        printf("Nenhum funcionario no intervalo.");
    } else {
        printf("\nTotal: %d funcionario(s) encontrados.\n", qtd);
        for (int i = 0; i < qtd; i++) {
            funcionario f;
            
            if (carregar_funcionario(&f, posicoes[i])) {
                printf("[%d] ", i+1);
                //PERGUNTAR PARA ELA
                imprimir_funcionario(&f,1);
            } else {
                printf("[%d] ERRO: Nao foi possivel carregar o funcionario da posicao %d\n", 
                       i+1, posicoes[i]);
            }
        }
    }
    
    free(posicoes);
    printf("----------------------------------------\n");
}




//IMPRIMIR ARVORE 
//IMPRIMIR ARVORE 
void imprimirArvore(){  
    printf("\n========================================\n");
    printf("  ESTRUTURA DA ÁRVORE B+\n");
    printf("========================================\n");

    FILE *arquivo = fopen(arquivoArvore, "rb");
    if (arquivo == NULL) 
        return;

    // Lê o cabeçalho para encontrar a raiz
    Cabecalho header;
    fseek(arquivo, 0, SEEK_SET);
    if (fread(&header, sizeof(Cabecalho), 1, arquivo) != 1) {
        fclose(arquivo);
        return;
    }

    // Dispara a impressão a partir da raiz no nível 0
    imprimirArvoreRecursivo(arquivo, header.raiz, 0);

    fclose(arquivo);

    printf("========================================\n");
}

void imprimirArvoreRecursivo(FILE *arquivo, int indicePagina, int nivel) {
    if (indicePagina == -1) return;

    // Aloca e carrega a página atual do disco
    Pagina *p = criaPagina();
    fseek(arquivo, sizeof(Cabecalho) + indicePagina * sizeof(Pagina), SEEK_SET);
    if (fread(p, sizeof(Pagina), 1, arquivo) != 1) {
        free(p);
        return;
    }

    // Define a identação para organização
    char indentacao[100] = "";
    for (int i = 0; i < nivel; i++) {
        strcat(indentacao, "    "); 
    }

    // Percorre e exibe as informações
    for (int i = 0; i < p->qtElementos; i++) {
        // Cast do ponteiro genérico para o tipo correto de chave do RH
        chaveComposta *chave = (chaveComposta*)p->chave[i];
        
        // Extrai o primeiro nome isolando a string até o primeiro espaço
        char primeiroNome[100];
        if (sscanf(chave->nome, "%s", primeiroNome) != 1) {
            strcpy(primeiroNome, chave->nome);
        }

        printf("%s%s (%02d/%02d/%04d)\n", indentacao,primeiroNome,chave->dataNascimento.dia,chave->dataNascimento.mes,chave->dataNascimento.ano);
    }

    // Se não for folha, desce recursivamente para os filhos incrementando o nível visual
    if (!p->ehfolha) {
        for (int i = 0; i <= p->qtElementos; i++) {
            imprimirArvoreRecursivo(arquivo, p->filho[i], nivel + 1);
        }
    }

    free(p); // Libera a memória RAM da página
}

// Diz à árvore quantos bytes a sua chave ocupa
size_t rh_tamanho_chave(const void* chave) {
    return sizeof(chaveComposta);
}

// Copia a struct chaveComposta do RH para o buffer da página
void rh_escrever_chave(const void* chave, void* buffer) {
    memcpy(buffer, chave, sizeof(chaveComposta));
}

// Puxa os bytes da página e joga de volta em uma struct do RH
void rh_ler_chave(void* destino, const void* buffer) {
    memcpy(destino, buffer, sizeof(chaveComposta));
}