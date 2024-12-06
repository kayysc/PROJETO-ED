#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define RESET "\033[0m"
#define HIGHLIGHT "\033[1;32m" 
#define NORMAL "\033[0;37m"

#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3

int i, j;
int prox_id = 1;

void toLowerCase(char *str) {
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

struct cliente {
    int id;
    char nome[50];
    char cpf[15];
    char data_nasc[11];
    char telefone[20];
    struct cliente *proximo_cl; // Lista encadeada
};

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

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void remover_formatacao_cpf(const char *cpf_formatado, char *cpf_limpo) {
    int j = 0;
    for (int i = 0; cpf_formatado[i] != '\0'; i++) {
        if (isdigit(cpf_formatado[i])) {
            cpf_limpo[j++] = cpf_formatado[i];
        }
    }
    cpf_limpo[j] = '\0';
}

// Função para validar e formatar CPF
void ler_cpf_formatado(char *cpf) {
    int pos = 0;
    char c;

    printf("CPF: ");
    while (pos < 14) {
        c = getch();

        if (c == '\b' && pos > 0) {
            pos--;
            if (cpf[pos] == '.' || cpf[pos] == '-') pos--;
            cpf[pos] = '\0';
            printf("\b \b");
        } else if (isdigit(c)) {
            if (pos == 3 || pos == 7) {
                cpf[pos++] = '.';
                printf(".");
            } else if (pos == 11) {
                cpf[pos++] = '-';
                printf("-");
            }
            cpf[pos++] = c;
            printf("%c", c);
        }
    }
    cpf[pos] = '\0';
    printf("\n");
}

// Função para calcular idade com base na data de nascimento
int calcular_idade(const char *data_nasc) {
    int dia, mes, ano;
    sscanf(data_nasc, "%d/%d/%d", &dia, &mes, &ano);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int idade = tm.tm_year + 1900 - ano;

    if (tm.tm_mon + 1 < mes || (tm.tm_mon + 1 == mes && tm.tm_mday < dia)) {
        idade--;
    }

    return idade;
}

// Função para validar e formatar data
int validar_data(const char *data) {
    int dia, mes, ano;

    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;

    if (ano < 1900 || ano > 2100 || mes < 1 || mes > 12 || dia < 1 || dia > 31) return 0;

    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > (bissexto ? 29 : 28)) return 0;
    }
    return 1;
}

void ler_data_formatada(char *data) {
    int pos = 0;
    char c;

    printf("Data de Nascimento (dd/mm/aaaa): ");
    while (pos < 10) {
        c = getch();
        if (c == '\b' && pos > 0) {
            pos--;
            if (data[pos] == '/') pos--;
            data[pos] = '\0';
            printf("\b \b");
        } else if (isdigit(c)) {
            if (pos == 2 || pos == 5) {
                data[pos++] = '/';
                printf("/");
            }
            data[pos++] = c;
            printf("%c", c);
        }
    }
    data[pos] = '\0';
    printf("\n");
}

// Função para validar e formatar telefone
void ler_telefone_formatado(char *telefone) {
    int pos = 0;
    char c;

    printf("Telefone: ");
    while (pos < 15) {
        c = getch();
        if (c == '\b' && pos > 0) {
            pos--;
            if (telefone[pos] == ')' || telefone[pos] == ' ' || telefone[pos] == '-') pos--;
            telefone[pos] = '\0';
            printf("\b \b");
        } else if (isdigit(c)) {
            if (pos == 0) {
                telefone[pos++] = '(';
                printf("(");
            } else if (pos == 3) {
                telefone[pos++] = ')';
                telefone[pos++] = ' ';
                printf(") ");
            } else if (pos == 9) {
                telefone[pos++] = '-';
                printf("-");
            }
            telefone[pos++] = c;
            printf("%c", c);
        }
    }
    telefone[pos] = '\0';
    printf("\n");
}

// Cadastro de cliente
void cadastrar_cliente(struct cliente **head) {
    struct cliente *novo_cl = (struct cliente *)malloc(sizeof(struct cliente));
    if (!novo_cl) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    novo_cl->id = prox_id++;

    printf("Nome: ");
    fflush(stdin);
    fgets(novo_cl->nome, 50, stdin);
    strtok(novo_cl->nome, "\n");

    do {
        ler_cpf_formatado(novo_cl->cpf);
    } while (strlen(novo_cl->cpf) != 14);

    do {
        ler_data_formatada(novo_cl->data_nasc);
        if (!validar_data(novo_cl->data_nasc)) {
            printf("Data inválida! Tente novamente.\n");
        }
    } while (!validar_data(novo_cl->data_nasc));

    do {
        ler_telefone_formatado(novo_cl->telefone);
    } while (strlen(novo_cl->telefone) != 15);

    novo_cl->proximo_cl = *head;
    *head = novo_cl;

    printf("\nCliente cadastrado com sucesso!\n");
    getch();
}

// Buscar cliente por CPF
struct cliente* buscar_cliente_por_cpf(struct cliente *head, const char *cpf) {
    struct cliente *atual = head;
    char cpf_limpo[12], cpf_atual_limpo[12];

    remover_formatacao_cpf(cpf, cpf_limpo);

    while (atual != NULL) {
        remover_formatacao_cpf(atual->cpf, cpf_atual_limpo);

        if (strcmp(cpf_atual_limpo, cpf_limpo) == 0) {
            // Não precisa imprimir os dados do cliente aqui
            return atual; // Retorna o ponteiro para o cliente encontrado
        }
        atual = atual->proximo_cl;
    }

    return NULL; // Retorna NULL se o cliente não for encontrado
}

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

void finalizarCompra(int totalAssentos, Rota rota, char assentosEscolhidos[MAX_PASSAGENS][5]) {
    char opcao;
    float preco_total;

    system(CLEAR);
    printf("===== FINALIZAR COMPRA =====\n\n");
    printf("Inteira ou Meia? (I/M): ");
    scanf(" %c", &opcao);

    if (opcao == 'I' || opcao == 'i') {
        preco_total = totalAssentos * atof(rota.preco + 2); // Converte o preço da rota para float, ignorando "R$"
    } else if (opcao == 'M' || opcao == 'm') {
        printf("Necessário comprovação de benefício\n");
        preco_total = totalAssentos * (atof(rota.preco + 2) / 2); // Converte o preço da rota para float, ignorando "R$"
    } else {
        printf("Opção inválida! Pressione qualquer tecla para continuar.\n");
        _getch();
        finalizarCompra(totalAssentos, rota, assentosEscolhidos); // Reinicia a função
        return;
    }

    system(CLEAR);
    printf("===== FINALIZAR COMPRA =====\n");
    printf("Rota: %s -> %s\n", rota.origem, rota.destino);
    printf("Data: %s\n", rota.data);
    printf("Horário: %s\n", rota.horario);
    printf("Assentos Selecionados:\n");
    for (i = 0; i < totalAssentos; i++) {
        printf("- %s\n", assentosEscolhidos[i]);
    }
    printf("Preço total: R$ %.2f\n", preco_total);
    printf("============================\n");
    printf("Confirma compra? (S/N): ");
    scanf(" %c", &opcao);

    if (opcao == 'S' || opcao == 's') {
        // TODO: Imprimir comprovante ou finalizar a compra
        printf("Compra confirmada! Pressione qualquer tecla para continuar.\n");
        _getch();
    } else {
        printf("Compra cancelada! Voltando ao Menu Principal...\n");
        _getch();
    }
}

// Função para exibir os detalhes da compra e permitir a troca de assentos
int detalhesDaCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota, char ***assentos, struct cliente **head_cliente) {
    char resposta; // Variável para armazenar a resposta do usuário
    char cpf_busca[15]; // Variável para armazenar o CPF do cliente
    struct cliente cliente_temp; // Variável temporária para armazenar os dados do cliente

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

    // Pergunta sobre o cadastro do cliente
    printf("Cliente possui cadastro? (S/N): ");
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        system(CLEAR);
        printf("=== BUSCAR CLIENTE POR CPF ===\n");
        printf("Digite o CPF do cliente");
        printf("\n");
        ler_cpf_formatado(cpf_busca);

        // Chama a função buscar_cliente_por_cpf e armazena o resultado
        struct cliente *cliente_encontrado = buscar_cliente_por_cpf(*head_cliente, cpf_busca); 

        if (cliente_encontrado != NULL) { // Verifica se o cliente foi encontrado
            printf("\nCliente Identificado: %s!!! Prosseguir com a compra? (S/N) ", cliente_encontrado->nome); 
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos); // Chama a função finalizarCompra
            } else {
                printf("Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorna ao menu principal
            }
        } else {
            // Cliente não encontrado - talvez voltar ao menu ou perguntar se quer cadastrar
            printf("Cliente não encontrado. Pressione qualquer tecla para continuar...\n");
            _getch();
        }
    } else if (resposta == 'N' || resposta == 'n') {
        printf("Cliente deseja realizar cadastro? (S/N): ");
        scanf(" %c", &resposta);
        if (resposta == 'S' || resposta == 's') {
            system(CLEAR);
            printf("=== CADASTRAR CLIENTE ===\n");
            cadastrar_cliente(head_cliente);

            // Após cadastrar, exibir a mensagem personalizada
            printf("\nCliente Identificado: %s!!! Prosseguir com a compra? (S/N) ", (*head_cliente)->nome); // Acessa o nome do cliente recém-cadastrado
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos); // Chama a função finalizarCompra
            } else {
                printf("Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorna ao menu principal
            }
        } else {
            // Lógica para coletar dados do cliente sem cadastro
            printf("=== INFORMAÇÕES DO CLIENTE ===\n");

            printf("Nome: ");
            fflush(stdin);
            fgets(cliente_temp.nome, 50, stdin);
            strtok(cliente_temp.nome, "\n");

            ler_cpf_formatado(cliente_temp.cpf);
            ler_telefone_formatado(cliente_temp.telefone);

            printf("\nCliente %s pronto para embarque!!! Prosseguir com a compra? (S/N) ", cliente_temp.nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos); // Chama a função finalizarCompra
            } else {
                printf("Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorna ao menu principal
            }
        }
    } else {
        printf("Resposta inválida! Pressione qualquer tecla para continuar.\n");
        _getch();
    }

    return 0; // Finaliza a função
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
void listarRotas(struct cliente **head_cliente) {
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
            detalhesDaCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada, assentos, head_cliente); // Passa head_cliente para detalhesDaCompra
        }
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }

    // Libera a memória alocada para as strings
    for (i = 0; i < MAX_DESTINOS; i++) {
        free((char *)opcoes[i]);
    }
}

// Função para filtrar rotas (usando a função genérica mostrarMenu)
void filtrarRotas(int filtro, struct cliente **head_cliente) {
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
            detalhesDaCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada, assentos, head_cliente); // Passa head_cliente para detalhesDaCompra
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
    struct cliente *head_cliente = NULL; // Inicializa a lista de clientes

    do {
        escolha = mostrarMenu("MENU DE ROTAS", opcoes_menu_principal, num_opcoes_menu_principal);

        switch (escolha) {
            case 0:
                listarRotas(&head_cliente); // Passa o endereço de head_cliente
                break;
            case 1:
                filtrarRotas(1, &head_cliente); // Passa o endereço de head_cliente
                break;
            case 2:
                filtrarRotas(2, &head_cliente); // Passa o endereço de head_cliente
                break;
            case 3:
                printf("Encerrando o programa. Até mais!\n");
                break;
        }
    } while (escolha != 3); // 3 é o índice da opção "Sair"

    // Liberação de memória dos clientes cadastrados
    struct cliente *temp;
    while (head_cliente != NULL) {
        temp = head_cliente;
        head_cliente = head_cliente->proximo_cl;
        free(temp);
    }

    return 0;
}
