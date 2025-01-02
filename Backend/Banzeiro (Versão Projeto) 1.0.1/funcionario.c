#include "auxiliar.h"

// Fun��es do menu do funcion�rio
void menuFuncionario() {
    const char *opcoes_menu_principal[] = {
        "Listar notas fiscais",
        "�rea do cliente",
        "Rotas",
        "Leis do consumidor",
        "Sair"
    };
    int num_opcoes_menu_principal = sizeof(opcoes_menu_principal) / sizeof(opcoes_menu_principal[0]);

    int escolha;
    struct cliente *head_cliente = NULL; // Inicializa a lista de clientes

    do {
        escolha = mostrarMenu("MENU FUNCION�RIO", opcoes_menu_principal, num_opcoes_menu_principal);
        switch (escolha) {
            case 0:
                ListagemNota(); // Listagem de NotaFiscal
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

                printf("\n        Legisla��o Aplic�vel aos Idosos\n\n"
                       "        1. Lei n� ");
                setColor(14);
                printf("10.741/2003");
                setColor(15);
                printf(" (Estatuto do Idoso)\n\n"
                       "        - Estabelece direitos para pessoas com 60 anos ou mais, com foco na garantia de\n"
                       "        acesso priorit�rio e tratamento adequado nos servi�os p�blicos e privados, incluindo\n"
                       "        transporte.\n\n"
                       "        2. Lei n� ");
                setColor(14);
                printf("10.048/2000");
                setColor(15);
                printf(" (Lei da Prioridade)\n\n"
                       "        - Garante a prioridade para idosos, gestantes, lactantes, pessoas com defici�ncia e\n"
                       "        pessoas acompanhadas por crian�as de colo no uso de servi�os p�blicos e\n"
                       "        privados, incluindo o transporte fluvial.\n\n");
                printf("\n\n        Legisla��o Aplic�vel aos Estudantes\n\n"
                       "        1. Lei n� ");
                setColor(14);
                printf("12.852/2013");
                setColor(15);
                printf(" (Estatuto da Juventude)\n\n"
                       "        - Garante direitos aos jovens de 15 a 29 anos, incluindo meia-passagem ou\n"
                       "        descontos no transporte coletivo, quando aplic�vel, e acesso preferencial a servi�os\n"
                       "        de educa��o, cultura, sa�de e transporte.\n\n"
                       "        2. Lei n� ");
                setColor(14);
                printf("12.933/2013");
                setColor(15);
                printf(" (Lei da Meia-Entrada)\n\n"
                       "        - Estende o direito � meia-entrada para transporte p�blico e eventos culturais,\n"
                       "        mediante a apresenta��o da Carteira de Identifica��o Estudantil (CIE) emitida por\n"
                       "        entidades estudantis reconhecidas.\n");
                printf("\n\nPressione qualquer tecla para voltar ao menu.\n");
                getch();
                break;
            case 4:
                system("cls");
                printf("\n ============= MENU FUNCION�RIO =============\n\n");
                printf("       Saindo do menu do funcion�rio...\n");
                Sleep(3200);
                login();
                break;
        }
    } while (escolha != 4); // 4 � o �ndice da op��o "Sair"

    // Libera��o de mem�ria dos clientes cadastrados
    struct cliente *temp;
    while (head_cliente != NULL) {
        temp = head_cliente;
        head_cliente = head_cliente->proximo_cl;
        free(temp);
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
        printf("\n       Escolha uma op��o: ");
        opcao = getchar();
        getchar(); // Captura o '\n' adicional ap�s o getchar()

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
                        // Exibe as informa��es da nota fiscal
                        printf("\n       Nota Fiscal Encontrada:\n");
                        printf("       ID: %d\n", atual->notaFiscal);
                        printf("       Nome: %s\n", atual->nome);
                        printf("       CPF: %s\n", atual->cpf);
                        printf("       Origem: %s\n", atual->origem);
                        printf("       Destino: %s\n", atual->destino);
                        printf("       Data: %s\n", atual->data);
                        printf("       Hor�rio: %s\n", atual->horario);
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
                        // Exibe as informa��es da nota fiscal
                        printf("\n       Nota Fiscal Encontrada:\n");
                        printf("       ID: %d\n", atual_id->notaFiscal);
                        printf("       Nome: %s\n", atual_id->nome);
                        printf("       CPF: %s\n", atual_id->cpf);
                        printf("       Origem: %s\n", atual_id->origem);
                        printf("       Destino: %s\n", atual_id->destino);
                        printf("       Data: %s\n", atual_id->data);
                        printf("       Hor�rio: %s\n", atual_id->horario);
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
                printf("\n       Op��o inv�lida! Pressione qualquer tecla para tentar novamente...");
                getchar();
        }
    } while (opcao != '0');
}
