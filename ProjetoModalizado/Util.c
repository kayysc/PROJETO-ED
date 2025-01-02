#include "Util.h"

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#ifdef _WIN32
#define CLEAR "cls"
#define SLEEP(ms) Sleep(ms)
#else
#define CLEAR "clear"
#define SLEEP(ms) usleep(ms * 1000)
#endif

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void animarBarco(){
    const char *frames[]={
            "\n\n"
            "              ~~~~~~            \n"
            "            ~~~~~~~~~~          \n"
            "           ~~~~~~~~~~~~         \n"
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
            "       |                |        \n"
            "       |  BANZEIRO  | ~~~~~  \n"
            "       ~~~~~~~~~~~~~~~~~~~~~~~~~ \n"
    };
    int i;
    for (i=0;i<10;i++){
        system(CLEAR);
        printf ("%s",frames[i%3]);
        fflush(stdout);
        SLEEP(300);
    }
    printf("\nBarco pronto para embarque \n");
    SLEEP(1500);

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
            "          ~     ~    ~\n"
            "             ~    ~      ~    ~\n"
            "           ~    ~    ~   ~   ~   ~\n"
            "               ~      ~     ~\n",

    };

    int i;
    for (i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);  // Garante que o frame seja exibido
        SLEEP(300);  // Espera 300ms entre os frames
    }
    printf("\nBarco pronto para embarque!\n");
    SLEEP(1500);
}

void ler_cpf_formatado(char *cpf){
    int pos = 0;
    char c;
    printf ("       CPF:");
    while( pos < 14){
        c= getch();
        if (c == '\b' && pos > 0){
            pos--;
            if(cpf[pos] == '.' || cpf[pos] == '-') pos--;
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

void remover_formatacao_cpf(const char *cpf_formatado, char *cpf_limpo) {
    int j = 0;
    int i;
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

    int idade = tm.tm_year + 1900 - ano;

    if (tm.tm_mon + 1 < mes || (tm.tm_mon + 1 == mes && tm.tm_mday < dia)) {
        idade--;
    }

    return idade;
}
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

    printf("       Data de Nascimento (dd/mm/aaaa): ");
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

