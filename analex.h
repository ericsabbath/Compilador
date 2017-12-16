
#include <stdio.h>
#include <stdlib.h>

#define TAM_L_TAB_PR 14
#define TAM_C_TAB_PR 14
#define TAM_L_TAB_LT 1000
#define TAM_C_TAB_LT 11
#define TAM_L_TAB_SN 23
#define TAM_C_TAB_SN 3

#define fimarquivo 1
#define fimarquivo2 2
#define constnaoterm 3
#define caracinvalido 4
#define caraccon 5

// Palavras reservadas

enum {
    inteiro, booleano, caracter, real, para, se, senao, retorne, semparam, semretorno, enquanto, fim, prototipo
};

char TabPR[TAM_L_TAB_PR][TAM_C_TAB_PR];

// Sinais
enum {
    abrechave, abrecolchete, abreparentese, fechachave, fechacolchete, fechaparentese,
	soma, subtracao, multiplicacao, divisao,
	atribui, ecomercial, e , ou,
	igual, diferente, maiorigual, menorigual, maior, menor,
    negacao, pontovirgula, virgula
};

// Tabela de sinais
char TabSN[TAM_L_TAB_SN][TAM_C_TAB_SN];

// Tabela de literais
char TabLT[TAM_L_TAB_LT][TAM_C_TAB_LT];


typedef enum {
    CT_B, CT_C, CT_R, CT_I, ID, PR, SN
} categoria;

typedef struct {
    categoria categ;

    union {
        int codigo;
        int indice;
        int valor_int;
        float valor_real;
        int valor_bool;
        char lexema[20];
    };
} token;

FILE *fp;
int estado;
int num_linhas;
int ind_lt;
char c;
int aux_fim;

token analex();
void TabelaPR();
void TabelaSN();
int letra(char c);
int digito(char c);
int letradigito(char);
int arquivo();
void mensagem(int);
void caracterinvalido(char);
void imprime(token);
