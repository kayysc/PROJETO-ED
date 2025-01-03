#include "auxiliar.h"

// Definição da variável prox_id
int prox_id = 1;

// Funções do menu do cliente
void cadastrar_cliente(struct cliente **head) {
    struct cliente *novo_cl = (struct cliente *)malloc(sizeof(struct cliente));
    if (!novo_cl) {
        setColor(12);
        printf("\n       Erro ao alocar memória!\n");
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
            printf("Data inválida");
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

void buscar_cliente_por_cpf(struct cliente *head, const char *cpf) {
    struct cliente *atual = head;
    char cpf_limpo[12], cpf_atual_limpo[12];

    remover_formatacao_cpf(cpf, cpf_limpo);

    while (atual != NULL) {
        remover_formatacao_cpf(atual->cpf, cpf_atual_limpo);
        if (strcmp(cpf_atual_limpo, cpf_limpo) == 0) {
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
            return;
        }
        atual = atual->proximo_cl;
    }
}

// Função para buscar cliente (comum)
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
    return NULL; // Retorna NULL se o cliente não for encontrado
}

// Buscar cliente por CPF (sem exibir informações)
struct cliente* buscarCliente_por_cpf_sem_exibir(struct cliente *head, const char *cpf) {
    return buscarCliente(head, cpf, 0); // Chama buscarCliente com exibir_informacoes = 0
}

// Buscar cliente por CPF (exibindo informações)
struct cliente* buscarCliente_por_cpf_com_exibir(struct cliente *head, const char *cpf) {
    return buscarCliente(head, cpf, 1); // Chama buscarCliente com exibir_informacoes = 1
}

void menuCliente(struct cliente **head_cliente) {
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

