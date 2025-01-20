#include "auxiliar.h"

int prox_id = 0;
struct cliente *clientes = NULL;
int num_clientes = 0;
char userLogado[50];

void salvarClientes(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "wb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para salvar os clientes.\n");
        return;
    }

    // Salva o número de clientes e os dados dos clientes
    fwrite(&num_clientes, sizeof(int), 1, fp);
    fwrite(clientes, sizeof(struct cliente), num_clientes, fp);

    fclose(fp);
}

void carregarClientes(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "rb");
    if (fp == NULL) {
        return; // Trata o erro de abertura do arquivo
    }

    // Lê o número de clientes
    fread(&num_clientes, sizeof(int), 1, fp);

    // Aloca memória para o array de clientes
    clientes = (struct cliente *)malloc(num_clientes * sizeof(struct cliente));
    if (clientes == NULL) {
        printf("Erro ao alocar memoria para os clientes.\n");
        exit(1);
    }

    // Lê os dados dos clientes do arquivo
    fread(clientes, sizeof(struct cliente), num_clientes, fp);

    fclose(fp);
}

void cadastrar_cliente() {
    if (num_clientes >= 100) {
        setColor(12);
        printf("\n        Limite maximo de clientes atingido!\n");
        setColor(15);
        return;
    }

    system(CLEAR);
    printf("\n ============= CADASTRAR CLIENTE =============\n\n");

    // Realoca memória para o array de clientes, incluindo o novo cliente
    struct cliente *temp = (struct cliente *)realloc(clientes, (num_clientes + 1) * sizeof(struct cliente));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para os clientes.\n");
        exit(1);
    }
    clientes = temp; 

    struct cliente novo_cl;
    novo_cl.id = prox_id++;

    printf("       Nome: ");
    fflush(stdin);
    fgets(novo_cl.nome, 50, stdin);
    strtok(novo_cl.nome, "\n");

    do {
        ler_cpf_formatado(novo_cl.cpf);
    } while (strlen(novo_cl.cpf) != 14);

    do {
        ler_data_formatada(novo_cl.data_nasc);
        if (!validar_data(novo_cl.data_nasc)) {
            printf("\n        ");
            setColor(12);
            printf("Data invalida");
            setColor(15);
            printf("! Tente novamente.\n");
        }
    } while (!validar_data(novo_cl.data_nasc));

    do {
        ler_telefone_formatado(novo_cl.telefone);
    } while (strlen(novo_cl.telefone) != 15);

    clientes[num_clientes] = novo_cl;
    num_clientes++;

    // Salva os clientes após o cadastro
    salvarClientes("clientes.dat"); 

    printf("\n        Cliente cadastrado com ");
    setColor(10);
    printf("sucesso");
    setColor(15);
    printf("!\n");

    // Registro da operação no histórico
    char descricaoOperacao[350];
    snprintf(descricaoOperacao, sizeof(descricaoOperacao), "Cadastro do cliente: %s (CPF: %s)", novo_cl.nome, novo_cl.cpf);
    registrarOperacao(userLogado, descricaoOperacao);

    getch();
}

// Função para buscar cliente por CPF
void buscar_cliente_por_cpf(const char *cpf) {
    char cpf_limpo[12], cpf_atual_limpo[12];
    remover_formatacao_cpf(cpf, cpf_limpo);

    for (int i = 0; i < num_clientes; i++) {
        remover_formatacao_cpf(clientes[i].cpf, cpf_atual_limpo);
        if (strcmp(cpf_atual_limpo, cpf_limpo) == 0) {
            printf("\n        Cliente encontrado:\n");
            printf("        ID: ");
            setColor(9);
            printf("%d\n", clientes[i].id);
            setColor(15);
            printf("        Nome: %s\n", clientes[i].nome);
            printf("        CPF: %s\n", clientes[i].cpf);
            printf("        Data de Nascimento: %s\n", clientes[i].data_nasc);
            printf("        Idade: %d anos\n", calcular_idade(clientes[i].data_nasc));
            printf("        Telefone: %s\n", clientes[i].telefone);
            return;
        }
    }

    printf("\n        Cliente nao encontrado.\n");
}

// Função para listar clientes
void listarClientes() {
    system(CLEAR);
    if (num_clientes == 0) {
        printf("\n        Nenhum cliente cadastrado.\n");
    } else {
        printf("\n ============= LISTA DE CLIENTES =============\n\n");
        for (int i = 0; i < num_clientes; i++) {
            printf("        ID: %d\n", clientes[i].id);
            printf("        Nome: %s\n", clientes[i].nome);
            printf("        CPF: %s\n", clientes[i].cpf);
            printf("        Data de Nascimento: %s\n", clientes[i].data_nasc);
            printf("        Idade: %d anos\n", calcular_idade(clientes[i].data_nasc));
            printf("        Telefone: %s\n", clientes[i].telefone);
            printf("------------------------------------------------\n");
        }
    }
    printf("\nPressione qualquer tecla para voltar ao menu.\n");
    getch();
}

// Função para buscar cliente (comum)
int buscarCliente(const char *cpf, int exibir_informacoes) {
    char cpf_limpo[12], cpf_atual_limpo[12];

    remover_formatacao_cpf(cpf, cpf_limpo);

    for (int i = 0; i < num_clientes; i++) {
        remover_formatacao_cpf(clientes[i].cpf, cpf_atual_limpo);
        if (strcmp(cpf_atual_limpo, cpf_limpo) == 0) {
            if (exibir_informacoes) {
                printf("\n        Cliente encontrado:\n");
                printf("        ID: ");
                setColor(9);
                printf("%d\n", clientes[i].id);
                setColor(15);
                printf("        Nome: %s\n", clientes[i].nome);
                printf("        CPF: %s\n", clientes[i].cpf);
                printf("        Data de Nascimento: %s\n", clientes[i].data_nasc);
                printf("        Idade: %d anos\n", calcular_idade(clientes[i].data_nasc));
                printf("        Telefone: %s\n", clientes[i].telefone);
            }
            return i; // Retorna o índice do cliente encontrado
        }
    }
    return -1; // Retorna -1 se o cliente não for encontrado
}

// Buscar cliente por CPF (sem exibir informações)
int buscarCliente_por_cpf_sem_exibir(const char *cpf) {
    return buscarCliente(cpf, 0);
}

// Buscar cliente por CPF (exibindo informações)
int buscarCliente_por_cpf_com_exibir(const char *cpf) {
    return buscarCliente(cpf, 1);
}

void menuCliente(char ***assentos, int linhas, int colunas) {
    const char *opcoes[] = {
        "Cadastrar cliente",
        "Listagem dos clientes", // Nova opção
        "Buscar cliente por CPF",
        "Cancelamento de Compra",
        "Voltar"
    };
    int escolha;
    char cpf_busca[15];

    do {
        escolha = mostrarMenu("ÁREA DO CLIENTE", opcoes, 5); // 5 opções agora
        switch (escolha) {
            case 0:
                system(CLEAR);
                printf("\n ============= CADASTRAR CLIENTE =============\n\n");
                cadastrar_cliente();
                break;
            case 1:
                system(CLEAR);
                listarClientes(); // Chama a função listarClientes
                break;
            case 2:
                system(CLEAR);
                printf("\n ============= BUSCAR CLIENTE POR CPF =============\n\n");
                printf("        Digite o CPF do cliente: ");
                printf("\n");
                ler_cpf_formatado(cpf_busca);
                buscarCliente_por_cpf_com_exibir(cpf_busca);
                printf("\n        Pressione qualquer tecla para continuar...");
                getch();
                break;
            case 3:
                system(CLEAR);
                cancelarCompra(assentos, linhas, colunas);
                break;
            case 4:
                printf("Voltando ao menu anterior...");
                return;
        }
    } while (1);
}
