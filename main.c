/*
 * ============================================================
 * ARQUIVO: main.c
 * ============================================================
 * 
 * PROGRAMA PRINCIPAL – MENU INTERATIVO DO SISTEMA DE RH
 * 
 * ------------------------------------------------------------
 * PASSO 1 – INICIALIZAÇÃO DO SISTEMA
 * ------------------------------------------------------------
 * 1.1. Abrir / criar o arquivo de índice da B+ Tree (bplus_init).
 * 1.2. Registrar os callbacks (comparador, tamanho, serialização).
 * 1.3. Carregar eventuais dados persistentes (se houver arquivo de dados).
 * 
 * ------------------------------------------------------------
 * PASSO 2 – LOOP PRINCIPAL DO MENU
 * ------------------------------------------------------------
 * 2.1. Exibir opções numeradas (1 a 6).
 * 2.2. Ler a opção do usuário.
 * 2.3. Switch para cada opção.
 * 
 * ------------------------------------------------------------
 * PASSO 3 – IMPLEMENTAÇÃO DOS CASES
 * ------------------------------------------------------------
 * 3.1. Case 1: chamar rh_inserir_funcionario().
 * 3.2. Case 2: chamar rh_buscar_funcionario().
 * 3.3. Case 3: chamar rh_excluir_funcionario().
 * 3.4. Case 4: chamar rh_listar_por_intervalo().
 * 3.5. Case 5: chamar bplus_print_tree() – exibe estrutura hierárquica.
 * 3.6. Case 6: encerrar (break do loop).
 * 
 * ------------------------------------------------------------
 * PASSO 4 – FINALIZAÇÃO SEGURA (OPÇÃO SAIR)
 * ------------------------------------------------------------
 * 4.1. Escrever cabeçalho atualizado.
 * 4.2. Fechar arquivos (bplus_close).
 * 4.3. Liberar memória alocada (se houver).
 * 4.4. Exibir mensagem de encerramento.
 * 
 * ------------------------------------------------------------
 * PASSO 5 – TRATAMENTO DE ERROS E ENTRADAS INVÁLIDAS
 * ------------------------------------------------------------
 * 5.1. Validar opções fora do intervalo.
 * 5.2. Limpar buffer do teclado após leituras.
 * 5.3. Capturar interrupções (SIGINT) para não corromper arquivos.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int opcao;
    do{
        printf("\n========================================\n");
        printf("  MENU PRINCIPAL\n");
        printf("========================================\n");
        printf("  1 - Inserir Funcionario\n");
        printf("  2 - Buscar Funcionario\n");
        printf("  3 - Excluir Funcionario\n");
        printf("  4 - Listar por Intervalo\n");
        printf("  5 - Exibir Estrutura do Ìndice\n");
        printf("  6 - sair\n");
        printf("========================================\n");
        
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}


        switch (opcao)
        {
            case 1:
            
                break;
            
            case 2:
            
                break;
            case 3:
            
                break;
            case 4:
            
                break;
            case 5:
            
                break;

            case 6:
            return false;
            
            default:
                print("Opção indisponivel\n");

        }
    }while(true);



    return 0;
}