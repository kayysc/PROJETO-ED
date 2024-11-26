#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <String.h>
#include <ctype.h>

int prox_id=1;

struct feedbacks{
		int id;
		char descricao[100];
	struct feedback *prox_list;
};
struct pessoa{
	char nome[30];
	char cpf[15];
	struct feedback *feedbacks;
	struct pessoa *prox_list_P;
};

void telaMenu();
void limparTela();
void telaCadastro(struct pessoa **head);
void telaFeedback(struct pessoa *head);
void fazerFeedback(struct pessoa **head);
void remocaoFeedback(struct pessoa **head,int id);
void listarFeedback(struct pessoa *head);
void telaListar(struct pessoa *head);



int main (){

	
	setlocale(LC_ALL,"Portuguese");
	
	telaMenu();
	return 0;
	
}
void telaMenu(){
	struct pessoa *pessoas = NULL;

	
	int escolha,i;
	do{
		SetConsoleTitle("Tela de Login");
		printf ("Digite:\
				\n1.Tela de feedback\
				\n2.Listar\
				\n3.Sair\
				\n?");
		scanf ("%i",&escolha);
		
		switch (escolha){
			
			case 1:
				telaCadastro(&pessoas);
				limparTela();
				break;
			case 2:
				telaListar(pessoas);
				break;
			case 3: 
				printf ("\nSaindo...");
				limparTela();
				i++;
				break;
			default:
				printf ("\nDigito incorreto, tente novamente");
				Sleep(3000);
				system("cls");
				break;
		}
	}while(i<1);
}
void limparTela(){
	printf ("\n\nClique tecla Enter para continuar!!!");
	getchar();
	getchar();
	system("cls");
}

void telaCadastro(struct pessoa **head){
	system("cls");
	SetConsoleTitle("Tela de feedback");
	
	int escolha;
	struct pessoa *cadastro = (struct pessoa*) malloc(sizeof(struct pessoa));
	
	if (cadastro==NULL){
		printf ("Erro!! Memoria não alocada");
		exit(1);
	}
	
	cadastro->feedbacks = NULL;
	
int i;
    
	do {
        fflush(stdin);
        printf("CPF:");
        fgets(cadastro->cpf, 15, stdin);
        cadastro->cpf[strcspn(cadastro->cpf, "\n")] = '\0';

        
        i = 0; 
        while (cadastro->cpf[i] != '\0') {
            if (!isdigit(cadastro->cpf[i])) {
                printf("CPF inválido! Digite apenas números.\n");
                break;
            }
            i++; 
        }

       
        if (cadastro->cpf[i] == '\0' && (strlen(cadastro->cpf) != 11)) {
            printf("CPF digitado é inválido, tente novamente (deve ter 11 dígitos).\n");
        }

    } while (cadastro->cpf[i] != '\0' || strlen(cadastro->cpf) != 11); 
    
	struct pessoa *verificador = *head;
	struct pessoa *usuario;

	while(verificador!=NULL){
		
		if(strcmp(verificador->cpf,cadastro->cpf)==0){
			usuario=verificador;
			telaFeedback(usuario);
			printf ("Conta Encontrada!!!");
			
			return;
		}
		verificador=verificador->prox_list_P;	
	}
	printf ("Digite seu nome: ");
	fgets(cadastro->nome,30,stdin);
	cadastro->nome[strcspn(cadastro->nome, "\n")] = '\0';
	
	cadastro->prox_list_P = *head;
	*head= cadastro;
	printf ("\nUsuário registrado!!!\n");
}


void telaListar(struct pessoa *head){
	system("cls");
	if (head==NULL){
		printf ("Não tem registro de usuários!!!");
		limparTela();
		return;
	}
	struct pessoa *verificador=head;
	
	while(verificador!=NULL){
		
		printf ("\n**************************************\n");
		printf ("\nUsuário: %s",verificador->nome);
		printf("\nCPF: %s",verificador->cpf);
		
		verificador=verificador->prox_list_P;
		
	}
	limparTela();
}

void telaFeedback(struct pessoa *head){
	int i,escolha,id;
	printf ("Usuário: %s\n",head->nome);
	do{
		printf ("\nDigite:\
				\n1.Fazer Feedbacks\
				\n2.Remover Feedbacks\
				\n3.Listar Feedbacks\
				\n4.Sair\n?");
				
		scanf ("%i",&escolha);
		switch (escolha){
			case 1:
				fazerFeedback(&head);
				limparTela();
				break;
			case 2:
				printf ("Digite o ID que vc queira tirar: ");
				scanf ("%i",&id);
				remocaoFeedback(&head,id);
				break;
			break;
			case 3:
				listarFeedback(head);
				limparTela();
				break;
			case 4:
				printf ("Saindo!!!");
				Sleep(3000);
				i++;
			default:
				printf ("Digito incorreto, tente novamente!!");
				limparTela();
				
				
		}
	}while(i<1);
	
}

void fazerFeedback(struct pessoa **head){
	system("cls");
	fflush(stdin);
	struct feedbacks *novoComentario = (struct feedbacks*) malloc(sizeof(struct feedbacks));
	
	novoComentario->id = prox_id;
	prox_id++;
	
	fflush(stdin);
	printf ("\nElogios,Reclamação,Comentário: ");
	fgets(novoComentario->descricao,100,stdin);
    	novoComentario->descricao[strcspn(novoComentario->descricao,"\n")] = '\0';


	novoComentario->prox_list = (*head)->feedbacks;
	(*head)->feedbacks = novoComentario;
	
	printf ("\nFeedback adicionado com sucesso!\n");

}

void listarFeedback(struct pessoa *head) {
    if (head->feedbacks == NULL || head == NULL) {
        printf("\nNão há nenhum Feedback criado!\n");
        return;
    }
    struct feedbacks *listar = head->feedbacks;

    while (listar != NULL) {
        printf("ID: %i\n", listar->id); 
        printf("Descrição: %s\n", listar->descricao); 
        printf("--------------------\n");

        listar = listar->prox_list; 
    }
    limparTela();
}

void remocaoFeedback(struct pessoa **head, int id) {
    system("cls");
    SetConsoleTitle("Tela de Remoção");

    if (*head == NULL) {
        printf("Não há pessoas cadastradas!\n");
        return;
    }

    struct pessoa *pessoaAtual = *head;
    while (pessoaAtual != NULL) {
        struct feedbacks *atual = pessoaAtual->feedbacks;
        struct feedbacks *anterior = NULL;

        while (atual != NULL && atual->id != id) {
            anterior = atual;
            atual = atual->prox_list;
        }

        if (atual != NULL) {
            if (anterior == NULL) { 
                pessoaAtual->feedbacks = atual->prox_list;
            } else { 
                anterior->prox_list = atual->prox_list;
            }
            free(atual);
            printf("Feedback removido com sucesso!\n");
            return; 
        }

        pessoaAtual = pessoaAtual->prox_list_P;
    }

    printf("Feedback com ID %d não encontrado!\n", id);
}

