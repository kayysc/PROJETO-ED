#include "auxiliar.h"

//Variáveis globais
int i,j;

// Definição da variável notasfiscais
struct Nota *notasfiscais = NULL;

void salvarNotasFiscais(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "wb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para salvar as notas fiscais.\n");
        return;
    }

    struct Nota *notaAtual = notasfiscais;
    while (notaAtual != NULL) {
        fwrite(notaAtual, sizeof(struct Nota), 1, fp);
        notaAtual = notaAtual->prox_Nota;
    }

    fclose(fp);
}

void carregarNotasFiscais(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "rb");
    if (fp == NULL) {
        // Se o arquivo não existir, não faz nada
        return;
    }

    struct Nota *notaAnterior = NULL;
    while (1) {
        struct Nota *notaAtual = (struct Nota *)malloc(sizeof(struct Nota));
        if (notaAtual == NULL) {
            printf("Erro ao alocar memoria para a nota fiscal.\n");
            exit(1);
        }
        size_t elementosLidos = fread(notaAtual, sizeof(struct Nota), 1, fp);
        if (elementosLidos == 0) {
            free(notaAtual); // Libera a memória alocada para a última nota
            break;
        }

        notaAtual->prox_Nota = NULL;
        if (notaAnterior == NULL) {
            notasfiscais = notaAtual;
        } else {
            notaAnterior->prox_Nota = notaAtual;
        }
        notaAnterior = notaAtual;
    }

    fclose(fp);
}

void geracaoNotaFiscal(Rota rota, int preco_total, const char *nome, const char *cpf, char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos) {
    system("cls");

    struct Nota *aux = (struct Nota *)malloc(sizeof(struct Nota));

    strcpy(aux->origem, rota.origem);
    strcpy(aux->destino, rota.destino);
    strcpy(aux->horario, rota.horario);
    strcpy(aux->data, rota.data);
    strcpy(aux->nome, nome);
    strcpy(aux->cpf, cpf);
    aux->preco = preco_total;

    // Copia os assentos para a nota fiscal
    for (i = 0; i < totalAssentos; i++) {
        strcpy(aux->assentos[i], assentosEscolhidos[i]);
    }

    aux->notaFiscal = rand() % 10000;
    aux->prox_Nota = notasfiscais;
    notasfiscais = aux;

    // Salva as notas fiscais após gerar a nova nota
    salvarNotasFiscais("notasfiscais.dat");

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
    // Exibe os assentos da compra
    printf("       Assentos: ");
    for (i = 0; i < totalAssentos; i++) {
        printf("%s ", aux->assentos[i]);
    }
    printf("\n");

    printf("\n ==========================================\n");
}

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

        // Exibe os assentos da nota fiscal
        printf("       Assentos: ");
        for (j = 0; j < MAX_PASSAGENS && buscar->assentos[j][0] != '\0'; j++) {
            printf("%s ", buscar->assentos[j]);
        }
        printf("\n");

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

void liberarAssentosSelecionados(char ***assentos, char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos) {
    for (int i = 0; i < totalAssentos; i++) {
        int linha = assentosEscolhidos[i][0] - 'A'; // Converter letra da linha para índice
        int coluna = assentosEscolhidos[i][2] - '1'; // Converter número da coluna para índice
        if (linha >= 0 && linha < MAX_PASSAGENS && coluna >= 0 && coluna < MAX_PASSAGENS) {
            sprintf(assentos[linha][coluna], "%c-%d", LETRAS[linha], coluna + 1); // Reverter para o estado original
        }
    }
}

void finalizarCompra(int totalAssentos, Rota rota, char assentosEscolhidos[MAX_PASSAGENS][5], const char *nome, const char *cpf, char ***assentos) {
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
            liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
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
            geracaoNotaFiscal(rota, preco_total, nome, cpf, assentosEscolhidos, totalAssentos);
            printf("\n       Compra confirmada! Pressione qualquer tecla para continuar.\n");
            _getch();
            break; // Sai do loop após a confirmação da compra
        } else { // Compra cancelada
            printf("\n       Compra cancelada! Voltando ao Menu Principal...\n");
            // Liberar os assentos selecionados
            liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
            _getch();
        }
    } 
}

int detalhesDaCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota, char ***assentos) {
    char resposta;
    system(CLEAR);
    printf("\n ============= DETALHES DA COMPRA =============\n\n");
    printf("        Rota: %s -> %s\n", rota.origem, rota.destino);
    printf("        Data: %s\n", rota.data);
    printf("        Horário: %s\n", rota.horario);
    printf("\n        Assentos Selecionados:\n\n");
    for (int i = 0; i < totalAssentos; i++) {
        setColor(13);
        printf("        - %s\n", assentosEscolhidos[i]);
    }
    setColor(15);
    printf("\n ===============================================\n\n");
    printf("        Cliente possui cadastro? (S/N): ");
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        char cpf_busca[15];
        system(CLEAR);
        printf("\n ============= BUSCAR CLIENTE POR CPF =============\n\n");
        printf("        Digite abaixo o CPF do cliente");
        printf("\n");
        ler_cpf_formatado(cpf_busca);

        // Chama a função buscarCliente_por_cpf_sem_exibir
        int indice_cliente = buscarCliente_por_cpf_sem_exibir(cpf_busca); 

        if (indice_cliente != -1) { // Verifica se o cliente foi encontrado
            printf("\n        Cliente Identificado: %s. Prosseguir com a compra? (S/N) ", clientes[indice_cliente].nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos, clientes[indice_cliente].nome, clientes[indice_cliente].cpf, assentos);
            } else {
                printf("\n        Compra cancelada! Voltando ao Menu Principal...\n");
                liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
                _getch();
                return 0;
            }
        } else {
            printf("\n        Cliente não encontrado.\n\nPressione qualquer tecla para continuar...\n");
            liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
            _getch();
        }
    } else if (resposta == 'N' || resposta == 'n') {
        printf("        Cliente deseja realizar cadastro? (S/N): ");
        scanf(" %c", &resposta);

        if (resposta == 'S' || resposta == 's') {
            system(CLEAR);
            printf("\n ============= CADASTRAR CLIENTE =============\n\n");

            // Chama a função cadastrar_cliente
            cadastrar_cliente(); 

            printf("\n        Cliente cadastrado: %s!!! Prosseguir com a compra? (S/N) ", clientes[num_clientes - 1].nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos, clientes[num_clientes - 1].nome, clientes[num_clientes - 1].cpf, assentos);
            } else {
                printf("\n        Compra cancelada! Voltando ao Menu Principal...\n");
                liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
                _getch();
                return 0;
            }
        } else {
            system("cls");
            struct cliente cliente_temp;
            printf("\n ============= INFORMAÇÕES DO CLIENTE =============\n\n");
            printf("        Nome: ");
            fflush(stdin);
            fgets(cliente_temp.nome, 50, stdin);
            strtok(cliente_temp.nome, "\n");

            ler_cpf_formatado(cliente_temp.cpf);
            ler_telefone_formatado(cliente_temp.telefone);
            printf("\n        Cliente %s pronto para embarque!!! Prosseguir com a compra? (S/N) ", cliente_temp.nome);
            scanf(" %c", &resposta);
            if (resposta == 'S' || resposta == 's') {
                finalizarCompra(totalAssentos, rota, assentosEscolhidos, cliente_temp.nome, cliente_temp.cpf, assentos);
            } else {
                printf("\n        Compra cancelada! Voltando ao Menu Principal...\n");
                liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
                _getch();
                return 0;
            }
        }
    } else {
        printf("        Resposta inválida! Pressione qualquer tecla para continuar.\n");
        liberarAssentosSelecionados(assentos, assentosEscolhidos, totalAssentos);
        _getch();
    }

    return 0;
}
