#include "auxiliar.h"

// Funções do menu do funcionário
void menuFuncionario() {
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
    } while (escolha != 4); // 4 é o índice da opção "Sair"

    // Liberação de memória dos clientes cadastrados
    struct cliente *temp;
    while (head_cliente != NULL) {
        temp = head_cliente;
        head_cliente = head_cliente->proximo_cl;
        free(temp);
    }
}

void cancelarCompra() {
    char cpf_formatado[15], cpf_limpo[12];
    int id, encontrado = 0;
    int opcao = 0; // Inicializa a opção selecionada
    int posicao = 0; // Inicializa a posição do cursor
    int tecla;

    do {
        system(CLEAR);
        printf("\n ============= CANCELAMENTO DE COMPRA =============\n\n");

        // Exibe as opções do menu com a seta indicando a opção atual
        if (posicao == 0) {
            setColor(14); // Amarelo para a opção selecionada
            printf("    -> Cancelar por CPF\n");
        } else {
            setColor(7); // Branco para as outras opções
            printf("    -> Cancelar por CPF\n");
        }
        if (posicao == 1) {
            setColor(14); // Amarelo para a opção selecionada
            printf("    -> Cancelar por ID da Nota Fiscal\n");
        } else {
            setColor(7); // Branco para as outras opções
            printf("    -> Cancelar por ID da Nota Fiscal\n");
        }
        if (posicao == 2) {
            setColor(14); // Amarelo para a opção selecionada
            printf("    -> Voltar ao menu\n");
        } else {
            setColor(7); // Branco para as outras opções
            printf("    -> Voltar ao menu\n");
        }
        setColor(15); // Reset para a cor padrão

        tecla = _getch();
        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posicao > 0) posicao--;
                    break;
                case 80: // Seta para baixo
                    if (posicao < 2) posicao++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            opcao = posicao + 1; // Define a opção selecionada
        }

        switch (opcao) {
            case 1: // Cancelar por CPF
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
                        getchar();
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
                opcao = 0; // Reinicializa a opção para voltar ao menu
                break;

            case 2: // Cancelar por ID
                system(CLEAR);
                printf("\n       Digite o ID da Nota Fiscal: ");
                scanf("%d", &id);
                getchar();

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
                        getchar();
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
                opcao = 0; // Reinicializa a opção para voltar ao menu
                break;

            case 3: // Voltar ao menu
                return; // Sai da função cancelarCompra

            default:
                break; // Ignora outras teclas
        }
    } while (opcao != 3); // 3 representa a opção "Voltar ao menu"
}
