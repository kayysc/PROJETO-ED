#include "auxiliar.h"

// Definição das variáveis funcionarios e num_funcionarios
FuncionarioCadastro funcionarios[100];  
int num_funcionarios = 0; 

//Variáveis globais
int i,j;

// Funções de persistência de dados
void salvarGerente(const char *arquivo, Gerente *usuario) {
    FILE *fp = fopen(arquivo, "wb");
    if (fp) {
        fwrite(usuario, sizeof(Gerente), 1, fp);
        fclose(fp);
    }
}

int carregarGerente(const char *arquivo, Gerente *usuario) {
    FILE *fp = fopen(arquivo, "rb");
    if (fp) {
        fread(usuario, sizeof(Gerente), 1, fp);
        fclose(fp);
        return 1; // Sucesso
    }
    return 0; // Falha
}

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

// Funções do menu do gerente
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
