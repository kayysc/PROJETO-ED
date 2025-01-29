#include "Funcionalidade.h"
#include "Util.h"

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

FuncionarioCadastro funcionarios[100];



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
        printf("\n       Erro ao salvar os dados dos funcion�rios.\n");
        setColor(15);
    }
}
int carregarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    FILE *fp = fopen(arquivo, "rb");
    int i;
    if (fp) {
        fread(num_funcionarios, sizeof(int), 1, fp); // L� o n�mero de funcion�rios
        fread(funcionarios, sizeof(FuncionarioCadastro), *num_funcionarios, fp); // L� o array de funcion�rios

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

void cadastrarFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios) {
    if (*num_funcionarios >= 100) {
        printf("\n       Limite ");
        setColor(12);
        printf("m�ximo");
        setColor(15);
        printf(" de funcion�rios atingido!\n");
        return;
    }

    FuncionarioCadastro novo_func;

    printf("\n ============= CADASTRO FUNCION�RIO =============\n\n");
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

    // Define o c�digo do funcion�rio
    novo_func.cod = *num_funcionarios + 1;

    printf("       Usu�rio: ");
    fflush(stdin);
    fgets(novo_func.usuario, 20, stdin);
    strtok(novo_func.usuario, "\n");

    printf("       Senha: ");
    fflush(stdin);
    fgets(novo_func.senha, 20, stdin);
    strtok(novo_func.senha, "\n");

    funcionarios[*num_funcionarios] = novo_func;
    (*num_funcionarios)++;

    // Salva todos os funcion�rios ap�s o cadastro
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, *num_funcionarios);

    printf("\n       Parab�ns ");
    setColor(11);
    printf("%s", novo_func.nome);
    setColor(7);
    printf(", oficialmente contratado!!!\n");
    printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}


void listarFuncionarios(FuncionarioCadastro *funcionarios, int num_funcionarios) {
    system(CLEAR);
    int i;
    // Carrega todos os funcion�rios do arquivo "funcionarios.dat"
    carregarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, &num_funcionarios);

    if (num_funcionarios == 0) {
        printf("\n       Nenhum funcion�rio cadastrado.\n");
    } else {
        printf("\n ============= LISTA DE FUNCION�RIOS =============\n\n");
        for (i = 0; i < num_funcionarios; i++) {
            printf("       Nome: %s\n", funcionarios[i].nome);
            printf("       CPF: %s\n", funcionarios[i].cpf);
            printf("       Telefone: %s\n", funcionarios[i].telefone);
            printf("       Email: %s\n", funcionarios[i].email);
            printf("       Conta Banc�ria: %s\n", funcionarios[i].conta_bancaria);
            printf("       Usu�rio: ");
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
    printf("\n ============= ALTERAR FUNCION�RIO =============\n\n");
    printf("\n       Digite abaixo o CPF do funcion�rio que deseja alterar\n\n");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcion�rio pelo CPF
    for (i = 0; i < num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        setColor(12);
        printf("\n       Funcion�rio n�o encontrado.\n");
        setColor(15);
        printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Altera os dados do funcion�rio
    system("cls");
    printf("\n ============= ALTERAR FUNCION�RIO =============\n\n");
    printf("\n       Funcion�rio ");
    setColor(11);
    printf("%s", funcionarios[posicao].nome);
    setColor(15);
    printf(", foi encontrado!\n");
    printf("\n       Digite abaixo os novos dados do funcion�rio\n");

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

    printf("       Usu�rio: ");
    fflush(stdin);
    fgets(funcionarios[posicao].usuario, 20, stdin);
    strtok(funcionarios[posicao].usuario, "\n");

    printf("       Senha: ");
    fflush(stdin);
    fgets(funcionarios[posicao].senha, 20, stdin);
    strtok(funcionarios[posicao].senha, "\n");

    // Salva todos os funcion�rios ap�s a altera��o
    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);

    printf("\n       Dados do funcion�rio alterados com ");
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
    printf("\n ============= EXCLUIR FUNCION�RIO =============\n\n");
    printf("\n       Digite abaixo o CPF do funcion�rio que deseja excluir\n\n");
    ler_cpf_formatado(cpf_busca);

    // Busca o funcion�rio pelo CPF
    for (i = 0; i < *num_funcionarios; i++) {
        if (strcmp(funcionarios[i].cpf, cpf_busca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        setColor(12);
        printf("\n       Funcion�rio n�o encontrado.\n");
        setColor(15);
        printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
        getch();
        return;
    }

    // Confirma��o da exclus�o
    system("cls");
    printf("\n ============= EXCLUIR FUNCION�RIO =============\n\n");
    printf("\n       Tem certeza que deseja excluir o funcion�rio ");
    setColor(11);
    printf("%s", funcionarios[posicao].nome);
    setColor(15);
    printf("? (S/N): ");
    char confirmacao = getch();

    if (confirmacao == 'S' || confirmacao == 's') {
        // Remove o funcion�rio do array
        char nome_arquivo[50];
        sprintf(nome_arquivo, "%s.dat", funcionarios[posicao].usuario);

        remove(nome_arquivo);

        for (i = posicao; i < *num_funcionarios - 1; i++) {
            funcionarios[i] = funcionarios[i + 1];
        }
        (*num_funcionarios)--;

        // Salva todos os funcion�rios ap�s a exclus�o
        salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, *num_funcionarios);

        printf("\n\n       Funcion�rio exclu�do com ");
        setColor(10);
        printf("sucesso");
        setColor(15);
        printf("!\n");
    } else {
        printf("\n\n       Exclus�o cancelada!\n");
    }

    printf("\nPressione qualquer tecla para voltar ao menu principal.\n");
    getch();
}




