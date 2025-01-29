#include "auxiliar.h"

// Definição das variáveis funcionarios e num_funcionarios
extern FuncionarioCadastro funcionarios[100];  
extern int num_funcionarios;
int i,j;

void login() {
    setlocale(LC_ALL, "Portuguese");
    Gerente gerente;
    FuncionarioCadastro funcionario;
    char login[20], senha[20];
    int i = 0;
    int opcao;
    int posicao = 0;
    int tecla;

    do {
        system(CLEAR);
        printf("\n ============= BEM VINDO AO BANZEIRO!!! =============\n\n");

        if (posicao == 0) {
            setColor(14);
            printf("    -> Entrar\n");
        } else {
            setColor(15);
            printf("       Entrar\n");
        }
        if (posicao == 1) {
            setColor(12);
            printf("    -> Sair\n");
        } else {
            setColor(15);
            printf("       Sair\n");
        }
        setColor(15);

        tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            switch (tecla) {
                case 72:
                    if (posicao > 0) posicao--;
                    break;
                case 80:
                    if (posicao < 1) posicao++;
                    break;
            }
        } else if (tecla == 13) {
            opcao = posicao + 1;
        }

        if (opcao == 2) {
            system("cls");
            printf("\n =============== SAINDO DO SISTEMA... ===============\n\n");
            Sleep(2200);
            printf("\n       Programa encerrado\n\n");
            exit(0);
        }

        if (opcao == 1) {
            system(CLEAR);
            printf("\n ============= BEM VINDO AO BANZEIRO!!! =============\n\n");
            printf("       Usuário: ");
            scanf("%s", login);
            printf("       Senha: ");

            i = 0;
            while (1) {
                char ch = getch();
                if (ch == 13) {
                    senha[i] = '\0';
                    break;
                } else if (ch == 8) {
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                    }
                } else {
                    senha[i++] = ch;
                    printf("*");
                }
            }
            printf("\n");

            // Tenta carregar o usuário como gerente
            if (carregarGerente(login, &gerente)) {
                if (strcmp(gerente.senha, senha) == 0) {
                    system(CLEAR);
                    printf("\n ============= BEM VINDO(A), ");
                    setColor(11);
                    printf("%s", gerente.nome);
                    setColor(15);
                    printf("! =============\n\n");
                    printf("       Aguarde um momento...\n\n");
                    Sleep(4200);
                    animarBarco();
                    system("cls");
                    menuGerente();
                    return;
                } else {
                    setColor(12);
                    printf("\n       Senha incorreta!\n");
                    setColor(15);
                    Sleep(1400);
                    system("cls");
                }
            } else {
                // Se não for gerente, verifica se é funcionário
                for (i = 0; i < num_funcionarios; i++) {
                    if (strcmp(funcionarios[i].usuario, login) == 0 && strcmp(funcionarios[i].senha, senha) == 0) {
                        system(CLEAR);
                        printf("\n ============= BEM VINDO(A), ");
                        setColor(11);
                        printf("%s", funcionarios[i].nome);
                        setColor(15);
                        printf("! =============\n\n");
                        printf("       Aguarde um momento...\n\n");

                        // Registro da operação no histórico (login do funcionário)
                        char descricaoOperacao[200];
                        snprintf(descricaoOperacao, sizeof(descricaoOperacao), "Login realizado");
                        registrarOperacao(funcionarios[i].nome, descricaoOperacao); // Usando o nome do funcionário como 'usuario'

                        Sleep(4200);
                        animarBarco();
                        system("cls");
                        menuFuncionario();
                        return;
                    }
                }
                // Se o loop terminar sem encontrar o funcionário
                setColor(12);
                printf("\n       Usuário não encontrado ou senha incorreta!\n");
                Sleep(1400);
                setColor(15);
                system("cls");
            }
        }
    } while (opcao != 2);
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    Gerente gerente = {"Ana", "admin", "1234", 01};
    salvarGerente("admin", &gerente);
    struct cliente *clientes = NULL; // Inicializa o array de clientes
    int num_clientes = 0; // Inicializa o número de clientes
    carregarClientes("clientes.dat");

    if (!carregarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, &num_funcionarios)) {
        printf("\nNenhum funcionário encontrado. Inicializando com lista vazia.\n");
    }

    printf("\nNúmero de funcionários carregados: %d\n", num_funcionarios);
    carregarNotasFiscais("notasfiscais.dat");

    login();

    salvarTodosFuncionariosPersistente("funcionarios.dat", funcionarios, num_funcionarios);
    salvarNotasFiscais("notasfiscais.dat");
    salvarClientes("clientes.dat");

    return 0;
}
