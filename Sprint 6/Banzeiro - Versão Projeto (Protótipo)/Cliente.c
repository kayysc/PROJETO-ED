#include "Cliente.h"
#include "Util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>


int prox_id=1;
void cadastrar_cliente(struct cliente **head) {
    struct cliente *novo_cl = (struct cliente *)malloc(sizeof(struct cliente));
    if (!novo_cl) {
        setColor(12);
        printf("\n       Erro ao alocar memÛria!\n");
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
            printf("Data invalida");
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
    return NULL; // Retorna NULL se o cliente n√£o for encontrado
}
struct cliente* buscarCliente_por_cpf_sem_exibir(struct cliente *head, const char *cpf) {
    return buscarCliente(head, cpf, 0); // Chama buscarCliente com exibir_informacoes = 0
}

struct cliente* buscarCliente_por_cpf_com_exibir(struct cliente *head, const char *cpf) {
    return buscarCliente(head, cpf, 1); // Chama buscarCliente com exibir_informacoes = 1
}


