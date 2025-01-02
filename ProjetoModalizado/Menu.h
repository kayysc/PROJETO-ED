//
// Created by carlo on 01/01/2025.
//

#ifndef UNTITLED_MENU_H
#define UNTITLED_MENU_H


int mostrarMenuGerente(const char *titulo,const char **opcoes, int Opcoes);
void menuGerente();
void menuFuncionario();
void login();

int mostrarMenu(const char *titulo,const char **opcoes,int numOpcoes);
void menuCliente(struct cliente **head_cliente);


#endif //UNTITLED_MENU_H

