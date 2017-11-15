#include "analex.h"

void TabelaPR() {
    int i = 0;
    char PR[TAM_L_TAB_PR][TAM_C_TAB_PR] = {"inteiro", "booleano", "caracter",
        "real", "para", "se", "senao", "retorne", "semparam", "semretorno","enquanto", "fim", "extern"};

    for (i = 0; i <= TAM_L_TAB_PR; i++)
        strcpy(TabPR[i], PR[i]);
}

void TabelaSN() {
    int i = 0;
    char SN[TAM_L_TAB_SN][TAM_C_TAB_SN] =
		{"{", "[", "(", "}", "]", ")",
		"+", "-", "*", "/",
		"=", "&", "&&", "||",
        "==", "!=", ">=", "<=", ">", "<",
		"!", ";", ","};

    for (i = 0; i <= TAM_L_TAB_SN; i++) strcpy(TabSN[i], SN[i]);
}

int letra(char c) {
    if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) return 1;
    else return 0;
}

int digito(char c) {
    if ((c >= '0') && (c <= '9')) return 1;
    else return 0;
}

int letradigito(char c) {
    if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0')
            && (c <= '9')) || (c == '_')) return 1;
    else return 0;
}

int arquivo() {
    if ((fp = fopen("teste.txt", "r")) == NULL) {
        printf("Arquivo nao encontrado.\n\n");
        system("PAUSE");
        exit(1);
    }
}

void mensagem(int tipo) {
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    switch (tipo) {
        case fimarquivo:
            printf("\nfim de arquivo \nlinhas: %d \n\n",
                    num_linhas);
            break;
        case constnaoterm:
            printf("\nconstante num nao terminada\n\nlinha: \
            %d\n\n", num_linhas);
            break;
        case caracinvalido:
            printf("\ncaracter invalido\n\nlinha: %d\n\n", num_linhas);
            break;
        case caraccon:
            printf("\nconstande de char invalida.\n\nlinha: %d\n\n", num_linhas);
            break;
    }
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    system("PAUSE");
    exit(1);
}

void caracterinvalido(char c) {
    printf("caracter invalido: %c\n\nLinha: %d\n\n", c,
            num_linhas);
    system("PAUSE");
    exit(1);
}

token analex() {
    token t;
    char cadeia[1000];
    int ind = 0;
    int i = 0;
    estado = 0;
    while (1) {
        c = getc(fp);
        switch (estado) {
            case 0:
                if ((c == ' ') || (c == '\t')) {
                    ;
                }
				else if (c == '\n') {
                    num_linhas++;
                }
				else if (letra(c)) {
                    cadeia[ind] = c;
                    ind++;
                    estado = 1;
                }
				else if ((c == '{') || (c == '[') || (c == '(') || (c == '+')
                        || (c == '}') || (c == ']') || (c == ')') || (c == '*')
                        || (c == ';') || (c == '-') || (c == ',')) {
                    cadeia[ind] = c;
                    ind++;
                    cadeia[ind] = '\0';
                    estado = 3;
                }
				else if (c == '&') {

				    estado = 4;
                }
				else if (c == '!') {
                    estado = 7;
                }
				else if (c == '=') {
                    estado = 10;
                }
				else if (c == '>') {
                    estado = 13;
                }
				else if (c == '<') {
                    estado = 16;
                }
				else if (c == '|') {
                    estado = 19;
                }
				else if (digito(c)) {
                    cadeia[ind] = c;
                    ind++;
                    estado = 21;
                } else if (c == '\'') {
                    estado = 26;
                } else if (c == '\"') {
                    estado = 30;
                } else if (c == '/') {
                    cadeia[ind] = c;
                    ind++;
                    estado = 32;
                } else if (c == fim) {
                    mensagem(fimarquivo);
                } else {
                    caracterinvalido(c);
                }
                break;

            case 1:
                if ((letradigito(c))) {
                    cadeia[ind] = c;
                    ind++;
                }
				else {
                    ungetc(c, fp);
                    cadeia[ind] = '\0';
                    estado = 2;
                }
                break;

            case 2:
                ungetc(c, fp);
                for (i = 0; i < TAM_L_TAB_PR; i++) {
                    if (strncmp(cadeia, TabPR[i],
                            TAM_C_TAB_PR) == 0) {
                        t.categ = PR;
                        t.codigo = i;
                        return t;
                    }
                }
                t.categ = ID;
                strcpy(t.lexema, cadeia);
                return t;
                break;

            case 3:
                ungetc(c, fp);
                for (i = 0; i < TAM_L_TAB_SN; i++) {
                    if (strncmp(cadeia, TabSN[i],
                            TAM_C_TAB_SN) == 0) {
                        t.categ = SN;
                        t.codigo = i;
                        return t;
                    }
                }
                break;

            case 4:
                if (c == '&') {
                    estado = 6;
                }
				else {
                    ungetc(c, fp);
                    estado = 5;
                }
                break;

            case 5:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = ecomercial;
                return t;
                break;

            case 6:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = e;
                return t;
                break;

            case 7:
                if (c == '=') {
                    estado = 9;
                }
				else {
                    ungetc(c, fp);
                    estado = 8;
                }
                break;

            case 8:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = negacao;
                return t;
                break;

            case 9:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = diferente;
                return t;
                break;

            case 10:
                if (c == '=') {
                    estado = 12;
                }
				else {
                    ungetc(c, fp);
                    estado = 11;
                }
                break;

            case 11:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = atribui;
                return t;
                break;

            case 12:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = igual;
                return t;
                break;

            case 13:
                if (c == '=') {
                    estado = 15;
                }
				else {
                    ungetc(c, fp);
                    estado = 14;
                }
                break;

            case 14:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = maior;
                return t;
                break;

            case 15:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = maiorigual;
                return t;
                break;

            case 16:
                if (c == '=') {
                    estado = 18;
                }
				else {
                    ungetc(c, fp);
                    estado = 17;
                }
                break;

            case 17:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = menor;
                return t;
                break;

            case 18:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = menorigual;
                return t;
                break;

            case 19:
                if (c == '|') {
                    estado = 20;
                }
				else {
                    caracterinvalido('|');
                }
                break;

            case 20:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = ou;
                return t;
                break;

            case 21:
                if (digito(c)) {
                    cadeia[ind] = c;
                    ind++;

                }
				else if (c == '.') {
                    cadeia[ind] = c;
                    ind++;
                    estado = 23;
                }
				else {
                    ungetc(c, fp);
                    cadeia[ind] = '\0';
                    estado = 22;
                }
                break;

            case 22:
                ungetc(c, fp);
                t.categ = CT_I;
                t.valor_int = atoi(cadeia);
                return t;
                break;

            case 23:
                if (digito(c)) {
                    cadeia[ind] = c;
                    ind++;
                    estado = 24;
                }
				else {
                    mensagem(constnaoterm);
                }
                break;

            case 24:
                if (digito(c)) {
                    cadeia[ind] = c;
                    ind++;
                }
				else {
                    ungetc(c, fp);
                    cadeia[ind] = '\0';
                    estado = 25;
                }
                break;

            case 25:
                ungetc(c, fp);
                t.categ = CT_R;
                t.valor_real = atof(cadeia);
                return t;
                break;

            case 26:
                if (c == '\\') {
                    cadeia[ind] = c;
                    ind++;
                    estado = 27;
                }
				else if ((isprint(c) != 0) && (c != '\'')) {
                    cadeia[ind] = c;
                    ind++;
                    estado = 28;
                }
				else {
                    mensagem(caraccon);
                }
                break;

            case 27:
                if ((isprint(c) != 0) && (c != '\'')) {
                    cadeia[ind] = c;
                    ind++;
                    estado = 28;
                }
				else {
                    mensagem(caraccon);
                }

            case 28:
                if (c == '\'') {
                    cadeia[ind] = '\0';
                    estado = 29;
                }
				else {
                    mensagem(caraccon);
                }
                break;

            case 29:
                ungetc(c, fp);
                t.categ = CT_C;
                t.indice = ind_lt;
                strcpy(TabLT[ind_lt], cadeia);
                ind_lt++;
                return t;
                break;

            case 30:
                if (c == '\"') {
                    cadeia[ind] = '\0';
                    estado = 31;
                }
				else {
                    cadeia[ind] = c;
                    ind++;
                }
                break;

            case 31:
                ungetc(c, fp);
                t.categ = CT_C;
                t.indice = ind_lt;
                strcpy(TabLT[ind_lt], cadeia);
                ind_lt++;
                return t;
                break;

            case 32:
                if (c == '*') {
                    ind = 0;
                    estado = 34;
                }
				else {
                    ungetc(c, fp);
                    cadeia[ind] = '\0';
                    estado = 33;
                }
                break;

            case 33:
                ungetc(c, fp);
                t.categ = SN;
                t.codigo = divisao;
                return t;
                break;

            case 34:
                if (c == '*') {
                    estado = 35;
                }
				else if (c == fim) {
                    mensagem(fimarquivo2);
                }
				else if (c == '\n') {
                    num_linhas++;
                }
				else {
                    ;
                }
                break;

            case 35:
                if (c == '*') {
                    ;
                }
				else if (c == '/') {
                    estado = 0;
                }
				else if (c == fim) {
                    mensagem(fimarquivo2);
                }
				else {
                    if (c == '\n') {
                        num_linhas++;
                    }
                    estado = 34;
                }
                break;
            default:
                caracterinvalido(c);
                break;
        }
    }
}

void imprime(token t) {
    switch (t.categ) {
        case ID: printf("  [ID]  %s\n\n", t.lexema);
            break;
        case PR: printf("  [PR]  %s\n\n", TabPR[t.codigo]);
            break;
        case SN: printf("  [SN]  %s\n\n", TabSN[t.codigo]);
            break;
        case CT_I: printf("  [CT_I]  %d\n\n", t.valor_int);
            break;
        case CT_R: printf("  [CT_R]  %f\n\n", t.valor_real);
            break;
        case CT_C: printf("  [CT_C]  \"%s\"\n\n", TabLT[t.indice]);
            break;
    }
}
