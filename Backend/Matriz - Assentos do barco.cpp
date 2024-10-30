#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define LINHAS 7
#define COLUNAS 4
#define LETRAS "ABCDEFG"

void inicializarAssentos(char assentos[LINHAS][COLUNAS][4]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            sprintf(assentos[i][j], "%c-%d", LETRAS[i], j + 1);
        }
    }
}

void mostrarAssentos(char assentos[LINHAS][COLUNAS][4]) {
    printf("\n======== MAPA DE ASSENTOS ========\n");
    printf("\n");
    printf("      [1]  [2]   |   [3]  [4]  \n");
    printf("     /-----------------------\\\n");

    for (int i = 0; i < LINHAS; i++) {
        printf(" [%c] ", LETRAS[i]);
        for (int j = 0; j < COLUNAS; j++) {
            if (j == 2) {
                printf("  |  ");
            }
            if (strcmp(assentos[i][j], "X") == 0) {
                printf(" X   ");
            } else {
                printf(" %-4s", assentos[i][j]);
            }
        }
        printf("\n");
    }
    printf("     \\-----------------------/\n");
    printf("\n");
    printf("==================================\n");
}

int escolherAssento(char assentos[LINHAS][COLUNAS][4], char letra, int numero) {
    int linha = letra - 'A';
    int coluna = numero - 1;

    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS) {
        printf("Assento inválido! Tente novamente.\n");
        return 0;
    }

    if (strcmp(assentos[linha][coluna], "X") == 0) {
        printf("Assento já ocupado! Escolha outro.\n");
        return 0;
    }

    strcpy(assentos[linha][coluna], "X");
    printf("Assento %c-%d escolhido com sucesso!\n", letra, numero);
    return 1;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    char assentos[LINHAS][COLUNAS][4];
    char letra;
    int numero;
    int sair = 0;

    inicializarAssentos(assentos);

    while (!sair) {
        system(CLEAR);

        mostrarAssentos(assentos);
        printf("\nEscolha seu assento ou digite 'S' para sair.\n");
        printf("Digite a letra do assento (A-G): ");
        scanf(" %c", &letra);

        if (letra == 'S' || letra == 's') {
            sair = 1;
            break;
        }

        printf("Digite o número do assento (1-4): ");
        scanf("%d", &numero);

        if (!escolherAssento(assentos, letra, numero)) {
            printf("Tente novamente.\n");
            limparBuffer();
        }
    }

    printf("Programa encerrado. Boa viagem!\n");
    return 0;
}
