#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3

typedef struct {
    char rota[30];
    char horario[10];
    char preco[10];
    int linhas;
    int colunas;
} Rota;

Rota rotas[MAX_DESTINOS] = {
    {"Manaus -> Tabatinga", "08:00", "R$200", 5, 4},
    {"Manaus -> Belém", "10:30", "R$150", 7, 4},
    {"Manaus -> Santarém", "10:00", "R$250", 6, 4},
    {"Manaus -> Parintins", "11:00", "R$130", 8, 4}
};

void inicializarAssentos(char ***assentos, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            sprintf(assentos[i][j], "%c-%d", LETRAS[i], j + 1);
        }
    }
}

char ***criarAssentos(int linhas, int colunas) {
    char ***assentos = (char ***)malloc(linhas * sizeof(char **));
    for (int i = 0; i < linhas; i++) {
        assentos[i] = (char **)malloc(colunas * sizeof(char *));
        for (int j = 0; j < colunas; j++) {
            assentos[i][j] = (char *)malloc(4 * sizeof(char));
        }
    }
    inicializarAssentos(assentos, linhas, colunas);
    return assentos;
}

void liberarAssentos(char ***assentos, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            free(assentos[i][j]);
        }
        free(assentos[i]);
    }
    free(assentos);
}

void mostrarAssentos(char ***assentos, int linhas, int colunas) {
    printf("\n======== MAPA DE ASSENTOS ========\n");
    printf("      [1]  [2]   |   [3]  [4]  \n");
    printf("     /-----------------------\\\n");

    for (int i = 0; i < linhas; i++) {
        printf(" [%c] ", LETRAS[i]);
        for (int j = 0; j < colunas; j++) {
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
    printf("==================================\n");
}

int escolherAssento(char ***assentos, int linhas, int colunas, char letra, int numero) {
    int linha = letra - 'A';
    int coluna = numero - 1;

    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas) {
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

void exibirMenuRotas() {
    printf("======== MENU DE ROTAS ========\n");
    for (int i = 0; i < MAX_DESTINOS; i++) {
        printf("Barco %d\n", i + 1);
        printf("%s\n", rotas[i].rota);
        printf("Horário: %s\n", rotas[i].horario);
        printf("Preço: %s\n", rotas[i].preco);
        printf("Capacidade: %d\n", rotas[i].linhas * rotas[i].colunas);
        printf("=================================\n");
    }
    printf("Escolha o número do barco para mais detalhes ou 0 para voltar ao menu principal:\n");
}

void imprimirComprovante(Rota rotaSelecionada, char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos) {
    int precoUnitario = atoi(rotaSelecionada.preco + 2); // Remove o "R$" para pegar o valor
    int precoTotal = precoUnitario * totalAssentos;

    printf("\n======== COMPROVANTE DE COMPRA ========\n");
    printf("Rota: %s\n", rotaSelecionada.rota);
    printf("Horário: %s\n", rotaSelecionada.horario);
    printf("Assentos escolhidos:\n");
    for (int i = 0; i < totalAssentos; i++) {
        printf(" - %s\n", assentosEscolhidos[i]);
    }
    printf("Preço total: R$%d\n", precoTotal);
    printf("=======================================\n");
}

void exibirCompraFinal(Rota rotaSelecionada, char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos) {
    system(CLEAR);
    imprimirComprovante(rotaSelecionada, assentosEscolhidos, totalAssentos);
    printf("\n======= SUA COMPRA =======\n");
    printf("1. Confirmo compra. Encerrar programa.\n");
    printf("Escolha uma opção: ");
    
    int opcao;
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        system(CLEAR);
        printf("Obrigado por escolher o Banzeiro. Vamos navegar juntos para o futuro? Boa Viagem!!!\n");
    }
}

int main() {
    setlocale(LC_ALL, "");  // Configuração para suporte a caracteres locais

    int escolha, sair = 0;
    char letra;
    int numero;

    while (!sair) {
        system(CLEAR);

        printf("======= MENU PRINCIPAL =======\n");
        printf("1. Selecionar Rota\n");
        printf("2. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);
        limparBuffer();

        switch (escolha) {
            case 1:
                while (1) {
                    system(CLEAR);
                    exibirMenuRotas();
                    int rotaEscolhida;
                    scanf("%d", &rotaEscolhida);
                    limparBuffer();

                    if (rotaEscolhida == 0) break;  // Volta ao menu principal

                    if (rotaEscolhida < 1 || rotaEscolhida > MAX_DESTINOS) {
                        printf("Opção inválida! Tente novamente.\n");
                        continue;
                    }

                    Rota rotaSelecionada = rotas[rotaEscolhida - 1];
                    char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
                    char assentosEscolhidos[MAX_PASSAGENS][5];
                    int totalAssentosEscolhidos = 0;

                    while (1) {
                        system(CLEAR);
                        printf("Informações do Barco:\n");
                        printf("Rota: %s\n", rotaSelecionada.rota);
                        printf("Horário: %s\n", rotaSelecionada.horario);
                        printf("Preço: %s\n", rotaSelecionada.preco);
                        printf("Capacidade: %d\n\n", rotaSelecionada.linhas * rotaSelecionada.colunas);
                        mostrarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
                        printf("\nEscolha seu assento (letra e número) ou '0 0' para concluir:\n");

                        printf("Letra: ");
                        scanf(" %c", &letra);
                        printf("Número: ");
                        scanf("%d", &numero);
                        limparBuffer();

                        if (letra == '0' && numero == 0) break;

                        if (totalAssentosEscolhidos >= MAX_PASSAGENS) {
                            printf("Limite de 3 assentos atingido!\n");
                            printf("1. Concluir compra\n2. Sair\nEscolha uma opção: ");
                            int opcaoLimite;
                            scanf("%d", &opcaoLimite);
                            limparBuffer();

                            if (opcaoLimite == 1) {
                                break;
                            } else if (opcaoLimite == 2) {
                                liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
                                return 0;
                            } else {
                                printf("Opção inválida. Voltando ao menu de assentos.\n");
                                continue;
                            }
                        }

                        if (escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, letra, numero)) {
                            snprintf(assentosEscolhidos[totalAssentosEscolhidos], 5, "%c-%d", letra, numero);
                            totalAssentosEscolhidos++;

                            printf("\n1. Escolher outro assento\n2. Concluir compra\nEscolha uma opção: ");
                            int opcao;
                            scanf("%d", &opcao);
                            limparBuffer();

                            if (opcao == 2) break;
                        }
                    }

                    liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
                    exibirCompraFinal(rotaSelecionada, assentosEscolhidos, totalAssentosEscolhidos);
                    sair = 1;
                    break;
                }
                break;

            case 2:
                sair = 1;
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }

    return 0;
}

