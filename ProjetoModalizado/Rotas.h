//
// Created by carlo on 01/01/2025.
//

#ifndef ROTAS_H
#define ROTAS_H


#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3

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
void salvarAssentos(char ***assentos, int linhas, int colunas, const char *nomeArquivo);
void carregarAssentos(char ***assentos,int linhas,int colunas,const char *nomeArquivo);
void inicializarAssentos(char ***assentos, int linhas, int colunas);
char ***criarAssentos(int linhas, int colunas);
void liberarAssentos(char ***assentos, int linhas, int colunas);
void mostrarAssento(char ***assentos,int linhas,int colunas, int posLinha,int posColuna);
int escolherAssento(char ***assentos, int linhas, int colunas, char assentosEscolhidos[MAX_PASSAGENS][5],int *totalAssentosEscolhidos,Rota rota);



#endif //UNTITLED_ROTAS_H

