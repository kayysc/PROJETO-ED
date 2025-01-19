#include "auxiliar.h"

int i,k;

// Funções do menu do funcionário
void menuFuncionario() {
	// Declaração e inicialização das variáveis
    struct cliente *clientes = NULL;  
    int num_clientes = 0;
    const char *opcoes_menu_principal[] = {
        "Listar notas fiscais",
        "Área do cliente",
        "Rotas",
        "Leis do consumidor",
        "Sair"
    };
    int num_opcoes_menu_principal = sizeof(opcoes_menu_principal) / sizeof(opcoes_menu_principal[0]);
    int escolha;

    do {
        escolha = mostrarMenu("MENU FUNCIONÁRIO", opcoes_menu_principal, num_opcoes_menu_principal);
        switch (escolha) {
            case 0:
                ListagemNota(); // Listagem de NotaFiscal
                break;
            case 1:{
				char ***assentos = criarAssentos(rotas[0].linhas, rotas[0].colunas); // Cria a matriz de assentos
    			menuCliente(assentos, rotas[0].linhas, rotas[0].colunas); // Passa a matriz para menuCliente
    			liberarAssentos(assentos, rotas[0].linhas, rotas[0].colunas); // Libera a matriz após o uso
				break;
			}
            case 2:
                listarRotas(clientes, num_clientes); // Chama a função listarRotas com o array de clientes e o número de clientes
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
        		char descricaoOperacao[200];
        		snprintf(descricaoOperacao, sizeof(descricaoOperacao), "Logout");
        		registrarOperacao(userLogado, descricaoOperacao); 
                login();
                break;
        }
    } while (escolha != 4); // 4 é o índice da opção "Sair"

    // Liberação de memória dos clientes cadastrados
    free(clientes);
}

void cancelarPorCPF(char ***assentos, int linhas, int colunas) {
    char cpf_formatado[15], cpf_limpo[12];
    int encontrado = 0;

    system(CLEAR);
    printf("\n        Digite o CPF do cliente:\n");
    ler_cpf_formatado(cpf_formatado);
    remover_formatacao_cpf(cpf_formatado, cpf_limpo);

    struct Nota *atual = notasfiscais, *anterior = NULL;

    while (atual != NULL) {
        char cpf_atual_limpo[12];
        remover_formatacao_cpf(atual->cpf, cpf_atual_limpo);

        if (strcmp(cpf_limpo, cpf_atual_limpo) == 0) {
            encontrado = 1;

            // Exibe as informações da nota fiscal
            printf("\n        Nota Fiscal Encontrada:\n");
            printf("        ID: %d\n", atual->notaFiscal);
            printf("        Nome: %s\n", atual->nome);
            printf("        CPF: %s\n", atual->cpf);
            printf("        Origem: %s\n", atual->origem);
            printf("        Destino: %s\n", atual->destino);
            printf("        Data: %s\n", atual->data);
            printf("        Horário: %s\n", atual->horario);
            printf("        Valor: R$ %.2f\n", (float)atual->preco);
            printf("        Assentos: ");
            for ( i = 0; i < MAX_PASSAGENS && atual->assentos[i][0] != '\0'; i++) {
                printf("%s ", atual->assentos[i]);
            }
            printf("\n");
            printf("\n        Deseja cancelar esta compra? (S/N): ");
            fflush(stdin);
            char confirmacao = getchar();
            getchar();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Identificar a rota correspondente
                int r = -1;
                for ( i = 0; i < MAX_DESTINOS; i++) {
                    if (strcmp(atual->origem, rotas[i].origem) == 0 && strcmp(atual->destino, rotas[i].destino) == 0) {
                        r = i;
                        break;
                    }
                }

                if (r == -1) {
                    printf("\n        Erro: Rota correspondente não encontrada.\n");
                    printf("        Pressione qualquer tecla para voltar ao menu...");
                    getchar();
                    return;
                }

                // Recarregar o estado atual dos assentos
                char nomeArquivoAssentos[50];
                sprintf(nomeArquivoAssentos, "assentos_rota_%d.txt", r);
                carregarAssentos(assentos, linhas, colunas, nomeArquivoAssentos);

                // Liberar apenas os assentos associados à nota fiscal
                for ( k = 0; k < MAX_PASSAGENS && atual->assentos[k][0] != '\0'; k++) {
                    int linha = atual->assentos[k][0] - 'A';
                    int coluna = atual->assentos[k][2] - '1';
                    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
                        sprintf(assentos[linha][coluna], "%c-%d", LETRAS[linha], coluna + 1);
                    }
                }

                // Salvar o estado atualizado dos assentos
                salvarAssentos(assentos, linhas, colunas, nomeArquivoAssentos);

                // Remove a nota fiscal da lista
                if (anterior == NULL) {
                    notasfiscais = atual->prox_Nota;
                } else {
                    anterior->prox_Nota = atual->prox_Nota;
                }
                 //registro
				char descricaoOperacao[500];
    			snprintf(descricaoOperacao, sizeof(descricaoOperacao), "Cancelamento da compra: |ID- %d |Nome- %s |CPF- %s)", atual->notaFiscal, atual->nome, atual->cpf);
   				registrarOperacao(userLogado, descricaoOperacao);
                free(atual);
                free(atual);

                // Persistir as notas fiscais atualizadas
                salvarNotasFiscais("notasfiscais.dat");

                printf("\n        Compra cancelada com sucesso!\n");
                printf("        Pressione qualquer tecla para voltar ao menu...");
                getchar();
                break;
            } else {
                printf("\n        Cancelamento abortado.\n");
                printf("        Pressione qualquer tecla para voltar ao menu...");
                getchar();
                break;
            }
        }
        anterior = atual;
        atual = atual->prox_Nota;
    }

    if (!encontrado) {
        printf("\n        Nenhuma nota fiscal encontrada para o CPF informado.\n");
        printf("        Pressione qualquer tecla para voltar ao menu...");
        getchar();
    }
}

void cancelarPorID(char ***assentos, int linhas, int colunas) {
    int id, encontrado = 0;

    system(CLEAR);
    printf("\n        Digite o ID da Nota Fiscal: ");

    if (scanf("%d", &id) != 1) {
        printf("\n        ID inválido. Tente novamente.\n");
        getchar();
        return;
    }
    getchar();

    struct Nota *atual = notasfiscais, *anterior = NULL;

    while (atual != NULL) {
        if (atual->notaFiscal == id) {
            encontrado = 1;

            // Exibe as informações da nota fiscal
            printf("\n        Nota Fiscal Encontrada:\n");
            printf("        ID: %d\n", atual->notaFiscal);
            printf("        Nome: %s\n", atual->nome);
            printf("        CPF: %s\n", atual->cpf);
            printf("        Origem: %s\n", atual->origem);
            printf("        Destino: %s\n", atual->destino);
            printf("        Data: %s\n", atual->data);
            printf("        Horário: %s\n", atual->horario);
            printf("        Valor: R$ %.2f\n", (float)atual->preco);
            printf("        Assentos: ");
            for ( i = 0; i < MAX_PASSAGENS && atual->assentos[i][0] != '\0'; i++) {
                printf("%s ", atual->assentos[i]);
            }
            printf("\n");
            printf("\n        Deseja cancelar esta compra? (S/N): ");
            fflush(stdin);
            char confirmacao = getchar();
            getchar();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Identificar a rota correspondente
                int r = -1;
                for ( i = 0; i < MAX_DESTINOS; i++) {
                    if (strcmp(atual->origem, rotas[i].origem) == 0 && strcmp(atual->destino, rotas[i].destino) == 0) {
                        r = i;
                        break;
                    }
                }

                if (r == -1) {
                    printf("\n        Erro: Rota correspondente não encontrada.\n");
                    printf("        Pressione qualquer tecla para voltar ao menu...");
                    getchar();
                    return;
                }

                // Recarregar o estado atual dos assentos
                char nomeArquivoAssentos[50];
                sprintf(nomeArquivoAssentos, "assentos_rota_%d.txt", r);
                carregarAssentos(assentos, linhas, colunas, nomeArquivoAssentos);

                // Liberar apenas os assentos associados à nota fiscal
                for ( k = 0; k < MAX_PASSAGENS && atual->assentos[k][0] != '\0'; k++) {
                    int linha = atual->assentos[k][0] - 'A';
                    int coluna = atual->assentos[k][2] - '1';
                    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
                        sprintf(assentos[linha][coluna], "%c-%d", LETRAS[linha], coluna + 1);
                    }
                }

                // Salvar o estado atualizado dos assentos
                salvarAssentos(assentos, linhas, colunas, nomeArquivoAssentos);

                // Remove a nota fiscal da lista
                if (anterior == NULL) {
                    notasfiscais = atual->prox_Nota;
                } else {
                    anterior->prox_Nota = atual->prox_Nota;
                }
                
                // Persistir as notas fiscais atualizadas
                salvarNotasFiscais("notasfiscais.dat");
                 
				 //registro
				char descricaoOperacao[500];
    			snprintf(descricaoOperacao, sizeof(descricaoOperacao), "Cancelamento da compra: |ID- %d |Nome- %s |CPF- %s)", atual->notaFiscal, atual->nome, atual->cpf);
   				registrarOperacao(userLogado, descricaoOperacao);
                
				free(atual);
				printf("\n        Compra cancelada com sucesso!\n");
                printf("        Pressione qualquer tecla para voltar ao menu...");
                getchar();
                break;
            } else {
                printf("\n        Cancelamento abortado.\n");
                printf("        Pressione qualquer tecla para voltar ao menu...");
                getchar();
                break;
            }
        }
        anterior = atual;
        atual = atual->prox_Nota;
    }

    if (!encontrado) {
        printf("\n        Nenhuma nota fiscal encontrada para o ID informado.\n");
        printf("        Pressione qualquer tecla para voltar ao menu...");
        getchar();
    }
}

void cancelarCompra(char ***assentos, int linhas, int colunas) {
    int opcao;
    do {
        opcao = mostrarMenu("CANCELAMENTO DE COMPRA",
                            (const char *[]){"Cancelar por CPF",
                                             "Cancelar por ID da Nota Fiscal",
                                             "Voltar ao menu"}, 3);

        switch (opcao) {
            case 0: // Cancelar por CPF
                cancelarPorCPF(assentos, linhas, colunas);
                break;
            case 1: // Cancelar por ID
                cancelarPorID(assentos, linhas, colunas);
                break;
            case 2: // Voltar ao menu
                return;
        }
    } while (opcao != 2);
}
