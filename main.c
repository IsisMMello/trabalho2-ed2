
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RH.h"
#include "Bplus.h"

int main(){

    inicializarArvore(ORDEM, sizeof(chaveComposta));

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
        if (scanf("%d", &opcao) != 1) {
            // Entrada inválida (não numérica)
            printf("Entrada invalida. Digite um numero.\n");
            // Limpa o buffer
            while (getchar() != '\n');
            continue;
        }

        // Limpa o buffer do teclado
        while (getchar() != '\n');



        switch (opcao)
        {
            case 1:
                rh_inserir_funcionario();
                break;
            
            case 2:
                rh_buscar_funcionario();
                break;
            case 3:
                rh_excluir_funcionario();
                break;
            case 4:
                rh_listar_intervalo();
                break;
            case 5:
                imprimirArvore();
                break;

            case 6:
                printf("\n Encerrando o programa.\n");
                return false;
            
            default:
                printf("Opção indisponivel\n");

        }
    }while(true);



    return 0;
}
