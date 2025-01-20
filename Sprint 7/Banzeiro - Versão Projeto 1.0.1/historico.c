#include "auxiliar.h"

static Registro *topo = NULL;

//obter a data e hora atual
void obterDataHoraAtual(char *buffer, int tamanho) {
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Função para registrar uma operação 
void registrarOperacao(const char *usuario, const char *operacao) {
    // Alocar memória para um novo registro
    Registro *novoRegistro = (Registro *)malloc(sizeof(Registro));
    if (novoRegistro == NULL) {
        printf("Erro ao alocar memória para o registro!\n");
        return;
    }

    // Preenche os dados do novo registro
    strncpy(novoRegistro->usuario, usuario, sizeof(novoRegistro->usuario) - 1);
    novoRegistro->usuario[sizeof(novoRegistro->usuario) - 1] = '\0'; // Certifique-se de que a string esteja terminada em nulo
    strncpy(novoRegistro->operacao, operacao, sizeof(novoRegistro->operacao) - 1);
    novoRegistro->operacao[sizeof(novoRegistro->operacao) - 1] = '\0'; // Certifique-se de que a string esteja terminada em nulo
    obterDataHoraAtual(novoRegistro->dataHora, sizeof(novoRegistro->dataHora));
    novoRegistro->proximo = topo; 

    topo = novoRegistro;
}

// Função para exibir o histórico
void exibirHistorico() {
    printf("=== Histórico de Transações===\n");
    Registro *atual = topo;
    if (atual == NULL) {
        printf("Nenhuma transação registrada.\n");
        getch();
        return;
    }

    while (atual != NULL) {
        printf("[%s] Usuário: %s | Operação: %s\n",
                atual->dataHora, atual->usuario, atual->operacao);
        atual = atual->proximo;
    }
}
