#include <stdio.h>
#include "meu_banco.h"

int main() {
    int op;

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Abrir o banco\n");
        printf("2 - Consultar historico\n");
        printf("3 - Efetuar sorteio\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);

        if (op == 1) {
            abrirBanco();
        } else if (op == 2) {
            consultarHistorico();
        } else if (op == 3) {
            efetuarSorteio();
        } else if (op == 0) {
            liberarBanco();
            printf("Encerrando...\n");
        } else {
            printf("Opcao invalida!\n");
        }
    } while (op != 0);

    return 0;
}