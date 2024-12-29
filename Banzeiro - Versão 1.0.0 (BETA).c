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
#define MAX_NOTAS 1

int i, j;
int prox_id = 1;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Animação de barco navegando no terminal
void animarBarco() {
    const char *frames[] = {
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
	for ( i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);  // Garante que o frame seja exibido
        SLEEP(300);  // Espera 300ms entre os frames
	}
		printf("\nBarco pronto para embarque!\n");
		SLEEP(1500);
}

// Função para validar e formatar CPF
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

// Função para validar e formatar número da conta bancária
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

// Estrutura para armazenar dados do gerente
typedef struct {
    char nome[50];
    char usuario[20];
    char senha[20];
    int cod;
} Gerente;

// Função para exibir o menu do gerente com navegação por teclado
int mostrarMenuGerente(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;

    do {
        system(CLEAR);
        printf("\n ============= %s =============\n\n", titulo); // Exibe o título do menu

        // Exibe as opções do menu com a seta indicando a opção atual
        for (i = 0; i < numOpcoes; i++) {
            if (posicao == i) {
                setColor(14); // Amarelo para a opção selecionada
                printf("    -> %s\n", opcoes[i]);
            } else {
                setColor(7); // Branco para as outras opções
                printf("       %s\n", opcoes[i]);
            }
        }
        setColor(7); // Reset para a cor padrão

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
            return posicao + 1; // Retorna a opção selecionada (1-based)
        }
    } while (tecla != 27); // ESC para sair do menu

    return 0; // Retorna 0 se o usuário pressionar ESC
}

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

// Função para salvar dados do gerente em um arquivo
void salvarGerente(const char *arquivo, Gerente *usuario) {
    FILE *fp = fopen(arquivo, "wb");
    if (fp) {
        fwrite(usuario, sizeof(Gerente), 1, fp);
        fclose(fp);
    }
}

// Função para carregar dados do gerente de um arquivo
int carregarGerente(const char *arquivo, Gerente *usuario) {
    FILE *fp = fopen(arquivo, "rb");
    if (fp) {
        fread(usuario, sizeof(Gerente), 1, fp);
        fclose(fp);
        return 1; // Sucesso
    }
    return 0; // Falha
}

// Função para salvar todos os funcionários em um único arquivo
void salvarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int num_funcionarios) {
    FILE *fp = fopen(arquivo, "wb");
    if (fp) {
        fwrite(&num_funcionarios, sizeof(int), 1, fp);
        fwrite(funcionarios, sizeof(FuncionarioCadastro), num_funcionarios, fp);
        fclose(fp);
    } else {
    	setColor(12);
        printf("\n       Erro ao salvar os dados dos funcionários.\n");
        setColor(15);
    }
}

int carregarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    FILE *fp = fopen(arquivo, "rb");
    if (fp) {
        fread(num_funcionarios, sizeof(int), 1, fp); // Lê o número de funcionários
        fread(funcionarios, sizeof(FuncionarioCadastro), *num_funcionarios, fp); // Lê o array de funcionários

        // Adiciona o caractere nulo ao final de cada string
        for (i = 0; i < *num_funcionarios; i++) {
            funcionarios[i].nome[strcspn(funcionarios[i].nome, "\n")] = '\0';
            funcionarios[i].cpf[strcspn(funcionarios[i].cpf, "\n")] = '\0';
            funcionarios[i].telefone[strcspn(funcionarios[i].telefone, "\n")] = '\0';
            funcionarios[i].email[strcspn(funcionarios[i].email, "\n")] = '\0';
            funcionarios[i].conta_bancaria[strcspn(funcionarios[i].conta_bancaria, "\n")] = '\0';
            funcionarios[i].usuario[strcspn(funcionarios[i].usuario, "\n")] = '\0';
            funcionarios[i].senha[strcspn(funcionarios[i].senha, "\n")] = '\0';
        }

        fclose(fp);
        return 1; // Sucesso
    }
    return 0; // Falha
}

// Função para cadastrar um novo funcionário
void cadastrarFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    if (*num_funcionarios >= 100) {
        printf("\n       Limite ");
        setColor(12);
		printf("máximo");
		setColor(15);
		printf(" de funcionários atingido!\n");
        return;
    }

    FuncionarioCadastro novo_func;
    
    printf("\n ============= CADASTRO FUNCIONÁRIO =============\n\n");
    printf("       Nome: ");
    fflush(stdin);
    fgets(novo_func.nome, 50, stdin);
    strtok(novo_func.nome, "\n");

    do {
        ler_cpf_formatado(novo_func.cpf);
    } while (strlen(novo_func.cpf) != 14);

    do {
        ler_telefone_formatado(novo_func.telefone);
    } while (strlen(novo_func.telefone) != 15);

    printf("       Email: ");
    fflush(stdin);
    fgets(novo_func.email, 50, stdin);
    strtok(novo_func.email, "\n");

    do {
        ler_conta_bancaria_formatada(novo_func.conta_bancaria);
    } while (strlen(novo_func.conta_bancaria) != 10);

    // Define o código do funcionário
    novo_func.cod = *num_funcionarios + 1;

    printf("       Usuário: ");
    fflush(stdin);
    fgets(novo_func.usuario, 20, stdin);
    strtok(novo_func.usuario, "\n");

    printf("       Senha: ");
    fflush(stdin);
    fgets(novo_func.senha, 20, stdin);
    strtok(novo_func.senha, "\n");

    funcionarios[*num_funcionarios] = novo_func;
    (*num_funcionarios)++;

    // Salva todos os funcionários após o cadastro
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, *num_funcionarios);

    printf("\n       Parabéns ");
    setColor(11);
	printf("%s", novo_func.nome);
	setColor(7);
	printf(", oficialmente contratado!!!\n");
    printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void listarFuncionarios(FuncionarioCadastro *funcionarios, int num_funcionarios) {
    system(CLEAR);

    // Carrega todos os funcionários do arquivo "funcionarios.dat"
    carregarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, &num_funcionarios);

    if (num_funcionarios == 0) {
        printf("\n       Nenhum funcionário cadastrado.\n");
    } else {
    	printf("\n ============= LISTA DE FUNCIONÁRIOS =============\n\n");
        for (i = 0; i < num_funcionarios; i++) {
            printf("       Nome: %s\n", funcionarios[i].nome);
            printf("       CPF: %s\n", funcionarios[i].cpf);
            printf("       Telefone: %s\n", funcionarios[i].telefone);
            printf("       Email: %s\n", funcionarios[i].email);
            printf("       Conta Bancária: %s\n", funcionarios[i].conta_bancaria);
            printf("       Usuário: ");
            setColor(9);
			printf("%s\n", funcionarios[i].usuario);
			setColor(15);
            printf("------------------------------------------------\n");
        }
    }
    printf("\nPressione qualquer tecla para voltar ao menu.\n");
    getch();
}

void alterarFuncionario(FuncionarioCadastro *funcionarios, int num_funcionarios) {
    char cpf_busca[15];
    int i, posicao = -1;

    system(CLEAR);
    printf("\n ============= ALTERAR FUNCIONÁRIO =============\n\n");
    printf("\n       Digite abaixo o CPF do funcionário que deseja alterar\n\n");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcionário pelo CPF
    for (i = 0; i < num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
    	setColor(12);
        printf("\n       Funcionário não encontrado.\n");
        setColor(15);
        printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Altera os dados do funcionário
    system("cls");
    printf("\n ============= ALTERAR FUNCIONÁRIO =============\n\n");
    printf("\n       Funcionário ");
    setColor(11);
	printf("%s", funcionarios[posicao].nome);
	setColor(15);
	printf(", foi encontrado!\n");
    printf("\n       Digite abaixo os novos dados do funcionário\n");

    printf("       Nome: ");
    fflush(stdin);
    fgets(funcionarios[posicao].nome, 50, stdin);
    strtok(funcionarios[posicao].nome, "\n");

    do {
        ler_telefone_formatado(funcionarios[posicao].telefone);
    } while (strlen(funcionarios[posicao].telefone) != 15);

    printf("       Email: ");
    fflush(stdin);
    fgets(funcionarios[posicao].email, 50, stdin);
    strtok(funcionarios[posicao].email, "\n");

    do {
        ler_conta_bancaria_formatada(funcionarios[posicao].conta_bancaria);
    } while (strlen(funcionarios[posicao].conta_bancaria) != 10);

    printf("       Usuário: ");
    fflush(stdin);
    fgets(funcionarios[posicao].usuario, 20, stdin);
    strtok(funcionarios[posicao].usuario, "\n");

    printf("       Senha: ");
    fflush(stdin);
    fgets(funcionarios[posicao].senha, 20, stdin);
    strtok(funcionarios[posicao].senha, "\n");

    // Salva todos os funcionários após a alteração
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);

    printf("\n       Dados do funcionário alterados com ");
    setColor(10);
	printf("sucesso");
	setColor(15);
	printf("!\n");
    printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void excluirFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    char cpf_busca[15];
    int i, posicao = -1;

    system(CLEAR);
    printf("\n ============= EXCLUIR FUNCIONÁRIO =============\n\n");
    printf("\n       Digite abaixo o CPF do funcionário que deseja excluir\n\n");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcionário pelo CPF
    for (i = 0; i < *num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
    	setColor(12);
        printf("\n       Funcionário não encontrado.\n");
        setColor(15);
        printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Confirmação da exclusão
    system("cls");
    printf("\n ============= EXCLUIR FUNCIONÁRIO =============\n\n");
    printf("\n       Tem certeza que deseja excluir o funcionário ");
    setColor(11);
	printf("%s", funcionarios[posicao].nome);
	setColor(15);
	printf("? (S/N): ");
    char confirmacao = getch();

    if (confirmacao == 'S' || confirmacao == 's') {
        // Remove o funcionário do array
        char nome_arquivo[50];
        sprintf(nome_arquivo, "%s.dat", funcionarios[posicao].usuario);

        remove(nome_arquivo);

        for (i = posicao; i < *num_funcionarios - 1; i++) {
            funcionarios[i] = funcionarios[i + 1];
        }
        (*num_funcionarios)--;

        // Salva todos os funcionários após a exclusão
        salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, *num_funcionarios);

        printf("\n\n       Funcionário excluído com ");
		setColor(10);
		printf("sucesso");
		setColor(15);
		printf("!\n");
    } else {
        printf("\n\n       Exclusão cancelada!\n");
    }

    printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void login();

// Menu do Gerente
void menuGerente() {
    int escolha;

    const char *opcoes_menu_gerente[] = {
        "Cadastro de funcionário",
        "Acessar funcionário",
        "Leis do consumidor",
        "Sair"
    };
    int num_opcoes_menu_gerente = sizeof(opcoes_menu_gerente) / sizeof(opcoes_menu_gerente[0]);

    do {
        escolha = mostrarMenuGerente("MENU GERENTE", opcoes_menu_gerente, num_opcoes_menu_gerente);

        switch (escolha) {
            case 1:
                system(CLEAR);
                cadastrarFuncionario(funcionarios, &num_funcionarios);
                break;
            case 2: // Acessar Funcionário
                {
                    int escolha_submenu;
                    const char *opcoes_submenu_acessar_funcionario[] = {
                        "Alterar dados do funcionário",
                        "Excluir funcionário",
                        "Listar funcionários",
                        "Voltar"
                    };
                    int num_opcoes_submenu = sizeof(opcoes_submenu_acessar_funcionario) / sizeof(opcoes_submenu_acessar_funcionario[0]);

                    do {
                        escolha_submenu = mostrarMenuGerente("ACESSAR FUNCIONÁRIO", opcoes_submenu_acessar_funcionario, num_opcoes_submenu);

                        switch (escolha_submenu) {
                            case 1:
                                system(CLEAR);
                                alterarFuncionario(funcionarios, num_funcionarios);
                                break;
                            case 2:
                                system(CLEAR);
                                excluirFuncionario(funcionarios, &num_funcionarios);
                                break;
                            case 3: // Listar funcionários
                                system(CLEAR);
                                listarFuncionarios(funcionarios, num_funcionarios);
                                break;
                            case 4: // Voltar ao menu principal
                                break;
                            default:
                                printf("Opção inválida!\n");
                        }
                    } while (escolha_submenu != 4);
                }
                break;
            case 3:
                system(CLEAR);
                printf("\n ============= LEIS DO CONSUMIDOR =============\n\n");
                setColor(15);
                
                
                printf("\n        Legislação Aplicável aos Idosos\n\n"
 					   "        1. Lei nº ");
				setColor(14);
				printf("10.741/2003");
				setColor(15);
				printf(" (Estatuto do Idoso)\n\n"
                       "        - Estabelece direitos para pessoas com 60 anos ou mais, com foco na garantia de\n"
                       "        acesso prioritário e tratamento adequado nos serviços públicos e privados, incluindo\n"
                       "        transporte.\n\n"
                	   "        2. Lei nº ");
				setColor(14);	   
				printf("10.048/2000");
				setColor(15);	   
				printf(" (Lei da Prioridade)\n\n"
                       "        - Garante a prioridade para idosos, gestantes, lactantes, pessoas com deficiência e\n"
                       "        pessoas acompanhadas por crianças de colo no uso de serviços públicos e\n"
                       "        privados, incluindo o transporte fluvial.\n\n");
                
                printf("\n\n        Legislação Aplicável aos Estudantes\n\n"
 					   "        1. Lei nº ");
				setColor(14);
				printf("12.852/2013");
				setColor(15);
				printf(" (Estatuto da Juventude)\n\n"
                       "        - Garante direitos aos jovens de 15 a 29 anos, incluindo meia-passagem ou\n"
                       "        descontos no transporte coletivo, quando aplicável, e acesso preferencial a serviços\n"
                       "        de educação, cultura, saúde e transporte.\n\n"
                       "        2. Lei nº ");
				setColor(14);	   
				printf("12.933/2013");
				setColor(15);	   
				printf(" (Lei da Meia-Entrada)\n\n"
                       "        - Estende o direito à meia-entrada para transporte público e eventos culturais,\n"
                       "        mediante a apresentação da Carteira de Identificação Estudantil (CIE) emitida por\n"
                       "        entidades estudantis reconhecidas.\n");
                printf("\n\nPressione qualquer tecla para voltar ao menu.\n");
                getch();
                break;
            case 4:
            	system("cls");
            	printf("\n ============= MENU GERENTE =============\n\n");
                printf("       Saindo do menu do gerente...\n");
                Sleep(3200);
                login();
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (escolha != 4);
}

struct cliente {
    int id;
    char nome[50];
    char cpf[15];
    char data_nasc[11];
    char telefone[20];
    struct cliente *proximo_cl; // Lista encadeada
};

// Cadastro de cliente
void cadastrar_cliente(struct cliente **head) {
    struct cliente *novo_cl = (struct cliente *)malloc(sizeof(struct cliente));
    if (!novo_cl) {
    	setColor(12);
        printf("\n       Erro ao alocar memória!\n");
        setColor(15);
        return;
    }
    novo_cl->id = prox_id++;

    printf("       Nome: ");
    fflush(stdin);
    fgets(novo_cl->nome, 50, stdin);
    strtok(novo_cl->nome, "\n");

    do {
        ler_cpf_formatado(novo_cl->cpf);
    } while (strlen(novo_cl->cpf) != 14);

    do {
        ler_data_formatada(novo_cl->data_nasc);
        if (!validar_data(novo_cl->data_nasc)) {
            printf("\n       ");
            setColor(12);
			printf("Data inválida");
			setColor(15);
			printf("! Tente novamente.\n");
        }
    } while (!validar_data(novo_cl->data_nasc));

    do {
        ler_telefone_formatado(novo_cl->telefone);
    } while (strlen(novo_cl->telefone) != 15);

    novo_cl->proximo_cl = *head;
    *head = novo_cl;

    printf("\n       Cliente cadastrado com ");
    setColor(10);
	printf("sucesso");
	setColor(15);
	printf("!\n");
    getch();
}

// Buscar cliente por CPF
void buscar_cliente_por_cpf(struct cliente *head, const char *cpf) {
    struct cliente *atual = head;
    char cpf_limpo[12], cpf_atual_limpo[12];

    remover_formatacao_cpf(cpf, cpf_limpo);

    while (atual != NULL) {
        remover_formatacao_cpf(atual->cpf, cpf_atual_limpo);

        if (strcmp(cpf_atual_limpo, cpf_limpo) == 0) {
            printf("\n       Cliente encontrado:\n");
            printf("       ID: ");
            setColor(9);
			printf("%d\n", atual->id);
			setColor(15);
            printf("       Nome: %s\n", atual->nome);
            printf("       CPF: %s\n", atual->cpf);
            printf("       Data de Nascimento: %s\n", atual->data_nasc);
            printf("       Idade: %d anos\n", calcular_idade(atual->data_nasc));
            printf("       Telefone: %s\n", atual->telefone);
            return;
        }
        atual = atual->proximo_cl;
    }
}

// Função para buscar cliente (comum)
struct cliente* buscarCliente(struct cliente *head, const char *cpf, int exibir_informacoes) {
    struct cliente *atual = head;
    char cpf_limpo[12], cpf_atual_limpo[12];

    remover_formatacao_cpf(cpf, cpf_limpo);

    while (atual != NULL) {
        remover_formatacao_cpf(atual->cpf, cpf_atual_limpo);

        if (strcmp(cpf_atual_limpo, cpf_limpo) == 0) {
            if (exibir_informacoes) {
                printf("\n       Cliente encontrado:\n");
                printf("       ID: ");
            	setColor(9);
				printf("%d\n", atual->id);
				setColor(15);
                printf("       Nome: %s\n", atual->nome);
                printf("       CPF: %s\n", atual->cpf);
                printf("       Data de Nascimento: %s\n", atual->data_nasc);
                printf("       Idade: %d anos\n", calcular_idade(atual->data_nasc));
                printf("       Telefone: %s\n", atual->telefone);
            }
            return atual; // Retorna o ponteiro para o cliente encontrado
        }
        atual = atual->proximo_cl;
    }
    return NULL; // Retorna NULL se o cliente não for encontrado
}

// Buscar cliente por CPF (sem exibir informações)
struct cliente* buscarCliente_por_cpf_sem_exibir(struct cliente *head, const char *cpf) {
    return buscarCliente(head, cpf, 0); // Chama buscarCliente com exibir_informacoes = 0
}

// Buscar cliente por CPF (exibindo informações)
struct cliente* buscarCliente_por_cpf_com_exibir(struct cliente *head, const char *cpf) {
    return buscarCliente(head, cpf, 1); // Chama buscarCliente com exibir_informacoes = 1
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

// Seleção de assentos com navegação
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
	printf("\n ============= NOTAS FISCAIS =============\n\n");
    printf("       Nota Fiscal ID: %d\n", aux->notaFiscal);
    printf("       Nome: %s\n", aux->nome);
    printf("       CPF: %s\n", aux->cpf);
    printf("       Origem: %s\n", aux->origem);
    printf("       Destino: %s\n", aux->destino);
    printf("       Data: ");
    setColor(9);
	printf("%s\n", aux->data);
	setColor(15);
    printf("       Horário: ");
    setColor(9);
	printf("%s\n", aux->horario);
	setColor(15);
    printf("       Preço Total: ");
    setColor(10);
	printf("R$ %.2f\n", (float)aux->preco);
	setColor(15);
    printf("\n ==========================================\n");
}

// Listagem de Nota fiscal, onde ele vai listar todas as notas, caso nem tenha, ele informa que não existe registro no programa
void ListagemNota() {
    struct Nota *buscar = notasfiscais;
    int i = 0;

    system(CLEAR);
    printf("\n ============= NOTAS FISCAIS =============\n\n");

    while (buscar != NULL) {
        printf("       ID da Nota Fiscal: %i\n", buscar->notaFiscal);
        printf("       CPF do Cliente: %s\n", buscar->cpf);
        printf("       Nome do Cliente: %s\n", buscar->nome);
        printf("       Origem: %s\n", buscar->origem);
        printf("       Destino: %s\n", buscar->destino);
        printf("       Data: ");
	    setColor(9);
		printf("%s\n", buscar->data);
		setColor(15);
    	printf("       Horário: ");
	    setColor(9);
		printf("%s\n", buscar->horario);
		setColor(15);
    	printf("       Preço Total: ");
    	setColor(10);
		printf("R$ %i\n", buscar->preco);
		setColor(15);
        printf("\n ==========================================\n");

        buscar = buscar->prox_Nota;
        i++;
        Sleep(1000);
    }

    if (i == 0) {
        printf("       Nenhuma nota fiscal foi cadastrada!!!\n");
        Sleep(2000);
    }

    printf("\nPressione qualquer tecla para voltar ao menu...\n");
    getch();
}

// Função genérica para exibir menus com navegação por teclado
int mostrarMenu(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;

    do {
        system(CLEAR);
        printf("\n ============= %s =============\n\n", titulo); // Exibe o título do menu

        // Exibe as opções do menu com a seta indicando a opção atual
        for (i = 0; i < numOpcoes; i++) {
            setColor(posicao == i ? 14 : 7); // Amarelo para a opção selecionada
            printf("    -> %s\n", opcoes[i]);
            setColor(15); // Reset para a cor padrão
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

void finalizarCompra(int totalAssentos, Rota rota, char assentosEscolhidos[MAX_PASSAGENS][5], const char *nome, const char *cpf) {
    char opcao;
    float preco_total;

    while (1) { // Loop while para controle de fluxo
        system(CLEAR);
        printf("\n ============= FINALIZAR COMPRA =============\n\n");
        printf("       Inteira ou Meia? (I/M): ");
        scanf(" %c", &opcao);
        if (opcao == 'I' || opcao == 'i') {
            preco_total = totalAssentos * atof(rota.preco + 2);
        } else if (opcao == 'M' || opcao == 'm') {
            printf("       Necessário comprovação de benefício\n");
            preco_total = totalAssentos * (atof(rota.preco + 2) / 2); 
        } else {
            printf("       Opção inválida! Pressione qualquer tecla para continuar.\n");
            _getch();
            return; // Retorna à função detalhesDaCompra
        }

        system(CLEAR);
        printf("\n ============= FINALIZAR COMPRA =============\n\n");
        printf("       Rota: %s -> %s\n", rota.origem, rota.destino);
        printf("       Data: ");
    	setColor(9);
		printf("%s\n", rota.data);
		setColor(15);
    	printf("       Horário: ");
    	setColor(9);
		printf("%s\n", rota.horario);
		setColor(15);
        printf("       Assentos Selecionados:\n\n");
        for (i = 0; i < totalAssentos; i++) {
        	setColor(13);
            printf("       - %s\n", assentosEscolhidos[i]);
        }
        setColor(15);
        printf("\n       Preço Total: ");
    	setColor(10);
		printf("R$ %.2f\n", preco_total);
		setColor(15);
        printf("\n ==========================================\n\n");
        printf("       Confirma compra? (S/N): ");
        scanf(" %c", &opcao);

        if (opcao == 'S' || opcao == 's') {
            geracaoNotaFiscal(rota, preco_total, nome, cpf);
            printf("\n       Compra confirmada! Pressione qualquer tecla para continuar.\n");
            _getch();
            break; // Sai do loop após a confirmação da compra
        } else {
            printf("\n       Compra cancelada! Voltando ao Menu Principal...\n");
            _getch();
        }
    } 
}

int detalhesDaCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota, char ***assentos, struct cliente **head_cliente) {
    char resposta;
    
    system(CLEAR);
    printf("\n ============= DETALHES DA COMPRA =============\n\n");
    printf("       Rota: %s -> %s\n", rota.origem, rota.destino);
    printf("       Data: %s\n", rota.data);
    printf("       Horário: %s\n", rota.horario);
    printf("\n       Assentos Selecionados:\n\n");
    for (i = 0; i < totalAssentos; i++) {
    	setColor(13);
        printf("       - %s\n", assentosEscolhidos[i]);
    }
    setColor(15);
    printf("\n ===============================================\n\n");

    printf("       Cliente possui cadastro? (S/N): ");
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        char cpf_busca[15]; // Variável cpf_busca declarada dentro do bloco condicional
        system(CLEAR);
        printf("\n ============= BUSCAR CLIENTE POR CPF =============\n\n");
        printf("       Digite abaixo o CPF do cliente");
        printf("\n");
        ler_cpf_formatado(cpf_busca);
        
        struct cliente *cliente_encontrado = buscarCliente_por_cpf_sem_exibir(*head_cliente, cpf_busca);
        
        if (cliente_encontrado != NULL) {
            printf("\n       Cliente Identificado: %s. Prosseguir com a compra? (S/N) ", cliente_encontrado->nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos, cliente_encontrado->nome, cliente_encontrado->cpf);
            } else {
                printf("\n       Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorno 0 para indicar que a função terminou
            }
        } else {
            printf("\n       Cliente não encontrado.\n\nPressione qualquer tecla para continuar...\n");
            _getch(); 
        }
    } else if (resposta == 'N' || resposta == 'n') {
        printf("       Cliente deseja realizar cadastro? (S/N): ");
        scanf(" %c", &resposta);
        if (resposta == 'S' || resposta == 's') {
            system(CLEAR);
            printf("\n ============= CADASTRAR CLIENTE =============\n\n");
            cadastrar_cliente(head_cliente);
            printf("\n       Cliente Identificado: %s!!! Prosseguir com a compra? (S/N) ", (*head_cliente)->nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos, (*head_cliente)->nome, (*head_cliente)->cpf);
            } else {
                printf("\n       Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorno 0 para indicar que a função terminou
            }
        } else {
        	system("cls");
            struct cliente cliente_temp; // Variável cliente_temp declarada dentro do bloco condicional
            printf("\n ============= INFORMAÇÕES DO CLIENTE =============\n\n");
            printf("       Nome: ");
            fflush(stdin);
            fgets(cliente_temp.nome, 50, stdin);
            strtok(cliente_temp.nome, "\n");

            ler_cpf_formatado(cliente_temp.cpf);
            ler_telefone_formatado(cliente_temp.telefone);
            printf("\n       Cliente %s pronto para embarque!!! Prosseguir com a compra? (S/N) ", cliente_temp.nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos, cliente_temp.nome, cliente_temp.cpf);
            } else {
                printf("\n       Compra cancelada! Voltando ao Menu Principal...\n");
                _getch();
                return 0; // Retorno 0 para indicar que a função terminou
            }
        }
    } else {
        printf("       Resposta inválida! Pressione qualquer tecla para continuar.\n");
        _getch();
    }
    return 0; // Finaliza a função
}

// Função para salvar o estado dos assentos em um arquivo
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

// Função para carregar o estado dos assentos de um arquivo
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

void cancelarCompra() {
    char opcao;
    char cpf_formatado[15], cpf_limpo[12];
    int id, encontrado = 0;

    do {
        system(CLEAR);
        printf("\n ============= CANCELAMENTO DE COMPRA =============\n");
        printf("       [1] Cancelar por CPF\n");
        printf("       [2] Cancelar por ID da Nota Fiscal\n");
        printf("       [0] Voltar ao menu\n");
        printf("\n       Escolha uma opção: ");
        opcao = getchar();
        getchar(); // Captura o '\n' adicional após o getchar()

        switch (opcao) {
            case '1': // Cancelar por CPF
                system(CLEAR);
                printf("\n       Digite o CPF do cliente:\n");
                ler_cpf_formatado(cpf_formatado);
                remover_formatacao_cpf(cpf_formatado, cpf_limpo);

                struct Nota *atual = notasfiscais, *anterior = NULL;
                encontrado = 0;

                while (atual != NULL) {
                    char cpf_atual_limpo[12];
                    remover_formatacao_cpf(atual->cpf, cpf_atual_limpo);

                    if (strcmp(cpf_limpo, cpf_atual_limpo) == 0) {
                        encontrado = 1;

                        // Exibe as informações da nota fiscal
                        printf("\n       Nota Fiscal Encontrada:\n");
                        printf("       ID: %d\n", atual->notaFiscal);
                        printf("       Nome: %s\n", atual->nome);
                        printf("       CPF: %s\n", atual->cpf);
                        printf("       Origem: %s\n", atual->origem);
                        printf("       Destino: %s\n", atual->destino);
                        printf("       Data: %s\n", atual->data);
                        printf("       Horário: %s\n", atual->horario);
                        printf("       Valor: R$ %.2f\n", (float)atual->preco);

                        printf("\n       Deseja cancelar esta compra? (S/N): ");
                        char confirmacao = getchar();
                        getchar(); // Captura o '\n' adicional
                        if (confirmacao == 's' || confirmacao == 'S') {
                            if (anterior == NULL) {
                                notasfiscais = atual->prox_Nota;
                            } else {
                                anterior->prox_Nota = atual->prox_Nota;
                            }
                            free(atual);
                            printf("\n       Compra cancelada com sucesso!\n");
                            printf("       Pressione qualquer tecla para voltar ao menu...");
                            getchar();
                            return;
                        } else {
                            printf("\n       Cancelamento abortado.\n");
                            printf("       Pressione qualquer tecla para voltar ao menu...");
                            getchar();
                            return;
                        }
                    }

                    anterior = atual;
                    atual = atual->prox_Nota;
                }

                if (!encontrado) {
                    printf("\n       Nenhuma nota fiscal encontrada para o CPF informado.\n");
                    printf("       Pressione qualquer tecla para voltar ao menu...");
                    getchar();
                }
                break;

            case '2': // Cancelar por ID
                system(CLEAR);
                printf("\n       Digite o ID da Nota Fiscal: ");
                scanf("%d", &id);
                getchar(); // Captura o '\n' adicional

                struct Nota *atual_id = notasfiscais, *anterior_id = NULL;
                encontrado = 0;

                while (atual_id != NULL) {
                    if (atual_id->notaFiscal == id) {
                        encontrado = 1;

                        // Exibe as informações da nota fiscal
                        printf("\n       Nota Fiscal Encontrada:\n");
                        printf("       ID: %d\n", atual_id->notaFiscal);
                        printf("       Nome: %s\n", atual_id->nome);
                        printf("       CPF: %s\n", atual_id->cpf);
                        printf("       Origem: %s\n", atual_id->origem);
                        printf("       Destino: %s\n", atual_id->destino);
                        printf("       Data: %s\n", atual_id->data);
                        printf("       Horário: %s\n", atual_id->horario);
                        printf("       Valor: R$ %.2f\n", (float)atual_id->preco);

                        printf("\n       Deseja cancelar esta compra? (S/N): ");
                        char confirmacao = getchar();
                        getchar(); // Captura o '\n' adicional
                        if (confirmacao == 's' || confirmacao == 'S') {
                            if (anterior_id == NULL) {
                                notasfiscais = atual_id->prox_Nota;
                            } else {
                                anterior_id->prox_Nota = atual_id->prox_Nota;
                            }
                            free(atual_id);
                            printf("\n       Compra cancelada com sucesso!\n");
                            printf("       Pressione qualquer tecla para voltar ao menu...");
                            getchar();
                            return;
                        } else {
                            printf("\n       Cancelamento abortado.\n");
                            printf("       Pressione qualquer tecla para voltar ao menu...");
                            getchar();
                            return;
                        }
                    }

                    anterior_id = atual_id;
                    atual_id = atual_id->prox_Nota;
                }

                if (!encontrado) {
                    printf("\n       Nenhuma nota fiscal encontrada para o ID informado.\n");
                    printf("       Pressione qualquer tecla para voltar ao menu...");
                    getchar();
                }
                break;

            case '0': // Voltar ao menu
                return;

            default:
                printf("\n       Opção inválida! Pressione qualquer tecla para tentar novamente...");
                getchar();
        }
    } while (opcao != '0');
}


// Menu da Área do Cliente
void menuCliente(struct cliente **head_cliente) { // Recebe o endereço do ponteiro head_cliente
    const char *opcoes[] = {
        "Cadastrar cliente",
        "Buscar cliente por CPF",
        "Cancelamento de Compra",
        "Voltar"
    };
    int escolha;
    char cpf_busca[15];

    do {
        escolha = mostrarMenu("ÁREA DO CLIENTE", opcoes, 4);
        switch (escolha) {
            case 0:
                system(CLEAR);
                printf("\n ============= CADASTRAR CLIENTE =============\n\n");
                cadastrar_cliente(head_cliente); // Passa o endereço de head_cliente
                break;
            case 1: {
                system(CLEAR);
                printf("\n ============= BUSCAR CLIENTE POR CPF =============\n\n");
                printf("       Digite o CPF do cliente");
                printf("\n");
                ler_cpf_formatado(cpf_busca);
                buscarCliente_por_cpf_com_exibir(*head_cliente, cpf_busca); // Chama a função com o ponteiro head_cliente
                printf("\n       Pressione qualquer tecla para continuar...");
                _getch();
                break;
            }
            case 2:
               system(CLEAR);
               cancelarCompra();
               break;
            case 3:
            	printf("Voltando ao menu anterior...");
            	return;
        }
    } while (1);
}

void login();

void menuFuncionario(){
    const char *opcoes_menu_principal[] = {
        "Listar notas fiscais",
		"Área do cliente",
		"Rotas",
		"Leis do consumidor",
        "Sair"
    };
    int num_opcoes_menu_principal = sizeof(opcoes_menu_principal) / sizeof(opcoes_menu_principal[0]);

    int escolha;
    struct cliente *head_cliente = NULL; // Inicializa a lista de clientes

    do {
        escolha = mostrarMenu("MENU FUNCIONÁRIO", opcoes_menu_principal, num_opcoes_menu_principal);

        switch (escolha) {
            case 0:
                ListagemNota(); //Listagem de NotaFiscal
                break;
            case 1:
                menuCliente(&head_cliente);
                break;
            case 2:
                listarRotas(&head_cliente);
                break;
            case 3:
                system(CLEAR);
                printf("\n ============= LEIS DO CONSUMIDOR =============\n\n");
                setColor(15);
                
                
                printf("\n        Legislação Aplicável aos Idosos\n\n"
 					   "        1. Lei nº ");
				setColor(14);
				printf("10.741/2003");
				setColor(15);
				printf(" (Estatuto do Idoso)\n\n"
                       "        - Estabelece direitos para pessoas com 60 anos ou mais, com foco na garantia de\n"
                       "        acesso prioritário e tratamento adequado nos serviços públicos e privados, incluindo\n"
                       "        transporte.\n\n"
                	   "        2. Lei nº ");
				setColor(14);	   
				printf("10.048/2000");
				setColor(15);	   
				printf(" (Lei da Prioridade)\n\n"
                       "        - Garante a prioridade para idosos, gestantes, lactantes, pessoas com deficiência e\n"
                       "        pessoas acompanhadas por crianças de colo no uso de serviços públicos e\n"
                       "        privados, incluindo o transporte fluvial.\n\n");
                
                printf("\n\n        Legislação Aplicável aos Estudantes\n\n"
 					   "        1. Lei nº ");
				setColor(14);
				printf("12.852/2013");
				setColor(15);
				printf(" (Estatuto da Juventude)\n\n"
                       "        - Garante direitos aos jovens de 15 a 29 anos, incluindo meia-passagem ou\n"
                       "        descontos no transporte coletivo, quando aplicável, e acesso preferencial a serviços\n"
                       "        de educação, cultura, saúde e transporte.\n\n"
                       "        2. Lei nº ");
				setColor(14);	   
				printf("12.933/2013");
				setColor(15);	   
				printf(" (Lei da Meia-Entrada)\n\n"
                       "        - Estende o direito à meia-entrada para transporte público e eventos culturais,\n"
                       "        mediante a apresentação da Carteira de Identificação Estudantil (CIE) emitida por\n"
                       "        entidades estudantis reconhecidas.\n");
                printf("\n\nPressione qualquer tecla para voltar ao menu.\n");
                getch();
                break;
            case 4:
            	system("cls");
            	printf("\n ============= MENU FUNCIONÁRIO =============\n\n");
                printf("       Saindo do menu do funcionário...\n");
                Sleep(3200);
                login();
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
}

void login() {
    setlocale(LC_ALL, "Portuguese");
    Gerente gerente;
    FuncionarioCadastro funcionario;
    char login[20], senha[20];
    int i = 0;
    int opcao;
    int posicao = 0;
    int tecla;

    do {
        system(CLEAR);
        printf("\n ============= BEM VINDO AO BANZEIRO!!! =============\n\n");

        if (posicao == 0) {
            setColor(14);
            printf("    -> Entrar\n");
        } else {
            setColor(15);
            printf("       Entrar\n");
        }
        if (posicao == 1) {
            setColor(12);
            printf("    -> Sair\n");
        } else {
            setColor(15);
            printf("       Sair\n");
        }
        setColor(15);

        tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            switch (tecla) {
                case 72:
                    if (posicao > 0) posicao--;
                    break;
                case 80:
                    if (posicao < 1) posicao++;
                    break;
            }
        } else if (tecla == 13) {
            opcao = posicao + 1;
        }

        if (opcao == 2) {
        	system("cls");
        	printf("\n =============== SAINDO DO SISTEMA... ===============\n\n");
        	Sleep(2200);
        	printf("\n       Programa encerrado\n\n");
            exit(0);
        }

        if (opcao == 1) {
        	system(CLEAR);
        	printf("\n ============= BEM VINDO AO BANZEIRO!!! =============\n\n");
            printf("       Usuário: ");
            scanf("%s", login);
            printf("       Senha: ");

            i = 0;
            while (1) {
                char ch = getch();
                if (ch == 13) {
                    senha[i] = '\0';
                    break;
                } else if (ch == 8) {
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                    }
                } else {
                    senha[i++] = ch;
                    printf("*");
                }
            }
            printf("\n");

            // Tenta carregar o usuário como gerente
            if (carregarGerente(login, &gerente)) {
                if (strcmp(gerente.senha, senha) == 0) {
                    system(CLEAR);
                    printf("\n ============= BEM VINDO(A), ");
                    setColor(11);
					printf("%s", gerente.nome);
					setColor(15);
					printf("! =============\n\n");
                    printf("       Aguarde um momento...\n\n");
                    Sleep(4200);
                    animarBarco();
                    system("cls");
                    menuGerente();
                    return;
                } else {
                	setColor(12);
                    printf("\n       Senha incorreta!\n");
                    setColor(15);
                    Sleep(1400);
                    system("cls");
                }
            } else {
                // Se não for gerente, verifica se é funcionário
                for (i = 0; i < num_funcionarios; i++) {
                    if (strcmp(funcionarios[i].usuario, login) == 0 && 
                        strcmp(funcionarios[i].senha, senha) == 0) {
                        system(CLEAR);
                        printf("\n ============= BEM VINDO(A), ");
                        setColor(11);
						printf("%s", funcionarios[i].nome);
						setColor(15);
						printf("! =============\n\n");
                        printf("       Aguarde um momento...\n\n");
                        Sleep(4200);
                        animarBarco();
                        system("cls");
                        menuFuncionario(); // Implementação futura do menu do funcionário
                        return;
                    }
                }

                // Se o loop terminar sem encontrar o funcionário
                setColor(12);
                printf("\n       Usuário não encontrado ou senha incorreta!\n");
                Sleep(1400);
                setColor(15);
                system("cls");
            }
        }
    } while (opcao != 2);
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    Gerente gerente = {"Ana", "admin", "1234", 01};
    salvarGerente("admin", &gerente);

    // Carrega todos os funcionários do arquivo "funcionarios.dat"
    if (!carregarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, &num_funcionarios)) {
        printf("\n       Nenhum funcionário encontrado. Inicializando com lista vazia.\n");
    }

    printf("\n       Número de funcionários carregados: %d\n", num_funcionarios);

    login();

    // Salva todos os funcionários em "funcionarios.dat" antes de sair
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);

    return 0;
}
