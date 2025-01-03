#ifndef AUXILIAR_H
#define AUXILIAR_H

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

#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3
#define MAX_NOTAS 1

// Estruturas de dados
typedef struct {
    char nome[50];
    char usuario[20];
    char senha[20];
    int cod;
} Gerente;

typedef struct {
    char nome[50];
    char cpf[15];
    char telefone[20];
    char email[50];
    char conta_bancaria[11];
    char usuario[20];
    char senha[20];
    int cod;
} FuncionarioCadastro;
// Definição das variáveis funcionarios e num_funcionarios
extern FuncionarioCadastro funcionarios[100];  
extern int num_funcionarios; 

struct cliente {
    int id;
    char nome[50];
    char cpf[15];
    char data_nasc[11];
    char telefone[20];
    struct cliente *proximo_cl;
};
extern int prox_id;

typedef struct {
    char origem[30];
    char destino[30];
    char data[12];
    char horario[10];
    char preco[10];
    int linhas;
    int colunas;
    int capacidade;
} Rota;

Rota rotas[MAX_DESTINOS];

struct Nota {
    char nome[50];
    char cpf[15];
    int notaFiscal;
    char origem[30];
    char destino[30];
    char data[12];
    char horario[10];
    int preco;
    struct Nota *prox_Nota;
};
extern struct Nota *notasfiscais;

// Protótipos das funções
void setColor(int color);
void animarBarco();
void animacaoBarco();
void ler_cpf_formatado(char *cpf);
void remover_formatacao_cpf(const char *cpf_formatado, char *cpf_limpo);
void ler_telefone_formatado(char *telefone);
void ler_conta_bancaria_formatada(char *conta);
int calcular_idade(const char *data_nasc);
int validar_data(const char *data);
void ler_data_formatada(char *data);
void salvarGerente(const char *arquivo, Gerente *usuario);
int carregarGerente(const char *arquivo, Gerente *usuario);
void salvarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int num_funcionarios);
int carregarTodosFuncionariosPersistente(const char *arquivo, FuncionarioCadastro *funcionarios, int *num_funcionarios);
void cadastrarFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios);
void listarFuncionarios(FuncionarioCadastro *funcionarios, int num_funcionarios);
void alterarFuncionario(FuncionarioCadastro *funcionarios, int num_funcionarios);
void excluirFuncionario(FuncionarioCadastro *funcionarios, int *num_funcionarios);
void cadastrar_cliente(struct cliente **head);
void buscar_cliente_por_cpf(struct cliente *head, const char *cpf);
struct cliente* buscarCliente(struct cliente *head, const char *cpf, int exibir_informacoes);
struct cliente* buscarCliente_por_cpf_sem_exibir(struct cliente *head, const char *cpf);
struct cliente* buscarCliente_por_cpf_com_exibir(struct cliente *head, const char *cpf);
void inicializarAssentos(char ***assentos, int linhas, int colunas);
char ***criarAssentos(int linhas, int colunas);
void liberarAssentos(char ***assentos, int linhas, int colunas);
void mostrarAssentos(char ***assentos, int linhas, int colunas, int posLinha, int posColuna);
int escolherAssento(char ***assentos, int linhas, int colunas, char assentosEscolhidos[MAX_PASSAGENS][5], int *totalAssentosEscolhidos, Rota rota);
void geracaoNotaFiscal(Rota rota, int preco_total, const char *nome, const char *cpf);
void ListagemNota();
int mostrarMenuGerente(const char *titulo, const char **opcoes, int numOpcoes);
int mostrarMenu(const char *titulo, const char **opcoes, int numOpcoes);
void finalizarCompra(int totalAssentos, Rota rota, char assentosEscolhidos[MAX_PASSAGENS][5], const char *nome, const char *cpf, char ***assentos);
int detalhesDaCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota, char ***assentos, struct cliente **head_cliente);
void listarRotas(struct cliente **head_cliente);
void cancelarCompra();
void menuCliente(struct cliente **head_cliente);
void menuFuncionario();
void menuGerente();
void menuFuncionario();
void login();


#endif // AUXILIAR_H

