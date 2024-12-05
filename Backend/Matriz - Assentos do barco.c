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

int i, j;

void toLowerCase(char *str) {
    for (i = 0; str[i]; i++) {
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
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            sprintf(assentos[i][j], "%c-%d", LETRAS[i], j + 1);
        }
    }
}

char ***criarAssentos(int linhas, int colunas) {
    char ***assentos = (char ***)malloc(linhas * sizeof(char **));
    for (i = 0; i < linhas; i++) {
        assentos[i] = (char **)malloc(colunas * sizeof(char *));
        for (j = 0; j < colunas; j++) {
            assentos[i][j] = (char *)malloc(4 * sizeof(char));
        }
    }
    inicializarAssentos(assentos, linhas, colunas);
    return assentos;
}

void liberarAssentos(char ***assentos, int linhas, int colunas) {
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            free(assentos[i][j]);
        }
        free(assentos[i]);
    }
    free(assentos);
}

// Função para exibir os assentos
void mostrarAssentos(char ***assentos, int linhas, int colunas, int posLinha, int posColuna) {
    printf("\n======== MAPA DE ASSENTOS ========\n");
    printf("        [1]  [2]     |   [3]  [4]  \n");
    printf("        /-----------------------\\\n");

    for (i = 0; i < linhas; i++) {
        printf(" [%c] ", LETRAS[i]);
        for (j = 0; j < colunas; j++) {
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

int detalhesDaCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota, char ***assentos) {
    int opcao = -1; // Inicializar com valor inválido
    int tecla, posicao = 0; // Inicializa a posição do cursor

    do {
        system(CLEAR);
        printf("======= DETALHES DA COMPRA =======\n");
        printf("Rota: %s -> %s\n", rota.origem, rota.destino);
        printf("Data: %s\n", rota.data);
        printf("Horário: %s\n", rota.horario);
        printf("Assentos Selecionados:\n");
        for (i = 0; i < totalAssentos; i++) {
            printf("- %s\n", assentosEscolhidos[i]);
        }
        printf("===============================\n");

        // Exibe as opções do menu com a seta indicando a opção atual
        setColor(posicao == 0 ? 14 : 7); // Amarelo para a opção selecionada
        printf("-> 1. Confirmar Compra\n");
        setColor(posicao == 1 ? 14 : 7);
        printf("-> 2. Trocar Assentos\n");
        setColor(posicao == 2 ? 14 : 7);
        printf("-> 0. Cancelar Compra\n");
        setColor(7); // Reset para a cor padrão

        tecla = _getch();

        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posicao > 0) posicao--;
                    break;
                case 80: // Seta para baixo
                    if (posicao < 2) posicao++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            opcao = posicao; // Define a opção com base na posição do cursor
        }

        // Adicionando um tratamento para a opção
        switch (opcao) {
            case 0: // Confirmar Compra
                system(CLEAR);
                printf("Obrigado por escolher o Banzeiro. Vamos navegar juntos para o futuro? Boa Viagem!!!\n");
                _getch();
                exit(1); // Compra confirmada
            case 1: // Trocar Assentos
                // Libera os assentos selecionados anteriormente
                for (i = 0; i < totalAssentos; i++) {
                    int linha = assentosEscolhidos[i][0] - 'A';
                    int coluna = assentosEscolhidos[i][2] - '1';
                    strcpy(assentos[linha][coluna], assentosEscolhidos[i]);
                }

                // Reinicia a contagem de assentos
                totalAssentos = 0;

                // Volta para a seleção de assentos
                escolherAssento(assentos, rota.linhas, rota.colunas, assentosEscolhidos, &totalAssentos, rota);
                opcao = -1; // Resetando a opção para voltar ao menu detalhesDaCompra
                break;
            case 2: // Cancelar Compra
                system(CLEAR);
                printf("Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorna ao menu principal
            default:
                break; // Evitar execução de outros casos
        }
    } while (opcao != 2); // Continua enquanto não cancelar
    return 0; // Finaliza adequadamente
}

// Função genérica para exibir menus com navegação por teclado
int mostrarMenu(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;

    do {
        system(CLEAR);
        printf("======= %s =======\n", titulo); // Exibe o título do menu

        // Exibe as opções do menu com a seta indicando a opção atual
        for (i = 0; i < numOpcoes; i++) {
            setColor(posicao == i ? 14 : 7); // Amarelo para a opção selecionada
            printf("-> %s\n", opcoes[i]);
            setColor(7); // Reset para a cor padrão
        }

        tecla = _getch();

        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posicao > 0) posicao--;
                    break;
                case 80: // Seta para baixo
                    if (posicao < numOpcoes - 1) posicao++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            return posicao; // Retorna a opção selecionada
        }
    } while (tecla != 27); // ESC para sair do menu

    return -1; // Retorna -1 se o usuário pressionar ESC
}

// Função para listar as rotas disponíveis (usando a função genérica mostrarMenu)
void listarRotas() {
    const char *opcoes[MAX_DESTINOS];
    for (i = 0; i < MAX_DESTINOS; i++) {
        char temp[50];
        sprintf(temp, "%d. %s -> %s - %s - %s", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario);
        opcoes[i] = strdup(temp); // Aloca memória para a string
    }

    int escolha = mostrarMenu("MENU DE ROTAS", opcoes, MAX_DESTINOS);

    if (escolha >= 0 && escolha < MAX_DESTINOS) {
        Rota rotaSelecionada = rotas[escolha];
        char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
        char assentosEscolhidos[MAX_PASSAGENS][5];
        int totalAssentosEscolhidos = 0;

        int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);
        if (resultado == 1 && totalAssentosEscolhidos > 0) {
            detalhesDaCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada, assentos);
        }
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }

    // Libera a memória alocada para as strings
    for (i = 0; i < MAX_DESTINOS; i++) {
        free((char *)opcoes[i]);
    }
}
// Função para filtrar rotas (usando a função genérica mostrarMenu)
void filtrarRotas(int filtro) {
    // Ordena as rotas com base no filtro
    if (filtro == 1) {
        qsort(rotas, MAX_DESTINOS, sizeof(Rota), compararPorCapacidade);
    } else if (filtro == 2) {
        qsort(rotas, MAX_DESTINOS, sizeof(Rota), compararPorPreco);
    }

    const char *opcoes[MAX_DESTINOS + 1]; // +1 para a opção "Voltar"
    for (i = 0; i < MAX_DESTINOS; i++) {
        char temp[80];
        sprintf(temp, "%d. %s -> %s - %s - %s - %s", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario, rotas[i].preco);
        opcoes[i] = strdup(temp);
    }
    opcoes[MAX_DESTINOS] = "0. Voltar ao menu principal";

    int escolha = mostrarMenu(filtro == 1 ? "DESTINOS MAIS POPULARES" : "DESTINOS MAIS ACESSÍVEIS", opcoes, MAX_DESTINOS + 1);

    if (escolha >= 0 && escolha < MAX_DESTINOS) {
        Rota rotaSelecionada = rotas[escolha];
        char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
        char assentosEscolhidos[MAX_PASSAGENS][5];
        int totalAssentosEscolhidos = 0;

        int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);
        if (resultado == 1 && totalAssentosEscolhidos > 0) {
            detalhesDaCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada, assentos);
        }
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }

    // Libera a memória alocada para as strings
    for (i = 0; i <= MAX_DESTINOS; i++) { // <= para incluir a opção "Voltar"
        free((char *)opcoes[i]);
    }
}

// Menu principal atualizado (usando a função genérica mostrarMenu)
int main() {
    setlocale(LC_ALL, "");

    const char *opcoes_menu_principal[] = {
        "1. Listar Rotas Disponíveis",
        "2. Destinos Mais Populares",
        "3. Destinos Mais Acessíveis",
        "4. Sair"
    };
    int num_opcoes_menu_principal = sizeof(opcoes_menu_principal) / sizeof(opcoes_menu_principal[0]);

    int escolha;

    do {
        escolha = mostrarMenu("MENU DE ROTAS", opcoes_menu_principal, num_opcoes_menu_principal);

        switch (escolha) {
            case 0:
                listarRotas();
                break;
            case 1:
                filtrarRotas(1); // Filtro de destinos mais populares
                break;
            case 2:
                filtrarRotas(2); // Filtro de destinos mais acessíveis
                break;
            case 3:
                printf("Encerrando o programa. Até mais!\n");
                break;
        }
    } while (escolha != 3); // 3 é o índice da opção "Sair"

    return 0;
}
