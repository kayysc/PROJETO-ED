//
// Created by carlo on 01/01/2025.
//

#ifndef NOTA_FISCAL_H
#define NOTA_FISCAL_H


#include "Rotas.h"
#include "Cliente.h"

struct Nota{
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


void geracaoNotaFiscal(Rota rota,int preco_total,const char *nome, const char *cpf);
void ListagemNota();
void finalizarCompra(int totalAssentos, Rota rota, char assentosEscolhidos[MAX_PASSAGENS][5], const char *nome, const char *cpf);
int detalhesDaCompra(char assentosEscolhidos[MAX_PASSAGENS][5], int totalAssentos, Rota rota, char ***assentos, struct cliente **head_cliente);
void listarRotas(struct cliente **head_cliente);
void cancelarCompra();
void salvarAssentos(char ***assentos, int linhas, int colunas, const char *nomeArquivo);
void carregarAssentos(char ***assentos,int linhas,int colunas,const char *nomeArquivo);


#endif //NOTA_FISCAL_H

