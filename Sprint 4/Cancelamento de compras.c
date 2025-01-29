#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define MAX_DESTINO 4
#define MAX_CLIENTE 3

typedef struct{
    int notafiscaltemp;
    char nome[50];
    char cpf[20];
    struct {
        char preco[50];
        char horario[10];
        char rota[50];
        char nota[50];
    }notaFiscal;
}cliente;

cliente Cliente[MAX_CLIENTE] = { 
    {14, "Carlos", "016.185.562.82", {"R$200", "8:00", "Manaus -> Tabatinga", "123456789"}}, //Declarações dos clientes
    {23, "Cristiane", "823.193.345.21",{"R$150", "10:30", "Manaus -> Belém", "01020304"}},
    {9,  "Leandra", "921.123.343.24", {"R$250", "10:00", "Manaus-> Santarém", "0203040506"}},
};


typedef struct {
    char rota[50];
    char horario[10];
    char preco[50];
}barco;

barco Barcos[MAX_DESTINO] = {
    {"Manaus -> Tabatinga", "08:00", "R$200"},  // Declaração dos barcos
    {"Manaus -> Belém", "10:30", "R$150"}, 
    {"Manaus -> Santarém", "10:00", "R$250"},
    {"Manaus -> Parintins", "11:00", "R$130"}};

void menu();
void telaCancelamento();
void linha();


int main(){
    SetConsoleTitle("Tela de Menu");
    menu();
    return 0;
}

void menu(){
    int escolha,i=0;
    while (i<1){
        printf ("Tela de cancelamento de passagem\n\n");
        printf ("Digite 1 ou 2\n\n1.Cancelar Passagem\n2.Sair\n");
        scanf ("%d",&escolha);
        getchar(); 
        switch (escolha){
            case 1:
                telaCancelamento();
                system("cls");
                break;
            case 2:
                printf ("Saindo...");
                Sleep(5000);
                i++;
                break;
            default:
                printf ("Erro, comando inválido!!\n\nTente isso novamente");
                Sleep(5000);
                system("cls");
        }
    }
}
    
void telaCancelamento(){
	
	system("cls");
    char notafiscal[50];
    int i=0,escolha,aux=0,j=0,k=0;
    SetConsoleTitle("Tela de Cancelamento");
    printf ("Digite sua nota fiscal: ");
    fgets(notafiscal,50,stdin);

    notafiscal[strcspn(notafiscal, "\n")] = '\0';
    for(int i=0;i<MAX_DESTINO;i++){
        if(strcmp(notafiscal,Cliente[i].notaFiscal.nota)==0){
        	linha();
            printf ("\nNome: %s",Cliente[i].nome);
            printf ("\ncpf: %s",Cliente[i].cpf);
            printf ("\nSua nota fiscal: %s\nsua rota: %s que sai as %s\n",Cliente[i].notaFiscal.nota,Cliente[i].notaFiscal.rota,Cliente[i].notaFiscal.horario);
            printf ("\nFaltando apenas %dh",Cliente[i].notafiscaltemp);
            aux++;
    		if (Cliente[i].notafiscaltemp>=12){
    				linha();
        			printf ("\n\nVoce tem direito de reembolso total");
        			printf ("\nDeseja realmente fazer cancelamento de passagem?\nEscolha entre 1 ou 2\n\n1.Sim\n2.Nao\n");
				while(j<1){
					scanf ("%d",&escolha);
					switch (escolha){
        				case 1:
        					printf ("Sua passagem foi cancelada, em ate 48h vai ser creditado na tua conta");
        					printf ("Obrigado, volte sempre!!");
        					Sleep(5000);
        					j++;
        					break;
        				case 2: 
        					printf ("Passagem nao cancelada, retornando ao menu principal");
        					Sleep(5000);
        					j++;
							break;
        				default:
        					printf("Erro,comando invalido\nTente novamente");
        					Sleep(3000);
        			}
				}
			}
			else{
				linha();
				printf ("Voce tem direito somente parcial do valor que sera restituido");
				printf ("\nDeseja realmente fazer cancelamento de passagem?\nEscolha entre 1 ou 2\n\n1.Sim\n2.Nao\n");
				printf ("");
        		scanf ("%d",&escolha);
				while(j<1){			
					switch (escolha){
        				case 1:
        					printf ("Sua passagem foi cancelada, em ate 48h vai ser creditado na tua conta");
        					printf ("Obrigado, volte sempre!!");
        					Sleep(5000);
        					j++;
        					break;
        				case 2: 
        					printf ("Passagem nao cancelada, retornando ao menu principal");
        					Sleep(5000);
        					j++;
        					break;
        				default:
        					printf("Erro,comando invalido\nTente novamente");
        					Sleep(3000);
					}
				}
    		}
    	}
	}
	if(aux==0){
		system("cls");
    	printf ("Nota Fiscal invalida, tente novamente\n");
    	Sleep(1000);
	}
}

void linha(){
	printf ("\n*******************************************************\n");
}