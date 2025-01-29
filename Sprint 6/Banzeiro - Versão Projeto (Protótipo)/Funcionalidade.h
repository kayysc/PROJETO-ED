#ifndef FUNCIONALIDADE_H
#define FUNCIONALIDADE_H

typedef struct Gerente{
    char nome[50];
    char usuario[20];
    char senha[20];
    int cod;
}Gerente;

typedef struct FuncionarioCadastro{
    char nome[50];
    char cpf[15];
    char telefone[20];
    char email[50];
    char conta_bancaria[11];  // Formato: "12345678-9"
    char usuario[20];
    char senha[20];
    int cod;
} FuncionarioCadastro;

FuncionarioCadastro funcionarios[100];
int num_funcionarios;

typedef struct FuncionarioCadastro FuncionarioCadastro ;
int mostrarMenuGerente(const char *titulo,const char **opcoes, int numOpcoes);

void cadastrarFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios);

void listarFuncionarios(FuncionarioCadastro *funcionarios, int num_funcionarios);
void salvarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int num_funcionarios);
int carregarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int *num_funcionarios);
void alterarFuncionario(FuncionarioCadastro *funcionarios, int num_funcionarios);
void excluirFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios);

void salvarGerente(const char *arquivo,Gerente *usuario);
int carregarGerente(const char *arquivo, Gerente *usuario);

#endif

