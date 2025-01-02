
#include "Funcionalidade.h"
#include "Util.h"

#include "Cliente.h"
#include "Menu.h"

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

int mostrarMenuGerente(const char *titulo,const char **opcoes,int numOpcoes){
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;
    int i;
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

void menuGerente(){
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
int mostrarMenu(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;
    int i;
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


