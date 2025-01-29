#include "auxiliar.h"

// Variáveis globais
int i, j;

// Funções utilitárias
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Animações
void animarBarco() {
    const char *frames[] = {
        "\n\n"
        "              ~~~~~~            \n"
        "            ~~~~~~~~~~          \n"
        "          ~~~~~~~~~~~~         \n"
        "          __|____|_____\\____    \n"
        "         |                |      \n"
        "         |  BANZEIRO  |~~~   \n"
        "       ~~~~~~~~~~~~~~~~~~~~~~~~~ \n",
        "\n\n"
        "            ~~~~~~~~~~           \n"
        "          ~~~~~~~~~~~~~~~        \n"
        "         ~~~~~~~~~~~~~~~~~~~     \n"
        "        __|____|______/_____\\_   \n"
        "       |                |        \n"
        "       |  BANZEIRO  | ~~~~   \n"
        "       ~~~~~~~~~~~~~~~~~~~~~~~~~ \n",
        "\n\n"
        "          ~~~~~~~~~~~~~~~~       \n"
        "        ~~~~~~~~~~~~~~~~~~~~     \n"
        "       ~~~~~~~~~~~~~~~~~~~~~~~   \n"
        "       ____|___|_______/_____\\_  \n"
        "      |                |        \n"
        "       |  BANZEIRO  | ~~~~~  \n"
        "       ~~~~~~~~~~~~~~~~~~~~~~~~~ \n"
    };

    for (i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);
        SLEEP(300);
    }
}

void animacaoBarco() {
    const char *frames[] = {
        "\n\n"
        "          ~     ~ \n"
        "             ~    ~   \n"
        "           ~    ~    ~   ~   ~ \n"
        "               ~      \n",
        "          ~  \n"
        "             ~    ~ \n"
        "           ~    ~\n"
        "              \n",
        "         ~     ~    ~\n"
        "             ~    ~      ~    ~\n"
        "           ~    ~    ~   ~   ~   ~\n"
        "               ~       ~     ~\n",
    };

    for (i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);
        SLEEP(300);
    }
    printf("\nBarco pronto para embarque!\n");
    SLEEP(1500);
}

// Função para ler CPF formatado com validação
void ler_cpf_formatado(char *cpf) {
    int pos = 0;
    char c;
    printf("       CPF: ");
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

            if (pos == 14) {
                char cpf_limpo[12];
                remover_formatacao_cpf(cpf, cpf_limpo); // Remove a formatação do CPF

                // Valida o CPF
                if (!validar_cpf(cpf_limpo)) {
                    setColor(12);
                    printf("\n        CPF inválido! Digite novamente.\n");
                    setColor(15);
                    getch(); // Aguarda o usuário pressionar uma tecla

                    // Apaga as últimas 3 linhas
                    printf("\033[A\033[K\033[A\033[K");

                    pos = 0;
                    cpf[0] = '\0';
                    printf("       CPF: ");
                } 
            }
        }
    }
    cpf[pos] = '\0';
    printf("\n");
}

// Função para validar o CPF
int validar_cpf(const char *cpf) {
    int i, j, digito1 = 0, digito2 = 0;

    // Verifica se o CPF tem 11 dígitos
    if (strlen(cpf) != 11) {
        return 0; // CPF inválido
    }

    // Verifica se todos os dígitos são iguais
    if (todos_digitos_iguais(cpf)) {
        return 0; // CPF inválido
    }

    // Calcula o primeiro dígito verificador
    for (i = 0, j = 10; i < 9; i++, j--) {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 = (digito1 * 10) % 11;
    if (digito1 == 10) {
        digito1 = 0;
    }

    // Calcula o segundo dígito verificador
    for (i = 0, j = 11; i < 10; i++, j--) {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 = (digito2 * 10) % 11;
    if (digito2 == 10) {
        digito2 = 0;
    }

    // Verifica se os dígitos verificadores calculados são iguais aos dígitos do CPF
    if (digito1 == cpf[9] - '0' && digito2 == cpf[10] - '0') {
        return 1; // CPF válido
    } else {
        return 0; // CPF inválido
    }
}

// Função auxiliar para verificar se todos os dígitos do CPF são iguais
int todos_digitos_iguais(const char *cpf) {
    char primeiro_digito = cpf[0];
    for (int i = 1; i < 11; i++) {
        if (cpf[i] != primeiro_digito) {
            return 0; // Nem todos os dígitos são iguais
        }
    }
    return 1; // Todos os dígitos são iguais
}

void remover_formatacao_cpf(const char *cpf_formatado, char *cpf_limpo) {
    int j = 0;
    for (i = 0; cpf_formatado[i] != '\0'; i++) {
        if (isdigit(cpf_formatado[i])) {
            cpf_limpo[j++] = cpf_formatado[i];
        }
    }
    cpf_limpo[j] = '\0';
}

void ler_telefone_formatado(char *telefone) {
    int pos = 0;
    char c;
    printf("       Telefone: ");
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
            } else if (pos == 10) {
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

void ler_conta_bancaria_formatada(char *conta) {
    int pos = 0;
    char c;
    printf("       Conta Bancária (xxxxxxxx-x): ");
    while (pos < 10) {
        c = getch();
        if (c == '\b' && pos > 0) {
            pos--;
            if (conta[pos] == '-') pos--;
            conta[pos] = '\0';
            printf("\b \b");
        } else if (isdigit(c)) {
            if (pos == 8) {
                conta[pos++] = '-';
                printf("-");
            } else {
                conta[pos++] = c;
                printf("%c", c);
            }
        }
    }
    conta[pos] = '\0';
    printf("\n");
}

int calcular_idade(const char *data_nasc) {
    int dia, mes, ano;
    sscanf(data_nasc, "%d/%d/%d", &dia, &mes, &ano);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int ano_atual = tm.tm_year + 1900;
    int mes_atual = tm.tm_mon + 1;
    int dia_atual = tm.tm_mday;

    int idade = ano_atual - ano;

    if (mes_atual < mes || (mes_atual == mes && dia_atual < dia)) {
        idade--;
    }

    return idade;
}

int validar_data(const char *data) {
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0; // Formato inválido
    }

    if (ano < 1900 || ano > 2100) {
        return 0; // Ano inválido
    }

    if (mes < 1 || mes > 12) {
        return 0; // Mês inválido
    }

    if (dia < 1 || dia > 31) {
        return 0; // Dia inválido
    }

    // Verificar dias específicos de cada mês (fevereiro, abril, junho, setembro e novembro)
    if ((mes == 2 && dia > 29) ||
        ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia == 31)) {
        return 0; // Dia inválido para o mês
    }

    return 1; // Data válida
}

void ler_data_formatada(char *data) {
    int pos = 0;
    char c;
    printf("       Data (dd/mm/aaaa): ");
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

