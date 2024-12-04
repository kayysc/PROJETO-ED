#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>  
#include <windows.h> 
#include <conio.h> 


#ifdef _WIN32
    #define CLEAR "cls"  // Comando completo para limpar tela no Windows, else est� para linux e macOs, junto ao 
    #define SLEEP(ms) Sleep(ms)
#else
    #define CLEAR "clear"
    #define SLEEP(ms) usleep(ms * 1000)
#endif

#define LINHAS 7
#define COLUNAS 4
#define LETRAS "ABCDEFG"
#define MAX_DESTINOS 4
#define MAX_CLIENTE 3
#define MAX_PASSAGENS 3
#define LETRAS "ABCDEFGH"

typedef struct {
	char nome[50], trajet[20], tipo[100];
 	int num, cabin;
 	float preco;

} Barco;

typedef struct {
    char rota[50];
    char horario[10];
    char preco[50];
}barco;

int prox_id= 1;

struct endereco{

	char cep[9];
	char rua[50];
	char numero[10];
	char bairro[50];
	char cidade[50];
	char estado[50];	
}; 

struct cliente{
	
	int id;
	char nome[50];
	char cpf[11];
	char data_nasc[20];
	struct endereco end;
	char email[50];
	char telefone[20];
	struct cliente *proximo_cl;// aponta para um novo cliente, permitindo uma lista encadeada
};

struct embarcacao{
	char nome[50];
	char tipo[50];
	int capacidade;
	struct embarcacao *proximo_em;//aponta para uma nova embarcaco
};

typedef struct {
    char nome[50];
    char usuario[20];
    char senha[20];
    char cargo[20];  // Ex.: "Gerente - geral, interino, " ou "Funcion�rio"
    int cod;
	int cpf;
} Funcionario;

// Estrutura de rota (com origem e data)
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

barco Barcos[MAX_DESTINOS] = {
    {"Manaus -> Tabatinga", "08:00", "R$200"},  // Declara��o dos barcos
    {"Manaus -> Bel�m", "10:30", "R$150"}, 
    {"Manaus -> Santar�m", "10:00", "R$250"},
    {"Manaus -> Parintins", "11:00", "R$130"}};
    
    Rota rotas[MAX_DESTINOS] = {
    {"Manaus", "Tabatinga", "25/11/2024", "08:00", "R$200", 5, 4, 20},
    {"Manaus", "Bel�m", "26/11/2024", "10:30", "R$150", 7, 4, 28},
    {"Manaus", "Santar�m", "27/11/2024", "10:00", "R$250", 6, 4, 24},
    {"Manaus", "Parintins", "28/11/2024", "11:00", "R$130", 8, 4, 32}
};

// Fun��o para alterar a cor no console
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Fun��es para gerenciar assentos
void inicializarAssentos(char ***assentos, int linhas, int colunas) {
	int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            sprintf(assentos[i][j], "%c-%d", LETRAS[i], j + 1);
        }
    }
}

char ***criarAssentos(int linhas, int colunas) {
    char ***assentos = (char ***)malloc(linhas * sizeof(char **));
    int i, j;
    for (i = 0; i < linhas; i++) {
        assentos[i] = (char **)malloc(colunas * sizeof(char *));
        for (j = 0; j < colunas; j++) {
            assentos[i][j] = (char *)malloc(4 * sizeof(char));
        }
    }
    inicializarAssentos(assentos, linhas, colunas);
    return assentos;
}

void liberarAssentos(char ***assentos, int linhas, int colunas) {
	int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            free(assentos[i][j]);
        }
        free(assentos[i]);
    }
    free(assentos);
}

// Fun��o para exibir os assentos
void mostrarAssentos(char ***assentos, int linhas, int colunas, int posLinha, int posColuna) {
	int i, j;
	
    printf("\n======== MAPA DE ASSENTOS ========\n");
    printf("      [1]  [2]   |   [3]  [4]  \n");
    printf("     /-----------------------\\\n");

    for (i = 0; i < linhas; i++) {
        printf(" [%c] ", LETRAS[i]);
        for (j = 0; j < colunas; j++) {
            if (j == 2) {
                printf("  |  ");
            }

            if (strcmp(assentos[i][j], "X") == 0) {
                setColor(12); // Vermelho para ocupados
                printf(" X   ");
            } else if (i == posLinha && j == posColuna) {
                setColor(14); // Amarelo para o assento selecionado
                printf(" %-4s", assentos[i][j]);
            } else {
                setColor(10); // Verde para dispon�veis
                printf(" %-4s", assentos[i][j]);
            }
            setColor(7); // Reset para texto padr�o
        }
        printf("\n");
    }
    printf("     \\-----------------------/\n");
    printf("==================================\n");
}

// Sele��o de assentos com navega��o
int escolherAssento(char ***assentos, int linhas, int colunas, char assentosEscolhidos[MAX_PASSAGENS][5], int *totalAssentosEscolhidos, Rota rota) {
    int posLinha = 0, posColuna = 0;

    while (*totalAssentosEscolhidos < MAX_PASSAGENS) {
        system(CLEAR);

        // Exibindo as informa��es do barco
        printf("Informa��es do Barco:\n");
        printf("Rota: %s -> %s\n", rota.origem, rota.destino);
        printf("Data: %s\n", rota.data);
        printf("Hor�rio: %s\n", rota.horario);
        printf("Pre�o: %s\n", rota.preco);
        printf("Capacidade: %d assentos\n", rota.capacidade);
        printf("\n");

        mostrarAssentos(assentos, linhas, colunas, posLinha, posColuna);

        printf("\nUse as setas para navegar e pressione Enter para confirmar o assento.\n");
        printf("Pressione 'f' para finalizar a sele��o, 'v' para voltar ao menu ou 'q' para sair.\n");

        int tecla = _getch();

        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posLinha > 0) posLinha--;
                    break;
                case 80: // Seta para baixo
                    if (posLinha < linhas - 1) posLinha++;
                    break;
                case 75: // Seta para esquerda
                    if (posColuna > 0) posColuna--;
                    break;
                case 77: // Seta para direita
                    if (posColuna < colunas - 1) posColuna++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            if (strcmp(assentos[posLinha][posColuna], "X") == 0) {
                printf("Assento j� ocupado! Tente novamente.\n");
                _getch();
            } else {
                strcpy(assentos[posLinha][posColuna], "X");
                snprintf(assentosEscolhidos[*totalAssentosEscolhidos], 5, "%c-%d", LETRAS[posLinha], posColuna + 1);
                (*totalAssentosEscolhidos)++;
                printf("Assento %c-%d selecionado com sucesso!\n", LETRAS[posLinha], posColuna + 1);
                _getch();
            }
        } else if (tecla == 'f' || tecla == 'F') { // Finalizar sele��o
            break;
        } else if (tecla == 'v' || tecla == 'V') { // Voltar
            return -1;
        } else if (tecla == 'q' || tecla == 'Q') { // Sair
            return 0;
        }
    }
    return 1;
}

// Fun��o para comparar pre�os de rotas (para ordena��o)
int compararPorPreco(const void *a, const void *b) {
    Rota *rotaA = (Rota *)a;
    Rota *rotaB = (Rota *)b;
    return strcmp(rotaA->preco, rotaB->preco);
}

// Fun��o para comparar capacidade de rotas (para ordena��o)
int compararPorCapacidade(const void *a, const void *b) {
    Rota *rotaA = (Rota *)a;
    Rota *rotaB = (Rota *)b;
    return rotaB->capacidade - rotaA->capacidade;
}

// Confirma��o e gera��o de comprovante
int confirmarCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota) {
	int i, j;
	
    system(CLEAR);
    printf("======= CONFIRMAR COMPRA =======\n");
    printf("Rota: %s -> %s\n", rota.origem, rota.destino);
    printf("Data: %s\n", rota.data);
    printf("Hor�rio: %s\n", rota.horario);
    printf("Pre�o: %s\n", rota.preco);
    printf("Assentos Selecionados:\n");
    for (i = 0; i < totalAssentos; i++) {
        printf("- %s\n", assentosEscolhidos[i]);
    }
    printf("===============================\n");
    printf("Deseja confirmar a compra? (S/N): ");

    char resposta;
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        system(CLEAR);
        printf("Obrigado por escolher o Banzeiro. Vamos navegar juntos para o futuro? Boa Viagem!!!\n");
        _getch();
        exit(1);
    } else {
        system(CLEAR);
        printf("Compra cancelada. Retornando ao menu principal.\n");
        _getch();
        return 0;
    }
}

// Fun��o para filtrar rotas
void filtrarRotas() {
	int i, j;
	
    system(CLEAR);
    printf("======= FILTRAR ROTAS =======\n");
    printf("1. Destinos Populares\n");
    printf("2. Destinos Acess�veis\n");
    printf("Escolha uma op��o: ");

    int opcaoFiltro;
    scanf("%d", &opcaoFiltro);

    switch (opcaoFiltro) {
        case 1: // Destinos Populares
            system(CLEAR);
            qsort(rotas, MAX_DESTINOS, sizeof(Rota), compararPorCapacidade);
            printf("\nRotas ordenadas por popularidade (capacidade):\n");
            break;
        case 2: // Destinos Acess�veis
            system(CLEAR);
            qsort(rotas, MAX_DESTINOS, sizeof(Rota), compararPorPreco);
            printf("\nRotas ordenadas por pre�o (mais acess�veis primeiro):\n");
            break;
        default:
            system(CLEAR);
            printf("Op��o inv�lida!\n");
            return;
    }

    // Exibir as rotas ordenadas
    for (i = 0; i < MAX_DESTINOS; i++) {
        printf("%d. %s -> %s - %s - %s - %s\n", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario, rotas[i].preco);
    }

    printf("\nEscolha uma rota pelo n�mero ou pressione 0 para voltar ao menu principal: ");
    int escolha;
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Volta ao menu principal
    } else if (escolha < 1 || escolha > MAX_DESTINOS) {
        printf("Op��o inv�lida! Retornando ao menu principal...\n");
        _getch();
        return;
    }

    // Rota selecionada
    Rota rotaSelecionada = rotas[escolha - 1];
    char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
    char assentosEscolhidos[MAX_PASSAGENS][5];
    int totalAssentosEscolhidos = 0;

    // Navega��o para sele��o de assentos
    int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);
    if (resultado == 1 && totalAssentosEscolhidos > 0) {
        if (confirmarCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada)) {
            liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
        }
    } else {
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }
}

void listarRotas() {
	int i, j;
	
    system(CLEAR);
    printf("============== MENU DE ROTAS ==============\n");
    for (i = 0; i < MAX_DESTINOS; i++) {
        printf("%d. %s -> %s - %s - %s\n", i + 1, rotas[i].origem, rotas[i].destino, rotas[i].data, rotas[i].horario);
        printf("===========================================\n");
    }

    int rotaEscolhida;
    printf("\nDigite o n�mero da rota desejada: ");
    scanf("%d", &rotaEscolhida);

    // Valida��o de entrada
    if (rotaEscolhida < 1 || rotaEscolhida > MAX_DESTINOS) {
        printf("Rota inv�lida! Retornando ao menu principal.\n");
        _getch();
        return;
    }

    Rota rotaSelecionada = rotas[rotaEscolhida - 1];
    char ***assentos = criarAssentos(rotaSelecionada.linhas, rotaSelecionada.colunas);
    char assentosEscolhidos[MAX_PASSAGENS][5];
    int totalAssentosEscolhidos = 0;

    int resultado = escolherAssento(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas, assentosEscolhidos, &totalAssentosEscolhidos, rotaSelecionada);

    if (resultado == 1 && totalAssentosEscolhidos > 0) {
        int confirmacao = confirmarCompra(assentosEscolhidos, totalAssentosEscolhidos, rotaSelecionada);
        if (confirmacao == 1) {
            printf("Compra confirmada! Obrigado.\n");
            liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
            return; // Sai da fun��o ap�s compra
        } else {
            printf("Compra cancelada. Retornando ao menu principal...\n");
            _getch();
            liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
        }
    } else {
        liberarAssentos(assentos, rotaSelecionada.linhas, rotaSelecionada.colunas);
    }
}

void animacaoBarco() {
	const char *frames[] = {
    "\n\n"
	"   ~     ~ \n"
    "      ~    ~   \n"
    "    ~    ~    ~   ~   ~ \n"
    "        ~      \n",
    "   ~  \n"
    "      ~    ~ \n"
    "    ~    ~\n"
    "       \n",
    "   ~     ~    ~\n"
    "      ~    ~      ~    ~\n"
    "    ~    ~    ~   ~   ~   ~\n"
    "        ~      ~     ~\n",
	
	};

	 int i;
	for ( i = 0; i < 10; i++) {
        system(CLEAR);
        printf("%s", frames[i % 3]);
        fflush(stdout);  // Garante que o frame seja exibido
        SLEEP(300);  // Espera 300ms entre os frames
	}
		printf("\nBarco pronto para embarque!\n");
		SLEEP(1500);
}

void salvarFuncionario(const char *arquivo, Funcionario *usuario) {
    FILE *fp = fopen(arquivo, "wb");
    if (fp) {
        fwrite(usuario, sizeof(Funcionario), 1, fp);
        fclose(fp);
    }
}

int carregarFuncionario(const char *arquivo, Funcionario *usuario) {
    FILE *fp = fopen(arquivo, "rb");
    if (fp) {
        fread(usuario, sizeof(Funcionario), 1, fp);
        fclose(fp);
        return 1;  // Sucesso
    }
    return 0;  // Falha
}

Funcionario* cadas_funcis(Funcionario *funcis, int *n){
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	(*n)++;
    funcis = (Funcionario*)realloc(funcis, (*n) * sizeof(Funcionario));
    if (funcis == NULL) {
        printf("Erro ao alocar mem�ria\n");
        exit(1);
    }

    Funcionario novo_funcis;
    novo_funcis.cod = *n;
    
	printf("\n ========== Cadastrar Funcionario ==========\n\n");
	printf(" ________________________________________\n");
	printf("|                                        \n");
	printf("|  Digite nome do funcionario: ");
	getchar();
	fflush(stdin);
	fgets(novo_funcis.nome, 50, stdin);
	novo_funcis.nome[strcspn(novo_funcis.nome, "\n")] = 0;
	printf("|  Digite CPF do funcionario: ");
	scanf("%d", &novo_funcis.cpf);
	printf("|  Digite funcao do funcionario: ");
	getchar();
	fflush(stdin);
	fgets(novo_funcis.cargo, 20, stdin);
	novo_funcis.cargo[strcspn(novo_funcis.cargo, "\n")] = 0;
	printf("|                                        \n");
	printf("|________________________________________\n");
	funcis[(*n) - 1] = novo_funcis;
	printf("\n\t\tCadastrado\n");
	printf("\nPressione qualquer tecla para continuar");
	system("pause > nul");
	system("CLS");
 						
	return funcis;
}

void ver_funcis(Funcionario *funcis, int n) {
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	int i;
	
	printf("\n ============= Ver Funcionarios =============\n\n");
	
    if (n == 0) {
        printf("\n\tNenhum cliente cadastrado.\n");
        printf("\nPressione qualquer tecla para continuar");
		system("pause > nul");
		system("CLS");
        return;
    }
    
    for (i = 0; i < n; i++) {
		printf(" ________________________________________\n");
		printf("|                                        \n");
		printf("|  Nome do funcionario: %s\n", funcis[i].nome);
		printf("|  CPF do funcionario: %d\n", funcis[i].cpf);
		printf("|  Codigo do funcionario: %d\n", funcis[i].cod);
		printf("|  Funcao do funcionario: %s\n", funcis[i].cargo);
		printf("|  Login do funcionario: %s\n", funcis[i].usuario);
		printf("|  Senha do funcionario: %s\n", funcis[i].senha);
		printf("|________________________________________\n");	   
    }
    
    printf("\nPressione qualquer tecla para continuar");
	system("pause > nul");
	system("CLS");
}

void atual_funcis(Funcionario *funcis, int n) {
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	int cod, i, cpf;
	
	printf("\n ========== Atualizar Funcionario ==========\n\n");
	
    if (n == 0) {
        printf("\n\tNenhum cliente cadastrado.\n");
        printf("\nPressione qualquer tecla para continuar");
		system("pause > nul");
		system("CLS");
        return;
    }
    
    printf("\nDigite o codigo do funcionario que deseja atualizar: \n");
    scanf("%d", &cod);
    printf("\n\nDigite o CPF do funcionario que deseja atualizar: \n");
    scanf("%d", &cpf);

    for (i = 0; i < n; i++) {
        if (funcis[i].cod == cod || funcis[i].cpf == cpf) {
            printf("\nAtualizando dados do funcionario com codigo %d...\n", cod);
            printf(" ________________________________________\n");
			printf("|                                        \n");
			printf("|  Atualize o nome do funcionario: ");
			getchar();
			fflush(stdin);
			fgets(funcis[i].nome, 50, stdin);
			funcis[i].nome[strcspn(funcis[i].nome, "\n")] = 0;
			printf("|  Atualize o CPF do funcionario: ");
			scanf("%d", &funcis[i].cpf);
			printf("|  Atualize a funcao do funcionario: ");
			getchar();
			fflush(stdin);
			fgets(funcis[i].cargo, 20, stdin);
			funcis[i].cargo[strcspn(funcis[i].cargo, "\n")] = 0;
			printf("|                                        \n");
			printf("|      Dados atualizados com sucesso!\n");
			printf("|________________________________________\n");
			printf("\n\t\tAtualizado!\n");
			printf("\nPressione qualquer tecla para continuar");
 			system("pause > nul");
 			system("CLS");
	
            return;
        }
    }
    printf("Funcionario com codigo %d n�o encontrado.\n", cod);
    printf("\nPressione qualquer tecla para continuar");
 	system("pause > nul");
 	system("CLS");
}

Funcionario* demit_funcis(Funcionario *funcis, int *n) {
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	int cod, i, j, cpf;
	
	printf("\n =========== Demitir Funcionarios ===========\n\n");
	
    if (*n == 0) {
        printf("\n\tNenhum funcionario cadastrado.\n");
        printf("\nPressione qualquer tecla para continuar");
		system("pause > nul");
		system("CLS");
        return funcis;
    }

    printf(" ________________________________________\n");
	printf("|                                        \n");
 	printf("|  Digite o codigo do funcionario que deseja demitir: ");
 	scanf("%d", &cod);
	printf("|  Digite o CPF do funcionario que deseja demitir: ");
 	scanf("%d", &cpf);
	
	
	
    for (i = 0; i < *n; i++) {
        if (funcis[i].cod == cod || funcis[i].cpf == cpf) {
        	printf("|  Demitindo funcionario com codigo %d e CPF %d...\n", cod, cpf);
        	
			for (j = i; j < *n - 1; j++) {
                funcis[j] = funcis[j + 1];
            }
            (*n)--;
            
            funcis = (Funcionario*)realloc(funcis, (*n) * sizeof(Funcionario));
			printf("|                                        \n");
			printf("|  Dados retirados com sucesso!\n");
			printf("|________________________________________\n");
			
			printf("\n\t\tRetirado!\n");
			
            printf("\n\tFuncionario demitido com sucesso!\n\n");
            printf("\nPressione qualquer tecla para continuar");
			system("pause > nul");
			system("CLS");
            
            return funcis;
        }
    }
    printf("\n\tFuncionario com codigo %d n�o encontrado.\n", cod);
    
    printf("\nPressione qualquer tecla para continuar");
	system("pause > nul");
	system("CLS");
	
    return funcis;
}

Barco* inser_barcos(Barco *barcos, int *m){
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	(*m)++;
    barcos = (Barco*)realloc(barcos, (*m) * sizeof(Barco));
    if (barcos == NULL) {
        printf("Erro ao alocar mem�ria\n");
        exit(1);
    }

    Barco novo_barcos;
    novo_barcos.num = *m;
	printf("\n ============== Inserir Barco ==============\n\n");
	printf(" ________________________________________\n");
	printf("|                                        \n");
	printf("|  Digite o nome do barco: ");
	getchar();
	fgets(novo_barcos.nome, 30, stdin);
	novo_barcos.nome[strcspn(novo_barcos.nome, "\n")] = 0;
	printf("|  Digite a quatidades de cabines: ");
	scanf("%d", &novo_barcos.cabin);
	printf("|  Digite o trajeto do barco: ");
	getchar();
	fgets(novo_barcos.trajet, 50, stdin);
	novo_barcos.trajet[strcspn(novo_barcos.trajet, "\n")] = 0;
	printf("|  Digite o valor das passagens do barco: ");
	scanf("%f", &novo_barcos.preco);
	printf("|  Digite o tipo do barco: ");
	getchar();
	fgets(novo_barcos.tipo, 100, stdin);
	novo_barcos.tipo[strcspn(novo_barcos.tipo, "\n")] = 0;
	printf("|                                        \n");
	printf("|________________________________________\n");
	barcos[(*m) - 1] = novo_barcos;
	printf("\n\t\tCadastrado\n");
	printf("\nPressione qualquer tecla para continuar");
	system("pause > nul");
	system("CLS");
 						
	return barcos;
}

void ver_barcos(Barco *barcos, int m) {
	setlocale(LC_ALL, "Portuguese");
	int i;
	system("cls");
	printf("\n ================ Ver Barcos ================\n\n");
	
    if (m == 0) {
        printf("\n\tNenhum barco cadastrado.\n");
        printf("\nPressione qualquer tecla para continuar");
		system("pause > nul");
		system("CLS");
        return;
    }
    
    for (i = 0; i < m; i++) {
		printf(" ________________________________________\n");
		printf("|                                        \n");
		printf("|  Nome do barco: %s\n", barcos[i].nome);
		printf("|  Trajeto do barco: %s\n", barcos[i].trajet);
		printf("|  Tipo do barco: %s\n", barcos[i].tipo);
		printf("|  Numero de cabines do barco: %d\n", barcos[i].cabin);
		printf("|  Numero do barco: %d\n", barcos[i].num);
		printf("|  Preco das passagens do barco: R$ %.2f\n", barcos[i].preco);
		printf("|________________________________________\n");	   
    }
    
    printf("\nPressione qualquer tecla para continuar");
	system("pause > nul");
	system("CLS");
}

void atual_barcos(Barco *barcos, int m) {
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	int num, i;
	
	printf("\n ============= Atualizar Barco =============\n\n");
	
    if (m == 0) {
        printf("\n\tNenhum barco cadastrado.\n");
        printf("\nPressione qualquer tecla para continuar");
		system("pause > nul");
		system("CLS");
        return;
    }
    
    printf("\nDigite o numero do barco que deseja atualizar: ");
    scanf("%d", &num);

    for (i = 0; i < m; i++) {
        if (barcos[i].num == num) {
            printf("\nAtualizando dados do funcionario com codigo %d...\n", num);
            printf(" ________________________________________\n");
			printf("|                                        \n");
			printf("|  Atualize o nome do barco: ");
			getchar();
			fgets(barcos[i].nome, 30, stdin);
			barcos[i].nome[strcspn(barcos[i].nome, "\n")] = 0;
			printf("|  Atualize a quatidades de cabines: ");
			scanf("%d", &barcos[i].cabin);
			printf("|  Atualize o trajeto do barco: ");
			getchar();
			fgets(barcos[i].trajet, 50, stdin);
			barcos[i].trajet[strcspn(barcos[i].trajet, "\n")] = 0;
			printf("|  Atualize o valor das passagens do barco: ");
			scanf("%f", &barcos[i].preco);
			printf("|  Atualize o tipo do barco: ");
			getchar();
			fgets(barcos[i].tipo, 100, stdin);
			barcos[i].tipo[strcspn(barcos[i].tipo, "\n")] = 0;
			printf("|                                        \n");
			printf("|    Dados atualizados com sucesso!\n");
			printf("|________________________________________\n");
			printf("\n\t\tAtualizado!\n");
			printf("\nPressione qualquer tecla para continuar");
 			system("pause > nul");
 			system("CLS");
	
            return;
        }
    }
    printf("\n\tBarco com nummero %d n�o encontrado.\n", num);
    printf("\nPressione qualquer tecla para continuar");
 	system("pause > nul");
 	system("CLS");
}

Barco* deletar_barcos(Barco *barcos, int *m) {
	system("cls");
	setlocale(LC_ALL, "Portuguese");
	int num, i, j;
	
	printf("\n ============== Excluir Barco ==============\n\n");
	
    if (*m == 0) {
        printf("\n\tNenhum barco cadastrado.\n");
        printf("\nPressione qualquer tecla para continuar");
		system("pause > nul");
		system("CLS");
        return barcos;
    }

    printf(" ________________________________________\n");
	printf("|                                        \n");
 	printf("|  Digite o numero do barco que deseja retirar: ");
 	scanf("%d", &num);

    for (i = 0; i < *m; i++) {
        if (barcos[i].num == num) {
        	printf("|  Retirando barco com codigo %d...\n", num);
        	
			for (j = i; j < *m - 1; j++) {
                barcos[j] = barcos[j + 1];
            }
            (*m)--;
            
            barcos = (Barco*)realloc(barcos, (*m) * sizeof(Barco));
			printf("|                                        \n");
			printf("|  Dados retirados com sucesso!\n");
			printf("|________________________________________\n");
			
			printf("\n\t\tRetirado!\n");
			
            printf("\n\tFuncionario demitido com sucesso!\n\n");
            printf("\nPressione qualquer tecla para continuar");
			system("pause > nul");
			system("CLS");
            
            return barcos;
        }
    }
    printf("\n\tBarco com numero %d n�o encontrado.\n", num);
    
    printf("\nPressione qualquer tecla para continuar");
	system("pause > nul");
	system("CLS");
	
    return barcos;
}

void recuperarSenha() {
	setlocale(LC_ALL, "Portuguese");
    Funcionario usuario;
    char login[20];

    printf("\n =========== Recupera��o de Senha ===========\n\n");
    printf("Informe o usu�rio: ");
    scanf("%s", login);

    if (carregarFuncionario(login, &usuario)) {
        printf("\n\tSenha do %s (%s): %s\n\n", usuario.cargo, usuario.nome, usuario.senha);
        system("pause");
        system("cls");
    } else {
        printf("\n\tUsu�rio n�o encontrado!\n\n");
        system("pause");
        system("cls");
    }
}

void gerenciar(char usuario[]){
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("BANZEIRO - GERENTE");
	Barco *barcos = NULL;
	struct cliente *head_cliente= NULL;
	Funcionario *funcis = NULL;
	int n = 0, m = 0, opcao=0;
	
	do{
		printf("\n =============== Menu %s ===============\n\n", usuario);
		printf("0. Sair\n");
		printf("1. Atualizar %s\n", usuario);
		printf("2. Excluir %s\n", usuario);
		scanf("%d", &opcao);
			
		if(strcmp(usuario, "funcionario") == 0){
			switch(opcao){
				case 1: 
                	system("cls");
                	printf("Atualizando Funcion�rio...\n\n");
                	system("pause");
                	atual_funcis(funcis, n);
                	break;
                case 2:
                	system("cls");
               		printf("Excluindo Funcion�rio...\n\n");
                	system("pause");
                	funcis = demit_funcis(funcis, &n);
               		break; 
               	case 0:
               		printf("Saindo...\n\n");
                	system("cls");
                	break;
                default:
                	printf("Opcao invalida\n");
			}
		}else{
			if(strcmp(usuario, "embarcacoes") == 0){
				switch(opcao){	
					case 1:
						system("cls");
                		printf("Atualizando Embarca��o...\n\n");
                		system("pause");
                		atual_barcos(barcos, m);
                		break;
					case 2:
						system("cls");
                		printf("Excluindo Embarca��o...\n\n");
                		system("pause");
                		barcos = deletar_barcos(barcos, &m);
                		break;
					case 0:
						printf("Saindo...\n\n");
                		system("cls");
                		break;
					default:
						printf("Opcao invalida");
				}
			}else{
				if(strcmp(usuario, "clientes")){
					switch(opcao){	
					case 1:
						system("cls");
                		printf("Atualizando Cliente...\n\n");
                		system("pause");
                		break;
					case 2:
						system("cls");
                		printf("Excluindo Cliente...\n\n");
                		system("pause");
                		break;
					case 0:
						printf("Saindo...\n\n");
                		system("cls");
                		break;
					default:
						printf("Opcao invalida");
				}
				}
			}
		}
	}while(opcao != 0);
}

void menuRotas(){
	int escolha, sair = 0, i, j;

    while (!sair) {
        system(CLEAR);
        printf("======= MENU PRINCIPAL =======\n");
        printf("0. Sair\n");
        printf("1. Listar Rotas Dispon�veis\n");
        printf("2. Filtros\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &escolha);

        // Limpa o buffer de entrada
        while (getchar() != '\n');

        switch (escolha) {
            case 1:
                listarRotas(); // Encapsule a l�gica de rotas em uma fun��o separada
                break;

            case 2:
                filtrarRotas();
                break;

            case 0:
                printf("Encerrando o programa. At� mais!\n");
                sair = 1; // Marca a sa�da do loop principal
                break;

            default:
                printf("Op��o inv�lida! Tente novamente.\n");
                _getch(); // Aguarda entrada para n�o pular a mensagem
        }
    }
}

// Menu do Gerente
void menuGerente() {
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("BANZEIRO - GERENTE");
	Barco *barcos = NULL;
	Funcionario *funcis = NULL;
	struct cliente *head_cliente= NULL;
	int n = 0, m = 0, opcao=0;
    do {
  		printf("\n =============== Menu Gerente ===============\n\n");
        printf("\t 1. Cadastrar Funcion�rio\n");
        printf("\t 2. Gerenciar Funcion�rio\n");
        printf("\t  3. Listar Funcion�rios\n");
        
        printf("\t 4. Cadastrar Embarca��o\n");
		printf("\t 5. Gerenciar Embarca��o\n");
        printf("\t  6. Listar Embarca��es\n");
        
        printf("\t  7. Leis\n");
        
        printf("\t\t  0. Sair\n\n");
        printf("\t   Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao) {
        	case 1:
        		system("cls");
                printf("\n\nCadastrando Funcion�rio...\n\n");
                system("pause");
                funcis = cadas_funcis(funcis, &n);
                break;
            case 2:
            	system("cls");
                printf("\n\nGerencia de Funcion�rios...\n\n");
                system("pause");
                gerenciar("funcionario");
                break;
            case 3:
            	system("cls");
                printf("Listando Funcion�rios...\n\n");
                system("pause");
                ver_funcis(funcis, n);
                break;
            case 4:
            	system("cls");
                printf("Cadastrando Embarca��o...\n\n");
                system("pause");
            	barcos = inser_barcos(barcos, &m);
                break;
            case 5:
            	system("cls");
                printf("Gerencia de embarca��es...\n\n");
                system("pause");
                gerenciar("embarcacoes");
                break;   
            case 6:
            	system("cls");
                printf("Listando Embarca��es...\n\n");
                system("pause");
                ver_barcos(barcos, m);
                break;
            case 7:
            	printf("Leis\n");
            	break;
			case 0:
                printf("Saindo...\n\n");
                system("cls");
                break;
            default:
                printf("Op��o inv�lida!\n\n");
        }
    } while (opcao != 0);
}

void areaCliente(){
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("BANZEIRO - FUNCION�RIO");	
	Funcionario *funcis = NULL;
	int escolha;

	printf("\n ============= Menu Cliente =============\n\n");
	printf("0. Sair");
	printf("1. Cadastrar Cliente\n");
	printf("2. Gerenciar\n");
	scanf("%d", &escolha);

	switch(escolha){
		case 1:
			system("cls");
			printf("Cadastrando Cliente...\n");
			system("pause");
			
			break;
		case 2:
			system("cls");
			printf("Gerenciar Cliente...\n");
			system("pause");
			gerenciar("clientes");
			break;
	}
}

void menuFuncionario() {
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("BANZEIRO - FUNCION�RIO");
	int n = 0, m = 0, opcao=0;
	
    do {
        printf("\n ============= Menu Funcion�rio =============\n\n");
        printf("\t           0. Sair\n\n");
		printf("\t    1. Area Cliente\n");
        printf("\t    2. Rotas\n\n");
        printf("\t   3. Leis\n\n");
        printf("\t    Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: 
				printf("Area Cliente...\n");
				areaCliente();
				system("pause");
				system("cls");
				break;
            case 2: 
				printf("Rotas\n");
				menuRotas();
				system("pause");
       			system("cls");
				break;
			case 3:
				printf("Leis\n"); 
				system("pause");
				system("cls");
				break;
			case 0:
				printf("Saindo...\n"); 
				system("cls");
				break;
			default:
				printf("Op��o inv�lida!\n");
				system("pause");
				system("cls");
        }
    } while (opcao != 0);
}

void login() {
	setlocale(LC_ALL, "Portuguese");
    Funcionario usuario;
    char login[20], senha[20];
    int i = 0;

    printf("\n ============= Login do Usu�rio =============\n\n");
    printf("   Usu�rio: ");
    scanf("%s", login);
    printf("   Senha: ");

    // Mascarando a entrada da senha
    while (1) {
        char ch = getch();  // Captura o caractere sem exibi-lo
        if (ch == 13) {     // Enter
            senha[i] = '\0'; // Finaliza a string da senha
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Apaga o �ltimo caractere na tela
            }
        } else {
            senha[i++] = ch;  // Armazena o caractere na senha
            printf("*");       // Exibe um asterisco
        }
    }
    printf("\n");

    if (carregarFuncionario(login, &usuario)) {
    	setlocale(LC_ALL, "Portuguese");
        if (strcmp(usuario.senha, senha) == 0) {
            printf("\n\n\tBem-vindo, %s! (%s)\n", usuario.nome, usuario.cargo);
            Sleep(4200);
            
            animacaoBarco();  // Exibe a anima��o do barco

            if (strcmp(usuario.cargo, "Gerente") == 0) {
                system("cls");
				menuGerente();
            } else {
            	system("cls");
                menuFuncionario();
            }
        } else {
            printf("Senha incorreta!\n");
            Sleep(2200);
            system("cls");
        }
    } else {
        printf("Usu�rio n�o encontrado!\n");
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    SetConsoleTitle("BANZEIRO");
    Funcionario gerente = {"Ana", "admin", "1234", "Gerente", 01, 7037};
    Funcionario funcionario = {"Jo�o", "func", "abcd", "Funcion�rio", 02, 8833};
    salvarFuncionario("admin", &gerente);
    salvarFuncionario("func", &funcionario);
    salvarFuncionario("funcionarios.dat", &gerente);

    int opcao;
    do {
        printf("\n === Sistema de Gerenciamento de Passagens ===\n\n");
        printf("\t\t1. Login\n");
        printf("\t    2. Recuperar Senha\n");
        printf("\t\t 0. Sair\n");
        printf("\n\t   Escolha uma op��o: ");
        scanf("%d", &opcao);
        system("cls");

        switch (opcao) {
            case 1: 
				login(); 
				break;
            case 2: 
				recuperarSenha(); 
				break;
			case 0: 
				printf("\n ========== Encerrando, at� amanh�! ==========\n\n");
            	Sleep(2000);
				break;
            default: 
				printf("Op��o inv�lida!\n");
        }
    } while (opcao != 0);

    return 0;
}
