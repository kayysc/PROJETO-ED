#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para comparar strings ignorando maiúsculas/minúsculas
int stringEqualsIgnoreCase(const char *str1, const char *str2) {
    char temp1[30], temp2[30];
    strcpy(temp1, str1);
    strcpy(temp2, str2);
    toLowerCase(temp1);
    toLowerCase(temp2);
    return strcmp(temp1, temp2) == 0;
}

// Função para alterar a cor no console
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Estrutura de rota (com origem e data)
typedef struct {
    char origem[30];
    char destino[30];
    char data[12];
    char horario[10];
    char preco[10];
    int linhas;
    int colunas;
    int capacidade;
} Rota;

// Rotas com informações de origem e data
Rota rotas[MAX_DESTINOS] = {
    {"Manaus", "Tabatinga", "25/11/2024", "08:00", "R$200", 5, 4, 20},
    {"Manaus", "Belém", "26/11/2024", "10:30", "R$150", 7, 4, 28},
    {"Manaus", "Santarém", "27/11/2024", "10:00", "R$250", 6, 4, 24},
    {"Manaus", "Parintins", "28/11/2024", "11:00", "R$130", 8, 4, 32}
};

// Funções para gerenciar assentos
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

// Função para exibir os assentos
void mostrarAssentos(char ***assentos, int linhas, int colunas, int posLinha, int posColuna) {
    printf("\n======== MAPA DE ASSENTOS ========\n");
    printf("        [1]  [2]     |     [3]  [4]  \n");
    printf("        /-----------------------\\\n");

    for (int i = 0; i < linhas; i++) {
        printf(" [%c] ", LETRAS[i]);
        for (int j = 0; j < colunas; j++) {
            if (j == 2) {
                printf("  |  ");
            }

            if (strcmp(assentos[i][j], "X") == 0) {
                setColor(12); // Vermelho para ocupados
                printf(" X   ");
            } else if (i == posLinha && j == posColuna) {
                setColor(14); // Amarelo para o assento selecionado
                printf(" %-4s", assentos[i][j]);
            } else {
                setColor(10); // Verde para disponíveis
                printf(" %-4s", assentos[i][j]);
            }
            setColor(7); // Reset para texto padrão
        }
        printf("\n");
    }
    printf("        \\-----------------------/\n");
    printf("==================================\n");
}

// Seleção de assentos com navegação
int escolherAssento(char ***assentos, int linhas, int colunas, char assentosEscolhidos[MAX_PASSAGENS][5], int *totalAssentosEscolhidos, Rota rota) {
    int posLinha = 0, posColuna = 0;

    while (*totalAssentosEscolhidos < MAX_PASSAGENS) {
        system(CLEAR);

        // Exibindo as informações do barco
        printf("Informações do Barco:\n");
        printf("Rota: %s -> %s\n", rota.origem, rota.destino);
        printf("Data: %s\n", rota.data);
        printf("Horário: %s\n", rota.horario);
        printf("Preço: %s\n", rota.preco);
        printf("Capacidade: %d assentos\n", rota.capacidade);
        printf("\n");

        mostrarAssentos(assentos, linhas, colunas, posLinha, posColuna);

        printf("\nUse as setas para navegar e pressione Enter para confirmar o assento.\n");
        printf("Pressione 'f' para finalizar a seleção, 'v' para voltar ao menu ou 'q' para sair.\n");

        int tecla = _getch();

        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posLinha > 0) posLinha--;
                    break;
                case 80: // Seta para baixo
                    if (posLinha < linhas - 1) posLinha++;
                    break;
                case 75: // Seta para esquerda
                    if (posColuna > 0) posColuna--;
                    break;
                case 77: // Seta para direita
                    if (posColuna < colunas - 1) posColuna++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            if (strcmp(assentos[posLinha][posColuna], "X") == 0) {
                printf("Assento já ocupado! Tente novamente.\n");
                _getch();
            } else {
                strcpy(assentos[posLinha][posColuna], "X");
                snprintf(assentosEscolhidos[*totalAssentosEscolhidos], 5, "%c-%d", LETRAS[posLinha], posColuna + 1);
                (*totalAssentosEscolhidos)++;
                printf("Assento %c-%d selecionado com sucesso!\n", LETRAS[posLinha], posColuna + 1);
                _getch();
            }
        } else if (tecla == 'f' || tecla == 'F') { // Finalizar seleção
            break;
        } else if (tecla == 'v' || tecla == 'V') { // Voltar
            return -1;
        } else if (tecla == 'q' || tecla == 'Q') { // Sair
            return 0;
        }
    }
    return 1;
}

// Função para comparar preços de rotas (para ordenação)
int compararPorPreco(const void *a, const void *b) {
    Rota *rotaA = (Rota *)a;
    Rota *rotaB = (Rota *)b;
    return strcmp(rotaA->preco, rotaB->preco);
}

// Função para comparar capacidade de rotas (para ordenação)
int compararPorCapacidade(const void *a, const void *b) {
    Rota *rotaA = (Rota *)a;
    Rota *rotaB = (Rota *)b;
    return rotaB->capacidade - rotaA->capacidade;
}

// Confirmação e geração de comprovante
int confirmarCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota) {
    system(CLEAR);
    printf("======= CONFIRMAR COMPRA =======\n");
    printf("Rota: %s -> %s\n", rota.origem, rota.destino);
    printf("Data: %s\n", rota.data);
    printf("Horário: %s\n", rota.horario);
    printf("Preço: %s\n", rota.preco);
    printf("Assentos Selecionados:\n");
    for (int i = 0; i < totalAssentos; i++) {
        printf("- %s\n", assentosEscolhidos[i]);
    }
    printf("===============================\n");
    printf("Deseja confirmar a compra? (S/N): ");

    char resposta;
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        system(CLEAR);
        printf("Obrigado por escolher o Banzeiro. Vamos navegar juntos para o futuro? Boa Viagem!!!\n");
        _getch();
        exit(1);
    } else {
        system(CLEAR);
        printf("Compra cancelada. Retornando ao menu principal.\n");
        _getch();
        return 0;
    }
}

// Função para filtrar rotas
void filtrarRotas() {
    system(CLEAR);
    printf("======= FILTRAR ROTAS =======\n");
    printf("1. Destinos Populares\n");
    printf("2. Destinos Acessíveis\n");
    printf("Escolha uma opção: ");

    int opcaoFiltro;
    scanf("%d", &opcaoFiltro);

    switch (opcaoFiltro) {
        case 1: // Destinos Populares
            system(CLEAR);
            qsort(rotas, MAX_DESTINOS, sizeof(Rota), compararPorCapacidade);
            printf("\nRotas ordenadas por popularidade (capacidade):\n");
            break;
        case 2: // Destinos Acessíveis
            system(CLEAR);
            qsort(rotas, MAX_DESTINOS, sizeof(Rota), compararPorPreco);
            printf("\nRotas ordenadas por preço (mais acessíveis primeiro):\n");
            break;
        default:
            system(CLEAR);
            printf("Opção inválida!\n");
            return;
    }

    // Exibir as rotas ordenadas
    for (int i = 0; i < MAX_DESTINOS; i++) {
        printf("%d. %s -> %s - %s - %s - %s\n", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario, rotas[i].preco);
    }

    printf("\nEscolha uma rota pelo número ou pressione 0 para voltar ao menu principal: ");
    int escolha;
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Volta ao menu principal
    } else if (escolha < 1 || escolha > MAX_DESTINOS) {
        printf("Opção inválida! Retornando ao menu principal...\n");
        _getch();
        return;
    }

    // Rota selecionada
    Rota rotaSelecionada = rotas[escolha - 1];
    char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
    char assentosEscolhidos[MAX_PASSAGENS][5];
    int totalAssentosEscolhidos = 0;

    // Navegação para seleção de assentos
    int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);
    if (resultado == 1 && totalAssentosEscolhidos > 0) {
        if (confirmarCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada)) {
            liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
        }
    } else {
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }
}

// Função para pesquisar rotas
void pesquisarRota() {
    char origem[30], destino[30], data[12];

    system(CLEAR);
    printf("======= PESQUISAR ROTA =======\n");
    printf("Origem: ");
    scanf(" %[^\n]", origem);
    printf("Destino: ");
    scanf(" %[^\n]", destino);
    printf("Data (DD/MM/AAAA): ");
    scanf(" %s", data);

    int encontrouRota = 0;
    int i;

    for (i = 0; i < MAX_DESTINOS; i++) {
        // Comparação insensível a maiúsculas/minúsculas
        if (stringEqualsIgnoreCase(rotas[i].origem, origem) &&
            stringEqualsIgnoreCase(rotas[i].destino, destino) &&
            strcmp(rotas[i].data, data) == 0) {  // Data é comparada de forma exata
            printf("\nRota encontrada:\n");
            printf("Origem: %s\n", rotas[i].origem);
            printf("Destino: %s\n", rotas[i].destino);
            printf("Data: %s\n", rotas[i].data);
            printf("Horário: %s\n", rotas[i].horario);
            printf("Preço: %s\n", rotas[i].preco);
            encontrouRota = 1;
            break;
        }
    }
}

void listarRotas() {
    system(CLEAR);
    printf("============== MENU DE ROTAS ==============\n");
    for (int i = 0; i < MAX_DESTINOS; i++) {
        printf("%d. %s -> %s - %s - %s\n", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario);
        printf("===========================================\n");
    }

    int rotaEscolhida;
    printf("\nDigite o número da rota desejada: ");
    scanf("%d", &rotaEscolhida);

    // Validação de entrada
    if (rotaEscolhida < 1 || rotaEscolhida > MAX_DESTINOS) {
        printf("Rota inválida! Retornando ao menu principal.\n");
        _getch();
        return;
    }

    Rota rotaSelecionada = rotas[rotaEscolhida - 1];
    char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
    char assentosEscolhidos[MAX_PASSAGENS][5];
    int totalAssentosEscolhidos = 0;

    int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);

    if (resultado == 1 && totalAssentosEscolhidos > 0) {
        int confirmacao = confirmarCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada);
        if (confirmacao == 1) {
            printf("Compra confirmada! Obrigado.\n");
            liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
            return; // Sai da função após compra
        } else {
            printf("Compra cancelada. Retornando ao menu principal...\n");
            _getch();
            liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
        }
    } else {
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }
}

// Menu principal
int main() {
    setlocale(LC_ALL, "");
    int escolha, sair = 0;

    while (!sair) {
        system(CLEAR);
        printf("======= MENU PRINCIPAL =======\n");
        printf("1. Pesquisar Rota\n");
        printf("2. Listar Rotas Disponíveis\n");
        printf("3. Filtros\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        // Limpa o buffer de entrada
        while (getchar() != '\n');

        switch (escolha) {
            case 1:
                pesquisarRota();
                break;

            case 2:
                listarRotas(); // Encapsule a lógica de rotas em uma função separada
                break;

            case 3:
                filtrarRotas();
                break;

            case 4:
                printf("Encerrando o programa. Até mais!\n");
                sair = 1; // Marca a saída do loop principal
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                _getch(); // Aguarda entrada para não pular a mensagem
        }
    }

    return 0;
}
