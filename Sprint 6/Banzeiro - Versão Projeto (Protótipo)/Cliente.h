//
// Created by carlo on 01/01/2025.
//

#ifndef CLIENTE_H
#define CLIENTE_H

struct cliente{
    int id;
    char nome[50];
    char cpf[15];
    char data_nasc[11];
    char telefone[20];
    struct cliente *proximo_cl;
};

void cadastrar_cliente(struct cliente **Head);
struct cliente* buscarCliente(struct cliente *head, const char *cpf, int exibir_informacoes);
struct cliente* buscarCliente_por_cpf_com_exibir(struct cliente *head, const char *cpf);
struct cliente* buscarCliente_por_cpf_sem_exibir(struct cliente *head, const char *cpf);


#endif //UNTITLED_CLIENTE_H

