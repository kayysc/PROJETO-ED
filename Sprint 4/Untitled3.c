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
#define SLEEP(ms) Sleep(ms)
#else
#define CLEAR "clear"
#define SLEEP(ms) usleep(ms * 1000)
#endif

#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3

int i, j;
int prox_id = 1;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
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

void remover_formatacao_cpf(const char *cpf_formatado, char *cpf_limpo) {
    int j = 0;
    for (i = 0; cpf_formatado[i] != '\0'; i++) {
        if (isdigit(cpf_formatado[i])) {
            cpf_limpo[j++] = cpf_formatado[i];
        }
    }
    cpf_limpo[j] = '\0';
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

// Função para validar e formatar número da conta bancária
void ler_conta_bancaria_formatada(char *conta) {
    int pos = 0;
    char c;
    printf("Conta Bancária (xxxxxxxx-x): ");
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

// Estrutura para armazenar dados do gerente
typedef struct {
    char nome[50];
    char usuario[20];
    char senha[20];
    int cod;
} Gerente;

// Estrutura para armazenar dados do funcionário (com todos os campos)
typedef struct {
    char nome[50];
    char cpf[15];
    char telefone[20];
    char email[50];
    char conta_bancaria[11];  // Formato: "12345678-9"
    char usuario[20];
    char senha[20];
    int cod;
} FuncionarioCadastro;

FuncionarioCadastro funcionarios[100];  // Array global para armazenar os funcionários
int num_funcionarios = 0;               // Variável global para o número

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

struct Nota {
    char nome[50];
    char cpf[15];
    int notaFiscal;
    char origem[30];
    char destino[30];
    char data[12];
    char horario[10];
    int preco;
    struct Nota *prox_Nota;
};

// Animação de barco navegando no terminal
void animarBarco() {
    const char *frames[] = {
    	"\n\n"
        "       ~~~~~~            \n"
        "     ~~~~~~~~~~          \n"
        "    ~~~~~~~~~~~~         \n"
        "   __|____|_____\\____    \n"
        "  |                |      \n"
        "  |  BANZEIRO  |~~~   \n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~ \n",
        "\n\n"
        "     ~~~~~~~~~~           \n"
        "   ~~~~~~~~~~~~~~~        \n"
        "  ~~~~~~~~~~~~~~~~~~~     \n"
        " __|____|______/_____\\_   \n"
        "|                |        \n"
        "|  BANZEIRO  | ~~~~   \n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~ \n",
        "\n\n"
        "   ~~~~~~~~~~~~~~~~       \n"
        " ~~~~~~~~~~~~~~~~~~~~     \n"
        "~~~~~~~~~~~~~~~~~~~~~~~   \n"
        "____|___|_______/_____\\_  \n"
        "|                |        \n"
        "|  BANZEIRO  | ~~~~~  \n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~ \n"
    };
    

    // Loop para exibir os frames em sequência, criando a animação
    int i;
	for ( i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);  // Garante que o frame seja exibido
        SLEEP(300);  // Espera 300ms entre os frames
    }
}

void animacaoBarco() {
	const char *frames[] = {
    "\n\n"
	"   ~     ~ \n"
    "      ~    ~   \n"
    "    ~    ~    ~   ~   ~ \n"
    "        ~      \n",
    "   ~  \n"
    "      ~    ~ \n"
    "    ~    ~\n"
    "       \n",
    "   ~     ~    ~\n"
    "      ~    ~      ~    ~\n"
    "    ~    ~    ~   ~   ~   ~\n"
    "        ~      ~     ~\n",
	
	};

	 int i;
	for ( i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);  // Garante que o frame seja exibido
        SLEEP(300);  // Espera 300ms entre os frames
	}
		printf("\nBarco pronto para embarque!\n");
		SLEEP(1500);
}

struct Nota *notasfiscais = NULL;

void geracaoNotaFiscal(Rota rota, int preco_total, const char *nome, const char *cpf) {
    system("cls");
	struct Nota *aux = (struct Nota *)malloc(sizeof(struct Nota));

    strcpy(aux->origem, rota.origem);
    strcpy(aux->destino, rota.destino);
    strcpy(aux->horario, rota.horario);
    strcpy(aux->data, rota.data);
    strcpy(aux->nome, nome);
    strcpy(aux->cpf, cpf);
    aux->preco = preco_total;
    aux->notaFiscal = rand() % 10000;

    aux->prox_Nota = notasfiscais;
    notasfiscais = aux;

    // Exibe todas as informações da nota fiscal
    printf("\n===== NOTA FISCAL =====\n");
    printf("Nota Fiscal ID: %d\n", aux->notaFiscal);
    printf("Nome: %s\n", aux->nome);
    printf("CPF: %s\n", aux->cpf);
    printf("Origem: %s\n", aux->origem);
    printf("Destino: %s\n", aux->destino);
    printf("Data: %s\n", aux->data);
    printf("Horário: %s\n", aux->horario);
    printf("Preço Total: R$ %.2f\n", (float)aux->preco);
    printf("=======================\n");
}

// Listagem de Nota fiscal, onde ele vai listar todas as notas, caso nem tenha, ele informa que não existe registro no programa
void ListagemNota() {
    struct Nota *buscar = notasfiscais;
    int i = 0;

    system(CLEAR);
    printf("===== NOTAS FISCAIS =====\n\n");

    while (buscar != NULL) {
        printf("ID da Nota Fiscal: %i\n", buscar->notaFiscal);
        printf("CPF do Cliente: %s\n", buscar->cpf);
        printf("Nome do Cliente: %s\n", buscar->nome);
        printf("Origem: %s\n", buscar->origem);
        printf("Destino: %s\n", buscar->destino);
        printf("Data: %s\n", buscar->data);
        printf("Horário: %s\n", buscar->horario);
        printf("Preço: R$%i\n", buscar->preco);
        printf("========================\n");

        buscar = buscar->prox_Nota;
        i++;
        Sleep(1000);
    }

    if (i == 0) {
        printf("Nenhuma nota fiscal foi cadastrada!!!\n");
        Sleep(2000);
    }

    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
}



