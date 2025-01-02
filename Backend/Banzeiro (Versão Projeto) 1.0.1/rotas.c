#include "auxiliar.h"

//Variáveis globais
int i,j;

// Rotas com informações de origem e data
Rota rotas[MAX_DESTINOS] = {
    {"Manaus", "Tabatinga", "25/11/2024", "08:00", "R$200", 5, 4, 20},
    {"Manaus", "Belém", "26/11/2024", "10:30", "R$150", 7, 4, 28},
    {"Manaus", "Santarém", "27/11/2024", "10:00", "R$250", 6, 4, 24},
    {"Manaus", "Parintins", "28/11/2024", "11:00", "R$130", 8, 4, 32}
};

// Funções de gerenciamento de assentos
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

void mostrarAssentos(char ***assentos, int linhas, int colunas, int posLinha, int posColuna) {
    printf("\n ============= MAPA DE ASSENTOS =============\n\n");
    printf("             [1]  [2]   |   [3]  [4]  \n");
    printf("            /-----------------------\\\n");
    for (i = 0; i < linhas; i++) {
        printf("        [%c] ", LETRAS[i]);
        for (j = 0; j < colunas; j++) {
            if (j == 2) {
                printf("  |  ");
            }

            if (strcmp(assentos[i][j], "X") == 0) {
                setColor(12); // Vermelho para ocupados
                printf("  X  ");
            } else if (i == posLinha && j == posColuna) {
                setColor(14); // Amarelo para o assento selecionado
                printf(" %-4s", assentos[i][j]);
            } else {
                setColor(10); // Verde para disponíveis
                printf(" %-4s", assentos[i][j]);
            }
            setColor(15); // Reset para texto padrão
        }
        printf("\n");
    }
    printf("            \\-----------------------/\n");
    printf("\n ===========================================\n");
}

int escolherAssento(char ***assentos, int linhas, int colunas, char assentosEscolhidos[MAX_PASSAGENS][5], int *totalAssentosEscolhidos, Rota rota) {
    int posLinha = 0, posColuna = 0;
    while (*totalAssentosEscolhidos < MAX_PASSAGENS) {
        system(CLEAR);
        // Exibindo as informações do barco

        printf("\n ============= INFORMAÇÕES DO BARCO =============\n\n");
        printf("       Rota: %s -> %s\n", rota.origem, rota.destino);
        printf("       Data: %s\n", rota.data);
        printf("       Horário: ");
        setColor(9);
        printf("%s\n", rota.horario);
        setColor(15);
        printf("       Preço: ");
        setColor(10);
        printf("%s\n", rota.preco);
        setColor(15);
        printf("       Capacidade: %d assentos\n", rota.capacidade);
        printf("\n");

        mostrarAssentos(assentos, linhas, colunas, posLinha, posColuna);
        printf("\n       Use as setas para navegar;\n");
        printf("       Pressione 'Enter' para confirmar o assento;\n");
        printf("       Pressione 'F' para finalizar a seleção;\n");
        printf("       Pressione 'V' para voltar ao menu;\n");
        printf("       Pressione 'Q' para sair.\n");

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
                printf("\n       ");
                setColor(12);
                printf("Assento já ocupado");
                setColor(15);
                printf("! Tente novamente.\n");
                _getch();
            } else {
                strcpy(assentos[posLinha][posColuna], "X");
                snprintf(assentosEscolhidos[*totalAssentosEscolhidos], 5, "%c-%d", LETRAS[posLinha], posColuna + 1);
                (*totalAssentosEscolhidos)++;
                printf("\n       Assento ");
                setColor(14);
                printf("%c-%d", LETRAS[posLinha], posColuna + 1);
                setColor(15);
                printf(" selecionado com ");
                setColor(10);
                printf("sucesso");
                setColor(15);
                printf("!\n");
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

// Funções para salvar e carregar o estado dos assentos
void salvarAssentos(char ***assentos, int linhas, int colunas, const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para salvar os assentos.\n");
        return;
    }

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            fprintf(fp, "%s ", assentos[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void carregarAssentos(char ***assentos, int linhas, int colunas, const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        // Se o arquivo não existir, inicializa os assentos como disponíveis
        inicializarAssentos(assentos, linhas, colunas);
        return;
    }

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            fscanf(fp, "%s", assentos[i][j]);
        }
    }

    fclose(fp);
}

void listarRotas(struct cliente **head_cliente) {
    const char *opcoes[MAX_DESTINOS + 1];
    for (i = 0; i < MAX_DESTINOS; i++) {
        char temp[50];
        sprintf(temp, "%d. %s -> %s - %s - %s", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario);
        opcoes[i] = strdup(temp);
    }
    opcoes[MAX_DESTINOS] = "Voltar";

    int escolha = mostrarMenu("MENU DE ROTAS", opcoes, MAX_DESTINOS + 1);
    if (escolha >= 0 && escolha < MAX_DESTINOS) {
        Rota rotaSelecionada = rotas[escolha];
        char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);

        // Carrega o estado dos assentos da rota
        char nomeArquivoAssentos[50];
        sprintf(nomeArquivoAssentos, "assentos_rota_%d.txt", escolha); // Nome do arquivo baseado no índice da rota
        carregarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, nomeArquivoAssentos);

        char assentosEscolhidos[MAX_PASSAGENS][5];
        int totalAssentosEscolhidos = 0;

        int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);
        if (resultado == 1 && totalAssentosEscolhidos > 0) {
            detalhesDaCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada, assentos, head_cliente);
        }

        // Salva o estado dos assentos da rota após a compra
        salvarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, nomeArquivoAssentos);

        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }
}
