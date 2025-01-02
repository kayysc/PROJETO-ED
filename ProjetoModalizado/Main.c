#include "Util.h"
#include "Funcionalidade.h"
#include "Rotas.h"
#include "Cliente.h"
#include "Menu.h"
#include "NotaFiscal.h"

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR "cls"
#define SLEEP(ms) Sleep(ms)
#else
#define CLEAR "clear"
#define SLEEP(ms) usleep(ms * 1000)
#endif
int i,j;
#define LETRAS "ABCDEFGH"
#define MAX_DESTINOS 4
#define MAX_PASSAGENS 3


int main(){
    setlocale(LC_ALL,"Portuguese");

    Gerente gerente = {"Ana","admin","1234",01};
    salvarGerente("admin",&gerente);

    if(!carregarTodosFuncionariosPersistente("funcionario.dat",funcionarios,&num_funcionarios)) {
        printf ("\n         Nenhum funcionario encontrado. Inicializando com lista vazia.\n");
    }

    printf("\n Nuºmero de funciona¡rio carregados: %d\n",num_funcionarios);

    login();

    salvarTodosFuncionariosPersistente("funcionario.dat",funcionarios,num_funcionarios);

    return 0;

}
