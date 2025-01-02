#include "auxiliar.h"

//Vari�veis globais
int i,j;

// Fun��o de menu
int mostrarMenuGerente(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posi��o do cursor
    int tecla;
    do {
        system(CLEAR);
        printf("\n ============= %s =============\n\n", titulo); // Exibe o t�tulo do menu

        // Exibe as op��es do menu com a seta indicando a op��o atual
        for (i = 0; i < numOpcoes; i++) {
            if (posicao == i) {
                setColor(14); // Amarelo para a op��o selecionada
                printf("    -> %s\n", opcoes[i]);
            } else {
                setColor(7); // Branco para as outras op��es
                printf("       %s\n", opcoes[i]);
            }
        }
        setColor(7); // Reset para a cor padr�o

        tecla = _getch();
        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posicao > 0) posicao--;
                    break;
                case 80: // Seta para baixo
                    if (posicao < numOpcoes - 1) posicao++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            return posicao + 1; // Retorna a op��o selecionada (1-based)
        }
    } while (tecla != 27); // ESC para sair do menu

    return 0; // Retorna 0 se o usu�rio pressionar ESC
}

int mostrarMenu(const char *titulo, const char **opcoes, int numOpcoes) {
    int posicao = 0; // Inicializa a posi��o do cursor
    int tecla;
    do {
        system(CLEAR);
        printf("\n ============= %s =============\n\n", titulo); // Exibe o t�tulo do menu

        // Exibe as op��es do menu com a seta indicando a op��o atual
        for (i = 0; i < numOpcoes; i++) {
            setColor(posicao == i ? 14 : 7); // Amarelo para a op��o selecionada
            printf("    -> %s\n", opcoes[i]);
            setColor(15); // Reset para a cor padr�o
        }

        tecla = _getch();
        if (tecla == 224) { // Teclas de seta
            tecla = _getch();
            switch (tecla) {
                case 72: // Seta para cima
                    if (posicao > 0) posicao--;
                    break;
                case 80: // Seta para baixo
                    if (posicao < numOpcoes - 1) posicao++;
                    break;
            }
        } else if (tecla == 13) { // Enter
            return posicao; // Retorna a op��o selecionada
        }
    } while (tecla != 27); // ESC para sair do menu

    return -1; // Retorna -1 se o usu�rio pressionar ESC
}
