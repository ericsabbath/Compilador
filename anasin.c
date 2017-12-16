#include "anasin.h"

void imprimir(token t) {
    switch (t.categ) {
        case ID: printf("  <ID> -- %s\n\n", t.lexema);
            break;
        case PR: printf("  <PR> -- %s\n\n", TabPR[t.codigo]);
            break;
        case SN: printf("  <SN> -- %s\n\n", TabSN[t.codigo]);
            break;
        case CT_I: printf("  <CT_I> -- %d\n\n", t.valor_int);
            break;
        case CT_R: printf("  <CT_R> -- %f\n\n", t.valor_real);
            break;
        case CT_C: printf("  <CT_C> -- \"%s\"\n\n", TabLT[t.indice]);
            break;
    }
}

void mensagemErro(char *mensagem) {
    printf("\n\nErro: %s na linha %d.\n\n\n\n", mensagem, num_linhas);
    system("PAUSE");
    exit(1);
}

void reconhecePR(int cod) {
    char mensagem[32];
    if ((token_atual.categ == PR) && (token_atual.codigo == cod)) {
        imprimir(token_atual);
        token_atual = proximo_token;
        proximo_token = analex();
    } else {
        sprintf(mensagem, "Palavra reservada %s esperada", TabPR[cod]);
        mensagemErro(mensagem);
    }
}

void reconheceSN(int cod) {
    char mensagem[32];
    if ((token_atual.categ == SN) && (token_atual.codigo == cod)) {
        imprimir(token_atual);
        token_atual = proximo_token;
        proximo_token = analex();
    } else {
        sprintf(mensagem, "Sinal %s esperado", TabSN[cod]);
        mensagemErro(mensagem);
    }
}

void reconheceID() {
    char mensagem[32];
    if (token_atual.categ == ID) {
        if (insereID(token_atual.lexema) == 0) {
            sprintf(mensagem, "Redeclaracao de %s", token_atual.lexema);
            mensagemErro(mensagem);
        }
        imprimir(token_atual);
        token_atual = proximo_token;
        proximo_token = analex();
    } else {
        mensagemErro("Identificador esperado");
    }
}

void reconhece_intcon() {
    if (token_atual.categ == CT_I) {
        imprimir(token_atual);
        token_atual = proximo_token;
        proximo_token = analex();
    } else {
        mensagemErro("Numero inteiro esperado");
    }
}

void reconhece_realcon() {
    if (token_atual.categ == CT_R) {
        imprimir(token_atual);
        token_atual = proximo_token;
        proximo_token = analex();
    } else {
        mensagemErro("Numero real esperado");
    }
}

void reconhece_charcon() {
    if (token_atual.categ == CT_C) {
        imprimir(token_atual);
        token_atual = proximo_token;
        proximo_token = analex();
    } else {
        mensagemErro("Literal esperado");
    }
}

void prog() {
    token_atual = analex();
    proximo_token = analex();
    ind_tb = 0;

    cont_var_global = 0;
    cont_label = 1;

    int i;
    int princip = 0;

    while (token_atual.categ != -1) {
        tipo_atual = SEM_TIPO;
        escopo_atual = 0;
        categ_atual = SEM_CATEG;
        decl();
    }

    for (i = 0; i <= ind_tb; i++) {
        if (strcmp(tab_sb[i].id, "principal") == 0) {
            princip = 1;
        }
    }

    if (!princip) {
        mensagemErro("Funcao principal nao encontrada");
    }


    if(cont_var_global > 0) {
        printf("DMEM %d\n", cont_var_global);
    }

    printf("HALT\n");

    if (token_atual.categ == -1) {
        mensagem(fimarquivo);
    }
}

void decl() {
    if (token_atual.categ == PR) {
            if ((token_atual.categ == PR) && ((token_atual.codigo == semretorno)||(token_atual.codigo == prototipo))) {
                if (token_atual.codigo == semretorno) {
                    reconhecePR(semretorno);
                } else if (token_atual.codigo == prototipo) {
                    reconhecePR(prototipo);
                }
                reconheceID();
                reconheceSN(abreparentese);
                tipos_param();
                reconheceSN(fechaparentese);
                if ((token_atual.categ == SN)
                        && (token_atual.codigo == abrechave)) {
                    categ_atual = FUNC;


                    lx = gera_label();
                    printf("LABEL L%d\n", lx);
                    //

                    updateID();
                    reconheceSN(abrechave);
                    while ((token_atual.categ == PR)
                            && ((token_atual.codigo == booleano)
                            || (token_atual.codigo == caracter)
                            || (token_atual.codigo == real)
                            || (token_atual.codigo == inteiro))) {
                        tipo();
                        categ_atual = VAR_LOCAL;
                        reconheceID();
                        if ((token_atual.categ == SN)
                                && (token_atual.codigo == abrecolchete)) {
                            reconheceSN(abrecolchete);
                            reconhece_intcon();
                            reconheceSN(fechacolchete);
                        }
                        while ((token_atual.categ == SN)
                                && (token_atual.codigo == virgula)) {
                            reconheceSN(virgula);
                            reconheceID();
                            if ((token_atual.categ == SN)
                                    && (token_atual.codigo == abrecolchete)) {
                                reconheceSN(abrecolchete);
                                reconhece_intcon();
                                reconheceSN(fechacolchete);
                            }
                        }
                        reconheceSN(pontovirgula);


                    }
                    while (((token_atual.categ == PR)
                            && ((token_atual.codigo == se)
                            || (token_atual.codigo == para)
                            || (token_atual.codigo == retorne)
                            || (token_atual.codigo == enquanto)))
                            || (token_atual.categ == ID)
                            || ((token_atual.categ == SN)
                            && ((token_atual.codigo == pontovirgula)
                            || (token_atual.codigo == abrechave)))) {

                        cmd();
                    }
                    reconheceSN(fechachave);
                } else {
                    categ_atual = DECL;
                    updateID();
                    while ((token_atual.categ == SN)
                            && (token_atual.codigo == virgula)) {
                        reconheceSN(virgula);
                        reconheceID();
                        reconheceSN(abreparentese);
                        tipos_param();
                        reconheceSN(fechaparentese);
                    }
                    reconheceSN(pontovirgula);

                }
            } else {
                tipo();
                reconheceID();
                if ((token_atual.categ == SN)
                        && (token_atual.codigo == abreparentese)) {
                    reconheceSN(abreparentese);
                    tipos_param();
                    reconheceSN(fechaparentese);
                    if ((token_atual.categ == SN)
                            && (token_atual.codigo == abrechave)) {
                        categ_atual = FUNC;
                        updateID();
                        reconheceSN(abrechave);
                        while ((token_atual.categ == PR)
                                && ((token_atual.codigo == booleano)
                                || (token_atual.codigo ==caracter)
                                || (token_atual.codigo == real)
                                || (token_atual.codigo == inteiro))) {
                            tipo();
                            categ_atual = VAR_LOCAL;
                            reconheceID();
                            if ((token_atual.categ == SN)
                                    && (token_atual.codigo == abrecolchete)) {
                                reconheceSN(abrecolchete);
                                reconhece_intcon();
                                reconheceSN(fechacolchete);
                            }
                            while ((token_atual.categ == SN)
                                    && (token_atual.codigo == virgula)) {
                                reconheceSN(virgula);
                                reconheceID();
                                if ((token_atual.categ == SN)
                                        && (token_atual.codigo
                                        == abrecolchete)) {
                                    reconheceSN(abrecolchete);
                                    reconhece_intcon();
                                    reconheceSN(fechacolchete);
                                }
                            }
                            reconheceSN(pontovirgula);
                        }
                        while (((token_atual.categ == PR)
                                && ((token_atual.codigo == se)
                                || (token_atual.codigo == para)
                                || (token_atual.codigo == retorne)
                                || (token_atual.codigo == enquanto)))
                                || (token_atual.categ == ID)
                                || ((token_atual.categ == SN)
                                && ((token_atual.codigo == pontovirgula)
                                || (token_atual.codigo == abrechave)))) {
                            cmd();
                        }
                        reconheceSN(fechachave);
                    } else {
                        categ_atual = DECL;
                        updateID();
                        while ((token_atual.categ == SN)
                                && (token_atual.codigo == virgula)) {
                            reconheceSN(virgula);
                            reconheceID();
                            reconheceSN(abreparentese);
                            tipos_param();
                            reconheceSN(fechaparentese);
                        }
                        reconheceSN(pontovirgula);
                    }
                } else {
                    categ_atual = VAR_GLOBAL;
                    updateID();
                    cont_var_global++;
                    if ((token_atual.categ == SN)
                            && (token_atual.codigo == abrecolchete)) {
                        reconheceSN(abrecolchete);
                        reconhece_intcon();
                        reconheceSN(fechacolchete);
                    }
                    while ((token_atual.categ == SN)
                            && (token_atual.codigo == virgula)) {
                        reconheceSN(virgula);
                        reconheceID();
                        cont_var_global++;
                        if ((token_atual.categ == SN)
                                && (token_atual.codigo == abrecolchete)) {
                            reconheceSN(abrecolchete);
                            reconhece_intcon();
                            reconheceSN(fechacolchete);
                        }
                    }
                    reconheceSN(pontovirgula);
                    //


                    printf("AMEM %d\n", cont_var_global);

                }
            }
       // }
    } else {
        mensagemErro("Declaracao esperada");
    }
}

void tipo() {
    if ((token_atual.categ == PR) && ((token_atual.codigo == booleano)
            || (token_atual.codigo == caracter) || (token_atual.codigo == real)
            || (token_atual.codigo == inteiro))) {
        switch (token_atual.codigo) {
            case booleano:
                tipo_atual = T_BOOL;
                break;
            case caracter:
                tipo_atual = T_CARAC;
                break;
            case real:
                tipo_atual = T_REAL;
                break;
            case inteiro:
                tipo_atual = T_INT;
                break;
            default:
                tipo_atual = SEM_TIPO;
                break;
        }
        reconhecePR(token_atual.codigo);
    } else {
        mensagemErro("Tipo esperado");
    }
}

void tipos_param() {
    if ((token_atual.categ == PR) && (token_atual.codigo == semparam)) {
        reconhecePR(semparam);
        tipo_atual = SEM_TIPO;
    } else {
        tipo();
        if ((token_atual.categ == SN) && (token_atual.codigo == ecomercial)) {
            reconheceSN(ecomercial);
            categ_atual = PAR;
            reconheceID();
        } else {
            categ_atual = PAR;
            reconheceID();
            if ((token_atual.categ == SN)
                    && (token_atual.codigo == abrecolchete)) {
                reconheceSN(abrecolchete);
                reconheceSN(fechacolchete);
            }
        }
        while ((token_atual.categ == SN) && (token_atual.codigo == virgula)) {
            reconheceSN(virgula);
            tipo();
            if ((token_atual.categ == SN)
                    && (token_atual.codigo == ecomercial)) {
                reconheceSN(ecomercial);
                categ_atual = PAR;
                reconheceID();
            } else {
                categ_atual = PAR;
                reconheceID();
                if ((token_atual.categ == SN)
                        && (token_atual.codigo == abrecolchete)) {
                    reconheceSN(abrecolchete);
                    reconheceSN(fechacolchete);
                }
            }
        }
    }
}

void cmd() {
    if (token_atual.categ == PR) {
        if (token_atual.codigo == se) {
            reconhecePR(se);
            reconheceSN(abreparentese);
            expr();
            reconheceSN(fechaparentese);

            lx = gera_label();

            printf("GOFALSE L%d\n", lx);

            cmd();

            printf("LABEL L%d\n", lx);

            if ((token_atual.categ == PR) && (token_atual.codigo == senao)) {
                reconhecePR(senao);
                cmd();
            }
        } else if (token_atual.codigo == enquanto) {
            reconhecePR(enquanto);

            lx = gera_label();
            printf("LABEL L%d\n", lx);

            reconheceSN(abreparentese);
            expr();

            reconheceSN(fechaparentese);

            lx2 = gera_label();
            printf("GOFALSE L%d\n", lx2);

            cmd();

            printf("GOTO L%d\n", lx);
            printf("LABEL L%d\n", lx2);

        } else if (token_atual.codigo == para) {
            reconhecePR(para);
            reconheceSN(abreparentese);
            if (token_atual.categ == ID) {
                atrib();
            }
            reconheceSN(pontovirgula);

            lx = gera_label();
            printf("LABEL L%d\n", lx);

            if ((token_atual.categ == ID) || ((token_atual.categ == SN)
                    && ((token_atual.codigo == soma)
                    || (token_atual.codigo == subtracao)))) {
                expr();
            }

            reconheceSN(pontovirgula);

            lx2 = gera_label();
            printf("GOFALSE L%d\n", lx2);

            lx3 = gera_label();
            printf("GOTO L%d\n", lx3);

            lx4 = gera_label();
            printf("LABEL L%d\n", lx4);

            if (token_atual.categ == ID) {
                atrib();
            }

            printf("GOTO L%d\n", lx);
            printf("LABEL L%d\n", lx3);

            reconheceSN(fechaparentese);
            cmd();

            printf("GOTO L%d\n", lx4);
            printf("LABEL L%d\n", lx2);

        } else if (token_atual.codigo == retorne) {
            reconhecePR(retorne);
            if ((token_atual.categ == ID) || ((token_atual.categ == SN)
                    && ((token_atual.codigo == soma) || (token_atual.codigo == subtracao)))) {
                expr();
            }
            reconheceSN(pontovirgula);
        }
    } else if (token_atual.categ == ID) {
        if ((proximo_token.categ == SN)
                && (proximo_token.codigo == abreparentese)) {
            reconheceID();
            reconheceSN(abreparentese);
            if ((token_atual.categ == ID) || ((token_atual.categ == SN)
                    && ((token_atual.codigo == soma)
                    || (token_atual.codigo == subtracao)))) {
                expr();
                while ((token_atual.categ == SN)
                        && (token_atual.codigo == virgula)) {
                    reconheceSN(virgula);
                    expr();
                }
            }
            reconheceSN(fechaparentese);
            reconheceSN(pontovirgula);
        } else {
            atrib();
            reconheceSN(pontovirgula);
        }
    } else if ((token_atual.categ == SN) && (token_atual.codigo == abrechave)) {
        reconheceSN(abrechave);
        while (((token_atual.categ == PR) && ((token_atual.codigo == se)
                || (token_atual.codigo == para) || (token_atual.codigo == retorne)
                || (token_atual.codigo == enquanto))) || (token_atual.categ == ID)
                || ((token_atual.categ == SN)
                && ((token_atual.codigo == pontovirgula)
                || (token_atual.codigo == abrechave)))) {
            cmd();
        }
        reconheceSN(fechachave);
    } else {
        reconheceSN(pontovirgula);
    }
}

void atrib() {
     char lexema[31];
    if (token_atual.categ == ID) {
        strcpy(lexema, token_atual.lexema);
    }

    reconheceID();

    if ((token_atual.categ == SN) & (token_atual.codigo == abrecolchete)) {
        reconheceSN(abrecolchete);
        expr();
        reconheceSN(fechacolchete);
    }

    reconheceSN(atribui);
    expr();

    printf("STOR %s\n", lexema);
}

void expr() {
    expr_simp();
    if ((token_atual.categ == SN) &&
              ((token_atual.codigo == igual) || (token_atual.codigo == diferente)
            || (token_atual.codigo == menorigual) || (token_atual.codigo == menor)
            || (token_atual.codigo == maiorigual) || (token_atual.codigo == maior))) {

        op_rel();

        expr_simp();



    }
}

void expr_simp() {
    if ((token_atual.categ == SN) && ((token_atual.codigo == soma)
            || (token_atual.codigo == subtracao))) {


        reconheceSN(token_atual.codigo);
    }
    termo();
    while (((token_atual.categ == SN) && ((token_atual.codigo == soma)
            || (token_atual.codigo == subtracao) || (token_atual.codigo == ou)
            || (token_atual.codigo == abreparentese)
            || (token_atual.codigo == negacao))) || (token_atual.categ == CT_I)
            || (token_atual.categ == CT_R) || (token_atual.categ == CT_C)
            || (token_atual.categ == ID)) {
        if ((token_atual.categ == SN) && ((token_atual.codigo == soma)
                || (token_atual.codigo == subtracao)
                || (token_atual.codigo == ou))) {

            printf("%s\n", ((token_atual.codigo==soma) ? "ADD \n" : "SUB \n"));
            reconheceSN(token_atual.codigo);
        }
        termo();
    }
}

void termo() {
    fator();
    while (((token_atual.categ == SN) && (
                (token_atual.codigo == multiplicacao)
            || (token_atual.codigo == divisao)
            || (token_atual.codigo == ou)
            || (token_atual.codigo == abreparentese)
            || (token_atual.codigo == negacao)))
            || (token_atual.categ == CT_I)
            || (token_atual.categ == CT_R)
            || (token_atual.categ == CT_C)
            || (token_atual.categ == ID)) {
        if ((token_atual.categ == SN) && ((token_atual.codigo == multiplicacao)
                || (token_atual.codigo == divisao)
                || (token_atual.codigo == e))) {

            printf("%s\n", ((token_atual.codigo==multiplicacao) ? "MUL \n" : "DIV \n"));

            reconheceSN(token_atual.codigo);
        }
        fator();
    }
}

void fator() {
    char lexema[31];
    if (token_atual.categ == ID) {
        strcpy(lexema, token_atual.lexema);
        reconheceID();
        if ((token_atual.categ == SN) && (token_atual.codigo == abreparentese)) {
            reconheceSN(abreparentese);
            if ((token_atual.categ == ID) || ((token_atual.categ == SN)
                    && ((token_atual.codigo == soma)
                    || (token_atual.codigo == subtracao)))) {
                expr();
                while ((token_atual.categ == SN)
                        && (token_atual.codigo == virgula)) {
                    reconheceSN(virgula);
                    expr();
                }
            }
            reconheceSN(fechaparentese);
        } else {
            if ((token_atual.categ == SN)
                    && (token_atual.codigo == abrecolchete)) {
                reconheceSN(abrecolchete);
                expr();
                reconheceSN(fechacolchete);
            }
        }
    } else if (token_atual.categ == SN) {
        if (token_atual.codigo == abreparentese) {
            reconheceSN(abreparentese);
            expr();
            reconheceSN(fechaparentese);
        } else if (token_atual.codigo == negacao) {
            reconheceSN(negacao);
            fator();
        }
    } else if (token_atual.categ == CT_I) {

        printf("PUSH %d\n", token_atual.valor_int);

        reconhece_intcon();
    } else if (token_atual.categ == CT_R) {

        printf("PUSH %f\n", token_atual.valor_real);

        reconhece_realcon();
    } else if (token_atual.categ == CT_C) {
        reconhece_charcon();
    } else {
        mensagemErro("Fator esperado");
    }
}

void op_rel() {
    if ((token_atual.categ == SN) && ((token_atual.codigo == igual)
            || (token_atual.codigo == diferente)
            || (token_atual.codigo == menorigual)
            || (token_atual.codigo == menor)
            || (token_atual.codigo == maiorigual)
            || (token_atual.codigo == maior))) {
        reconheceSN(token_atual.codigo);
    } else {
        mensagemErro("Operador relacional esperado");
    }
}

int insereID(char *lexema){
    int i;
    switch (categ_atual) {
        case DECL:
            for (i = 0; i <= ind_tb; i++) {
                if ((strcmp(tab_sb[i].id, lexema) == 0) && ((tab_sb[i].categ == DECL) || (tab_sb[i].categ == FUNC)))
                    {
                    return 0;
                }
            }
            strcpy(tab_sb[ind_tb].id, lexema);
            tab_sb[ind_tb].categ = categ_atual;
            tab_sb[ind_tb].tipo = tipo_atual;
            tab_sb[ind_tb].escopo = escopo_atual;
            ind_tb++;
            return 1;
            break;
        case VAR_LOCAL:
            for (i = 0; i <= ind_tb; i++) {
                if ((strcmp(tab_sb[i].id, lexema) == 0) && (tab_sb[i].categ == VAR_LOCAL) && (strcmp(tab_sb[i].var_local.nome_funcao, n_func_atual) == 0)) {
                    return 0;
                }
            }
            strcpy(tab_sb[ind_tb].id, lexema);
            tab_sb[ind_tb].categ = categ_atual;
            tab_sb[ind_tb].tipo = tipo_atual;
            tab_sb[ind_tb].escopo = escopo_atual;
            strcpy(tab_sb[ind_tb].var_local.nome_funcao, n_func_atual);
            ind_tb++;
            return 1;
            break;
        default:
            strcpy(tab_sb[ind_tb].id, lexema);
            tab_sb[ind_tb].categ = categ_atual;
            tab_sb[ind_tb].tipo = tipo_atual;
            tab_sb[ind_tb].escopo = escopo_atual;
            ind_tb++;
            return 1;
            break;
    }
}

void updateID() {
    int i, j;
    char lexema[31];
    char mensagem[31];
    for (i = 0; i <= ind_tb; i++) {
        if (tab_sb[i].categ == SEM_CATEG) {
            j = i;
            strcpy(lexema, tab_sb[i].id);
        }
    }
    for (i = 0; i <= ind_tb; i++) {
        if ((strcmp(tab_sb[i].id, lexema) == 0) && (tab_sb[i].categ == categ_atual)) {
            sprintf(mensagem, "Redeclaracao de %s", lexema);
            mensagemErro(mensagem);
        }
    }
    tab_sb[j].categ = categ_atual;
}
int gera_label() {
    return cont_label++;
}
