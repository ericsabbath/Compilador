#ifndef ANASIN_H_INCLUDED
#define ANASIN_H_INCLUDED
#endif // ANASIN_H_INCLUDED

#include "analex.h"

//categoria de identificador
typedef enum {
    VAR_GLOBAL, VAR_LOCAL, FUNC, PAR, SEM_CATEG, DECL
} cat_id;

typedef enum {
    T_INT, T_BOOL, T_REAL, T_CARAC, SEM_TIPO
} tipo_id;

typedef struct {
    char id[30];
    cat_id categ;
    int tipo;
    int escopo;

    union {

        struct _var_global {
            int extrn;
            int eh_array;
        } var_global;

        struct _var_local {
            char nome_funcao[30];
            int eh_array;
        } var_local;

        struct _parametro {
            char nome_funcao[30];
            int ref;
            int eh_array;
            int excluido;
        } parametro;

        struct _funcao {
            int num_param;
        } funcao;
    };
} reg_ts;

token token_atual;
token proximo_token;

tipo_id tipo_atual;
int escopo_atual;
cat_id categ_atual;
reg_ts tab_sb[1000];
int ind_tb;
char n_func_atual[30];

void imprimir(token);
void mensagemErro(char *);
void reconhecePR(int);
void reconheceSN(int);
void reconheceID();
void reconhece_intcon();
void reconhece_realcon();
void reconhece_carcon();

void prog();
void decl();
void tipo();
void tipos_param();
void cmd();
void atrib();
void expr();
void expr_simp();
void termo();
void fator();
void op_rel();

int insereID(char *);
void updateID();

int cont_var_g;
int cont_label;
int gera_label();
int lx, lx2, lx3, lx4;
