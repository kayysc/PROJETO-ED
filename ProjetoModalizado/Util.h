#ifndef UTIL_H
#define UTIL_H


void setColor(int color);
void animarBarco();
void ler_cpf_formatado(char *cpf);
void remover_formatacao_cpf(const char *cpf_formatado,char *cpf_limpo);
void ler_telefone_formatado(char *telefone);
void ler_conta_bancaria_formatada(char *conta);
int calcular_idade(const char *data_nasc);
int validar_data(const char *data);
void ler_data_formatada(char *data);

#endif //UNTITLED_UTIL_H

