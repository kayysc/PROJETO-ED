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
        printf("Erro ao salvar os dados dos funcionários.\n");
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

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Função para exibir o menu do gerente com navegação por teclado
int mostrarMenuGerente(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;

    do {
        system(CLEAR);
        printf("======= %s =======\n", titulo); // Exibe o título do menu

        // Exibe as opções do menu com a seta indicando a opção atual
        for (i = 0; i < numOpcoes; i++) {
            if (posicao == i) {
                setColor(14); // Amarelo para a opção selecionada
                printf("-> %s\n", opcoes[i]);
            } else {
                setColor(7); // Branco para as outras opções
                printf("   %s\n", opcoes[i]);
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

// Função para cadastrar um novo funcionário
void cadastrarFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    if (*num_funcionarios >= 100) {
        printf("Limite máximo de funcionários atingido!\n");
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

    // Define o código do funcionário
    novo_func.cod = *num_funcionarios + 1;

    printf("Usuário: ");
    fflush(stdin);
    fgets(novo_func.usuario, 20, stdin);
    strtok(novo_func.usuario, "\n");

    printf("Senha: ");
    fflush(stdin);
    fgets(novo_func.senha, 20, stdin);
    strtok(novo_func.senha, "\n");

    funcionarios[*num_funcionarios] = novo_func;
    (*num_funcionarios)++;

    // Salva todos os funcionários após o cadastro
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, *num_funcionarios);

    printf("\nParabéns %s, oficialmente contratado!!!\n", novo_func.nome);
    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void listarFuncionarios(FuncionarioCadastro *funcionarios, int num_funcionarios) {
    system(CLEAR);

    // Carrega todos os funcionários do arquivo "funcionarios.dat"
    carregarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, &num_funcionarios);

    if (num_funcionarios == 0) {
        printf("Nenhum funcionário cadastrado.\n");
    } else {
        printf("========= LISTA DE FUNCIONÁRIOS =========\n");
        for (i = 0; i < num_funcionarios; i++) {
            printf("Nome: %s\n", funcionarios[i].nome);
            printf("CPF: %s\n", funcionarios[i].cpf);
            printf("Telefone: %s\n", funcionarios[i].telefone);
            printf("Email: %s\n", funcionarios[i].email);
            printf("Conta Bancária: %s\n", funcionarios[i].conta_bancaria);
            printf("Usuário: %s\n", funcionarios[i].usuario);
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
    printf("========= ALTERAR FUNCIONÁRIO =========\n");
    printf("Digite o CPF do funcionário: ");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcionário pelo CPF
    for (i = 0; i < num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Funcionário não encontrado.\n");
        printf("Pressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Altera os dados do funcionário
    printf("========= ALTERAR FUNCIONÁRIO =========\n");
    printf("Digite os novos dados do funcionário:\n");

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

    printf("Usuário: ");
    fflush(stdin);
    fgets(funcionarios[posicao].usuario, 20, stdin);
    strtok(funcionarios[posicao].usuario, "\n");

    printf("Senha: ");
    fflush(stdin);
    fgets(funcionarios[posicao].senha, 20, stdin);
    strtok(funcionarios[posicao].senha, "\n");

    // Salva todos os funcionários após a alteração
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);

    printf("Dados do funcionário alterados com sucesso!\n");
    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void excluirFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    char cpf_busca[15];
    int i, posicao = -1;

    system(CLEAR);
    printf("========= EXCLUIR FUNCIONÁRIO =========\n");
    printf("Digite o CPF do funcionário: ");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcionário pelo CPF
    for (i = 0; i < *num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Funcionário não encontrado.\n");
        printf("Pressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Confirmação da exclusão
    printf("========= EXCLUIR FUNCIONÁRIO =========\n");
    printf("Tem certeza que deseja excluir o funcionário %s? (S/N): ", funcionarios[posicao].nome);
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

        printf("Funcionário excluído com sucesso!\n");
    } else {
        printf("Exclusão cancelada!\n");
    }

    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}

void login();

// Menu do Gerente
void menuGerente() {
    int escolha;

    const char *opcoes_menu_gerente[] = {
        "1. Cadastro de funcionário",
        "2. Acessar Funcionário",
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
            case 2: // Acessar Funcionário
                {
                    int escolha_submenu;
                    const char *opcoes_submenu_acessar_funcionario[] = {
                        "1. Alterar dados do funcionário",
                        "2. Excluir funcionário",
                        "3. Listar funcionários",
                        "4. Voltar"
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
                printf("Opção 3 escolhida (Leis do Consumidor).\n");
                printf("Pressione qualquer tecla para voltar ao menu.\n");
                getch();
                break;
            case 4:
                printf("Saindo do menu do gerente...\n");
                Sleep(3200);
                login();
                break;
            default:
                printf("Opção inválida!\n");
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
        	system(CLEAR);
        	printf("\n ============= BEM VINDO AO BANZEIRO!!! =============\n\n");
            printf("   Usuário: ");
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

            // Tenta carregar o usuário como gerente
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
                // Se não for gerente, verifica se é funcionário
                for (i = 0; i < num_funcionarios; i++) {
                    if (strcmp(funcionarios[i].usuario, login) == 0 && 
                        strcmp(funcionarios[i].senha, senha) == 0) {
                        printf("\n\n\tBem-vindo, %s!\n", funcionarios[i].nome);
                        Sleep(4200);
                        system("cls");
                        // menuFuncionario(); // Implementação futura do menu do funcionário
                        return;
                    }
                }

                // Se o loop terminar sem encontrar o funcionário
                printf("Usuário não encontrado ou senha incorreta!\n");
                Sleep(2200);
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
        printf("Nenhum funcionário encontrado. Inicializando com lista vazia.\n");
    }

    printf("Número de funcionários carregados: %d\n", num_funcionarios);

    login();

    // Salva todos os funcionários em "funcionarios.dat" antes de sair
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);

    return 0;
}
