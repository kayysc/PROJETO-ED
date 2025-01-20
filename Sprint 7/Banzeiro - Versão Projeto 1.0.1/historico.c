#include "auxiliar.h"

static Registro *topo = NULL;

//obter a data e hora atual
void obterDataHoraAtual(char *buffer, int tamanho) {
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Fun��o para registrar uma opera��o 
void registrarOperacao(const char *usuario, const char *operacao) {
    // Alocar mem�ria para um novo registro
    Registro *novoRegistro = (Registro *)malloc(sizeof(Registro));
    if (novoRegistro == NULL) {
        printf("Erro ao alocar mem�ria para o registro!\n");
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

// Fun��o para exibir o hist�rico
void exibirHistorico() {
    printf("=== Hist�rico de Transa��es===\n");
    Registro *atual = topo;
    if (atual == NULL) {
        printf("Nenhuma transa��o registrada.\n");
        getch();
        return;
    }

    while (atual != NULL) {
        printf("[%s] Usu�rio: %s | Opera��o: %s\n",
                atual->dataHora, atual->usuario, atual->operacao);
        atual = atual->proximo;
    }
}
