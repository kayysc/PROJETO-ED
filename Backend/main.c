#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM 5 // capacidade/assentos do barco
#define PASSAGENS 3 //Quantas passagens o usuario selecionou, ser� definida antes dessa p�gina

void selecionar(int *m, int i, int j) {
    int caso;

    while (1) {
        printf("-----------------------\n");
        printf("| 1. Continuar Sele��o |\n");
        printf("| 2. Excluir �ltima sele��o |\n");
        printf("-----------------------\n");
        scanf("%d", &caso);

        if (caso == 1) {
            return; // Retorna para a sele��o de assentos
        } else if (caso == 2) {
            m[j] = 1; // Libera o assento
            printf("Assento exclu�do da sele��o.\n");
            // Falta cont -= cont aqui, quando coloco da  erro
            return; // Retorna para a sele��o de assentos
        } else {
            printf("Op��o inv�lida! Tente novamente.\n");
        }
    }
}
int main(){
    setlocale(LC_ALL, "Portuguese");

    int m[TAM][TAM], i, j, cont, caso;
    int *ptr_i, *ptr_j; //Definindo ponteiros
    int (*ptr_m)[TAM] = m; //Definindo ponteiros

    ptr_i = &i;
    ptr_j = &j;
    //Valor 1 == assento dispon�vel, valor 0 == assento n�o disponivel
    for(i=0; i<TAM; i++){
        for(j=0; j<TAM; j++){
            ptr_m[i][j] = 1; //alocando valor 1 em todos indices
        }
    } //Solu��o acima possivelmente provis�ria

    for(cont=0; cont<PASSAGENS; cont++){
        printf("-----------------------\n");
        printf("| Sele��o de assentos |\n"); //Utilizei assentos pois n�o achei um termo melhor
        printf("-----------------------\n");
        printf("Escolha o seu lugar(maximo de 3 lugareas por usu�rio)\n");
        for(i=0; i<TAM; i++){
            for(j=0; j<TAM; j++){
                printf("[%d] ", ptr_m[i][j]);// mostra matriz na tela
            }
            printf("\n");
        }

        scanf("%d %d", ptr_i, ptr_j);//Sele��o do assento
        fflush(stdin);

        if(*ptr_i >= TAM || *ptr_i >= TAM){//Verifica caso usu�rio coloque assento n�o dispon�vel
            printf("Op��o inv�lida! Tente novamente\n" );
            cont -= cont;//Garante que usu�rio sempre selecione assento para cada passagen, nesse caso 3
        }else{
            if(ptr_m[i][j] == 1){
                ptr_m[i][j] = 0;//Vaga selecionada
                system("cls");
                selecionar(m[i], i, j); // Tabela se continua ou exclui a ultima sele��o
            }else{
                printf("O lugar j� est� reservado\n");//Caso selecione lugar j� reservado
                cont -= cont;
                system("pause");
            }
        }
        system("cls");
    }
    printf("-----------------------\n");
    printf("| Lugares reservados |\n");//Final
    printf("-----------------------\n");
    return 0;
}

