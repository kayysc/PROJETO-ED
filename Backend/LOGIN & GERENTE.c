#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR "cls"
#define SLEEP(ms) Sleep(ms)
#else
#define CLEAR "clear"
#define SLEEP(ms) usleep(ms * 1000)
#endif

int i, j;

// Estrutura para armazenar dados do gerente
typedef struct {
    char nome[50];
    char usuario[20];
    char senha[20];
    int cod;
} Gerente;

// Estrutura para armazenar dados do funcion�rio (com todos os campos)
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

FuncionarioCadastro funcionarios[100];  // Array global para armazenar os funcion�rios
int num_funcionarios = 0;               // Vari�vel global para o n�mero

// Fun��o para salvar dados do gerente em um arquivo
void salvarGerente(const char *arquivo, Gerente *usuario) {
    FILE *fp = fopen(arquivo, "wb");
    if (fp) {
        fwrite(usuario, sizeof(Gerente), 1, fp);
        fclose(fp);
    }
}

// Fun��o para carregar dados do gerente de um arquivo
int carregarGerente(const char *arquivo, Gerente *usuario) {
    FILE *fp = fopen(arquivo, "rb");
    if (fp) {
        fread(usuario, sizeof(Gerente), 1, fp);
        fclose(fp);
        return 1; // Sucesso
    }
    return 0; // Falha
}

void salvarFuncionario(const char *arquivo, FuncionarioCadastro *usuario) {
    FILE *fp = fopen(arquivo, "wb"); // Modo de escrita bin�ria
    if (fp) {
        fwrite(usuario, sizeof(FuncionarioCadastro), 1, fp); // Salva os dados do funcion�rio em bin�rio
        fclose(fp);
    }
}

int carregarFuncionario(const char *arquivo, FuncionarioCadastro *usuario) {
    FILE *fp = fopen(arquivo, "rb"); // Modo de leitura bin�ria
    if (fp) {
        fread(usuario, sizeof(FuncionarioCadastro), 1, fp); // Carrega os dados do funcion�rio em bin�rio
        fclose(fp);
        return 1; // Sucesso
    }
    return 0; // Falha
}

// Fun��o para salvar todos os funcion�rios em um �nico arquivo
void salvarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int num_funcionarios) {
    FILE *fp = fopen(arquivo, "wb");
    if (fp) {
        fwrite(&num_funcionarios, sizeof(int), 1, fp);
        fwrite(funcionarios, sizeof(FuncionarioCadastro), num_funcionarios, fp);
        fclose(fp);
    } else {
        printf("Erro ao salvar os dados dos funcion�rios.\n");
    }
}

int carregarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    FILE *fp = fopen(arquivo, "rb");
    if (fp) {
        fread(num_funcionarios, sizeof(int), 1, fp); // L� o n�mero de funcion�rios
        fread(funcionarios, sizeof(FuncionarioCadastro), *num_funcionarios, fp); // L� o array de funcion�rios

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

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Fun��o para exibir o menu do gerente com navega��o por teclado
int mostrarMenuGerente(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posi��o do cursor
    int tecla;

    do {
        system(CLEAR);
        printf("======= %s =======\n", titulo); // Exibe o t�tulo do menu

        // Exibe as op��es do menu com a seta indicando a op��o atual
        for (i = 0; i < numOpcoes; i++) {
            if (posicao == i) {
                setColor(14); // Amarelo para a op��o selecionada
                printf("-> %s\n", opcoes[i]);
            } else {
                setColor(7); // Branco para as outras op��es
                printf("   %s\n", opcoes[i]);
            }
        }
        setColor(7); // Reset para a cor padr�o

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
            return posicao + 1; // Retorna a op��o selecionada (1-based)
        }
    } while (tecla != 27); // ESC para sair do menu

    return 0; // Retorna 0 se o usu�rio pressionar ESC
}

// Fun��o para validar e formatar CPF
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

// Fun��o para validar e formatar telefone
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

// Fun��o para validar e formatar n�mero da conta banc�ria
void ler_conta_bancaria_formatada(char *conta) {
    int pos = 0;
    char c;
    printf("Conta Banc�ria (xxxxxxxx-x): ");
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

// Fun��o para cadastrar um novo funcion�rio
void cadastrarFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    if (*num_funcionarios >= 100) {
        printf("Limite m�ximo de funcion�rios atingido!\n");
        return;
    }

    FuncionarioCadastro novo_func;

    printf("Nome: ");
    fflush(stdin);
    fgets(novo_func.nome, 50, stdin);
    strtok(novo_func.nome, "\n");

    do {
        ler_cpf_formatado(novo_func.cpf);
    } while (strlen(novo_func.cpf) != 14);

    do {
        ler_telefone_formatado(novo_func.telefone);
    } while (strlen(novo_func.telefone) != 15);

    printf("Email: ");
    fflush(stdin);
    fgets(novo_func.email, 50, stdin);
    strtok(novo_func.email, "\n");

    do {
        ler_conta_bancaria_formatada(novo_func.conta_bancaria);
    } while (strlen(novo_func.conta_bancaria) != 10);

    // Define o c�digo do funcion�rio
    novo_func.cod = *num_funcionarios + 1;

    printf("Usu�rio: ");
    fflush(stdin);
    fgets(novo_func.usuario, 20, stdin);
    strtok(novo_func.usuario, "\n");

    printf("Senha: ");
    fflush(stdin);
    fgets(novo_func.senha, 20, stdin);
    strtok(novo_func.senha, "\n");

    funcionarios[*num_funcionarios] = novo_func;
    (*num_funcionarios)++;

    // Salva os dados do funcion�rio em um arquivo
    char nome_arquivo[50];
    sprintf(nome_arquivo, "%s.dat", novo_func.usuario); // Nome do arquivo = usuario.dat
    salvarFuncionario(nome_arquivo, &novo_func);

    printf("\nParab�ns %s, oficialmente contratado!!!\n", novo_func.nome);
    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void listarFuncionarios(FuncionarioCadastro *funcionarios, int num_funcionarios) {
    system(CLEAR);

    // Carrega os dados dos funcion�rios dos arquivos
    for (i = 0; i < num_funcionarios; i++) {
        char nome_arquivo[50];
        sprintf(nome_arquivo, "%s.dat", funcionarios[i].usuario);
        if (!carregarFuncionario(nome_arquivo, &funcionarios[i])) {
            printf("Erro ao carregar funcion�rio %s\n", nome_arquivo);
        }
    }

    if (num_funcionarios == 0) {
        printf("Nenhum funcion�rio cadastrado.\n");
    } else {
        printf("========= LISTA DE FUNCION�RIOS =========\n");
        for (i = 0; i < num_funcionarios; i++) {
            printf("Nome: %s\n", funcionarios[i].nome);
            printf("CPF: %s\n", funcionarios[i].cpf);
            printf("Telefone: %s\n", funcionarios[i].telefone);
            printf("Email: %s\n", funcionarios[i].email);
            printf("Conta Banc�ria: %s\n", funcionarios[i].conta_bancaria);
            printf("Usu�rio: %s\n", funcionarios[i].usuario);
            printf("------------------------------------\n");
        }
    }
    printf("Pressione qualquer tecla para voltar ao menu.\n");
    getch();
}

void alterarFuncionario(FuncionarioCadastro *funcionarios, int num_funcionarios) {
    char cpf_busca[15];
    int i, posicao = -1;

    system(CLEAR);
    printf("========= ALTERAR FUNCION�RIO =========\n");
    printf("Digite o CPF do funcion�rio: ");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcion�rio pelo CPF
    for (i = 0; i < num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Funcion�rio n�o encontrado.\n");
        printf("Pressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Carrega os dados do funcion�rio do arquivo
    char nome_arquivo[50];
    sprintf(nome_arquivo, "%s.dat", funcionarios[posicao].usuario);
    if (carregarFuncionario(nome_arquivo, &funcionarios[posicao])) {
        printf("Funcion�rio carregado com sucesso\n");
    } else {
        printf("Erro ao carregar funcion�rio\n");
    }

    // Altera os dados do funcion�rio
    printf("========= ALTERAR FUNCION�RIO =========\n");
    printf("Digite os novos dados do funcion�rio:\n");

    printf("Nome: ");
    fflush(stdin);
    fgets(funcionarios[posicao].nome, 50, stdin);
    strtok(funcionarios[posicao].nome, "\n");

    do {
        ler_telefone_formatado(funcionarios[posicao].telefone);
    } while (strlen(funcionarios[posicao].telefone) != 15);

    printf("Email: ");
    fflush(stdin);
    fgets(funcionarios[posicao].email, 50, stdin);
    strtok(funcionarios[posicao].email, "\n");

    do {
        ler_conta_bancaria_formatada(funcionarios[posicao].conta_bancaria);
    } while (strlen(funcionarios[posicao].conta_bancaria) != 11);

    printf("Usu�rio: ");
    fflush(stdin);
    fgets(funcionarios[posicao].usuario, 20, stdin);
    strtok(funcionarios[posicao].usuario, "\n");

    printf("Senha: ");
    fflush(stdin);
    fgets(funcionarios[posicao].senha, 20, stdin);
    strtok(funcionarios[posicao].senha, "\n");

    // Salva os dados atualizados do funcion�rio em um arquivo
    sprintf(nome_arquivo, "%s.dat", funcionarios[posicao].usuario);
    salvarFuncionario(nome_arquivo, &funcionarios[posicao]);

    printf("Dados do funcion�rio alterados com sucesso!\n");
    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void excluirFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    char cpf_busca[15];
    int i, posicao = -1;

    system(CLEAR);
    printf("========= EXCLUIR FUNCION�RIO =========\n");
    printf("Digite o CPF do funcion�rio: ");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcion�rio pelo CPF
    for (i = 0; i < *num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Funcion�rio n�o encontrado.\n");
        printf("Pressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Confirma��o da exclus�o
    printf("========= EXCLUIR FUNCION�RIO =========\n");
    printf("Tem certeza que deseja excluir o funcion�rio %s? (S/N): ", funcionarios[posicao].nome);
    char confirmacao = getch();

    if (confirmacao == 'S' || confirmacao == 's') {
        // Remove o funcion�rio do array e seu arquivo
        char nome_arquivo[50];
        sprintf(nome_arquivo, "%s.dat", funcionarios[posicao].usuario);

        // Carrega os dados do funcion�rio do arquivo (para ter certeza de que o nome de usu�rio est� correto)
        carregarFuncionario(nome_arquivo, &funcionarios[posicao]);

        remove(nome_arquivo);

        for (i = posicao; i < *num_funcionarios - 1; i++) {
            funcionarios[i] = funcionarios[i + 1];
        }
        (*num_funcionarios)--;

        printf("Funcion�rio exclu�do com sucesso!\n");
    } else {
        printf("Exclus�o cancelada!\n");
    }

    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

// Menu do Gerente
void menuGerente() {
    int escolha;

    const char *opcoes_menu_gerente[] = {
        "1. Cadastro de funcion�rio",
        "2. Acessar Funcion�rio",
        "3. Leis do Consumidor",
        "4. Sair"
    };
    int num_opcoes_menu_gerente = sizeof(opcoes_menu_gerente) / sizeof(opcoes_menu_gerente[0]);

    do {
        escolha = mostrarMenuGerente("MENU GERENTE", opcoes_menu_gerente, num_opcoes_menu_gerente);

        switch (escolha) {
            case 1:
                system(CLEAR);
                cadastrarFuncionario(funcionarios, &num_funcionarios);
                break;
            case 2: // Acessar Funcion�rio
                {
                    int escolha_submenu;
                    const char *opcoes_submenu_acessar_funcionario[] = {
                        "1. Alterar dados do funcion�rio",
                        "2. Excluir funcion�rio",
                        "3. Listar funcion�rios",
                        "4. Voltar"
                    };
                    int num_opcoes_submenu = sizeof(opcoes_submenu_acessar_funcionario) / sizeof(opcoes_submenu_acessar_funcionario[0]);

                    do {
                        escolha_submenu = mostrarMenuGerente("ACESSAR FUNCION�RIO", opcoes_submenu_acessar_funcionario, num_opcoes_submenu);

                        switch (escolha_submenu) {
                            case 1:
                                system(CLEAR);
                                alterarFuncionario(funcionarios, num_funcionarios);
                                break;
                            case 2:
                                system(CLEAR);
                                excluirFuncionario(funcionarios, &num_funcionarios);
                                break;
                            case 3: // Listar funcion�rios
                                system(CLEAR);
                                listarFuncionarios(funcionarios, num_funcionarios);
                                break;
                            case 4: // Voltar ao menu principal
                                // Voltar ao menu principal
                                break;
                            default:
                                printf("Op��o inv�lida!\n");
                        }
                    } while (escolha_submenu != 4);
                }
                break;
            case 3:
                system(CLEAR);
                printf("Op��o 3 escolhida (Leis do Consumidor).\n");
                printf("Pressione qualquer tecla para voltar ao menu.\n");
                getch();
                break;
            case 4:
                printf("Saindo do menu do gerente...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }
    } while (escolha != 4);
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
            printf("-> 1. Entrar\n");
        } else {
            setColor(7);
            printf("   1. Entrar\n");
        }
        if (posicao == 1) {
            setColor(14);
            printf("-> 2. Sair\n");
        } else {
            setColor(7);
            printf("   2. Sair\n");
        }
        setColor(7);

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
            printf("Saindo do sistema...\n");
            exit(0);
        }

        if (opcao == 1) {
            printf("   Usu�rio: ");
            scanf("%s", login);
            printf("   Senha: ");

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

            // Tenta carregar o usu�rio como gerente
            if (carregarGerente(login, &gerente)) {
                if (strcmp(gerente.senha, senha) == 0) {
                    system(CLEAR);
                    printf("Bem-vindo, %s! Aguarde um momento...\n", gerente.nome);
                    Sleep(4200);
                    system("cls");
                    menuGerente();
                    return;
                } else {
                    printf("Senha incorreta!\n");
                    Sleep(2200);
                    system("cls");
                }
            } else {
                // Se n�o for gerente, tenta carregar como funcion�rio
                char nome_arquivo[50];
                sprintf(nome_arquivo, "%s.dat", login);
                if (carregarFuncionario(nome_arquivo, &funcionario)) {
                    if (strcmp(funcionario.senha, senha) == 0) {
                        printf("\n\n\tBem-vindo, %s!\n", funcionario.nome);
                        Sleep(4200);
                        system("cls");
                        // menuFuncionario(); // Implementa��o futura do menu do funcion�rio
                        return;
                    } else {
                        printf("Senha incorreta!\n");
                        Sleep(2200);
                        system("cls");
                    }
                } else {
                    printf("Usu�rio n�o encontrado!\n");
                    Sleep(2200);
                    system("cls");
                }
            }
        }
    } while (opcao != 2);
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    Gerente gerente = {"Ana", "admin", "1234", 01};
    salvarGerente("admin", &gerente);

    // Carrega todos os funcion�rios do arquivo "funcionarios.dat"
    if (!carregarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, &num_funcionarios)) {
        printf("Nenhum funcion�rio encontrado. Inicializando com lista vazia.\n");
    }

    // **Remove o loop for que tentava carregar os funcion�rios individualmente**

    printf("N�mero de funcion�rios carregados: %d\n", num_funcionarios);

    login();

    // Salva todos os funcion�rios em "funcionarios.dat" antes de sair
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);

    return 0;
}
