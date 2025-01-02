
#include "NotaFiscal.h"
#include "Rotas.h"
#include "Util.h"
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
struct Nota *notasfiscais = NULL; // Definição da variável

int i,j;
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
        getchar();

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
                break;

            case '2': // Cancelar por ID
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
                break;

            case '0': // Voltar ao menu
                return;

            default:
                printf("\n       Opção inválida! Pressione qualquer tecla para tentar novamente...");
                getchar();
        }
    } while (opcao != '0');
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
